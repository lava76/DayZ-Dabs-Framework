//@ It is your responsibility to manage the memory for this menu object
class ScriptViewMenu: ScriptView
{		
	static ref map<UIScriptedMenu, ScriptViewMenu> AllByMenu = new map<UIScriptedMenu, ScriptViewMenu>();
	
	protected ref UIScriptViewMenu m_UIScriptViewMenu = new UIScriptViewMenu(this);
	
	//@ This menu will control its subsequent children UI menus, but NOT children ScriptViewMenu!!!
	protected ref UIScriptedMenu m_ChildMenu;
	
	void ScriptViewMenu()
	{
		if (!AllByMenu) {
			AllByMenu = new map<UIScriptedMenu, ScriptViewMenu>();
		}
		
		AllByMenu[m_UIScriptViewMenu] = this;
		// We are the parent menu
		g_Game.GetUIManager().ShowScriptedMenu(m_UIScriptViewMenu, g_Game.GetUIManager().GetMenu());
		
		// Handle input excludes
		g_Game.GetMission().AddActiveInputExcludes(GetInputExcludes());		
		array<int> input_restrictions = {};
		foreach (int input_restriction: input_restrictions) {
			UAInput input = GetUApi().GetInputByID(input_restriction);
			if (!input) {
				Error(string.Format("Invalid input specified in input restrictions idx=%1", input_restriction));
				continue;
			}
			
			input.ForceEnable(true);
		}
		
		// Handles hiding / showing cursor
		if (UseMouse()) {
			g_Game.GetInput().ChangeGameFocus(1, INPUT_DEVICE_MOUSE);
		}
		
		if (UseKeyboard()) {
			g_Game.GetInput().ChangeGameFocus(1, INPUT_DEVICE_KEYBOARD);
		}
		
		// 2 is required, 1 worked but i wanted to be safe. reason: calling CloseAll() on the same frame would lock the cursor away in a vault
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(g_Game.SetMouseCursorDesiredVisibility, 2, false, UseMouse());
	}
	
	void ~ScriptViewMenu()
	{
		// This lazy calls the destructor
		if (m_UIScriptViewMenu) {
			UIScriptedMenu parent_menu = UIScriptedMenu.Cast(m_UIScriptViewMenu.GetParentMenu());
			AllByMenu.Remove(m_UIScriptViewMenu);
			g_Game.GetUIManager().HideScriptedMenu(m_UIScriptViewMenu);
			
			if (parent_menu) {
				g_Game.GetUIManager().ShowScriptedMenu(parent_menu, parent_menu.GetParentMenu());
			}
		}
		
		bool use_mouse_parent = (m_UIScriptViewMenu && m_UIScriptViewMenu.GetParentMenu() && m_UIScriptViewMenu.GetParentMenu().UseMouse());
		// Mouse control
		if (UseMouse() && !use_mouse_parent) {
			//g_Game.GetInput().ChangeGameFocus(-1, INPUT_DEVICE_MOUSE);
		}
		
		bool use_keyboard_parent = (m_UIScriptViewMenu && m_UIScriptViewMenu.GetParentMenu() && m_UIScriptViewMenu.GetParentMenu().UseKeyboard());
		if (UseKeyboard() && !use_keyboard_parent) {
			//g_Game.GetInput().ChangeGameFocus(-1, INPUT_DEVICE_KEYBOARD);
		}
		
		g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(g_Game.SetMouseCursorDesiredVisibility, 0, false, use_mouse_parent);
		
		// input excludes
		if (g_Game.GetMission()) {
			g_Game.GetMission().RemoveActiveInputExcludes(GetInputExcludes());
		}
		
		// input restrictions
		array<int> input_restrictions = {};
		foreach (int input_restriction: input_restrictions) {
			UAInput input = GetUApi().GetInputByID(input_restriction);
			if (!input) {
				continue;
			}
			
			input.ForceEnable(false);
		}
	}
	
	void ShowDialog(string caption, string text, int id, int buttons /*DBT_*/, int default_button /*DBB_*/, int type /*DMT_*/)
	{
		g_Game.GetUIManager().ShowDialog(caption, text, id, buttons, default_button, type, m_UIScriptViewMenu);
	}
	
	//@ important functions called by the UI manager when entering and leaving child menus
	//		as much as i want to put this in the constructor, you cant because menus can be hidden and shown as they juggle
	//		back and fourth. and we dont want to delete a menu when showing a child menu
	void OnMenuEnter(UIMenuPanel parent_panel)
	{		
		// Handles re-showing parent menu since UIManager automatically hides it
		UIScriptedMenu parent_menu = UIScriptedMenu.Cast(parent_panel);
		if (!HidesParentMenu(parent_panel) && parent_menu && parent_menu.GetLayoutRoot()) {
			parent_menu.GetLayoutRoot().Show(true);
		}
	}
	
	void OnMenuExit(UIMenuPanel parent_panel)
	{
		if (m_ChildMenu) {
			m_LayoutRoot.Show(false);
		} else {
			Close();
		}
	}
	
	UIScriptedMenu EnterChildMenu(int id)
	{
		return EnterChildMenu(GetGame().GetMission().CreateScriptedMenu(id));
	}
		
	UIScriptedMenu EnterChildMenu(ScriptViewMenu menu)
	{
		return EnterChildMenu(menu.GetUIScriptViewMenu());
	}
	
	UIScriptedMenu EnterChildMenu(UIScriptedMenu scripted_menu)
	{
		m_ChildMenu = scripted_menu;
		g_Game.GetUIManager().ShowScriptedMenu(m_ChildMenu, m_UIScriptViewMenu);
		return m_ChildMenu;
	}
	
	//! Is mouse cursor shown on menu enter
	bool UseMouse()
	{
		return true;
	}
	
	bool UseKeyboard()
	{
		return false;
	}
	
	//! see <exclude> in bin/specific.xml
	array<string> GetInputExcludes()
	{
		return {};
	}
	
	//! list of UA inputs to force
	array<int> GetInputRestrictions()
	{
		return {};
	}
	
	bool HidesParentMenu(UIMenuPanel parent_panel)
	{
		return true;
	}
	
	void Close()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(_Close);
	}
	
	bool CanClose()
	{
		return true;
	}
	
	// tbd how i want to handle this. ive implemented it in the playground in MG::OnUpdate but DF is a weird one
	bool CanCloseWithEscape()
	{
		return CanClose();
	}
	
	private void _Close()
	{
		if (CanClose()) {
			delete this;
		}
	}
		
	UIScriptViewMenu GetUIScriptViewMenu()
	{
		return m_UIScriptViewMenu;
	}
	
	UIScriptedMenu GetChildMenu()
	{
		return m_ChildMenu;
	}
	
	static ScriptViewMenu Get(UIScriptedMenu menu)
	{
		if (!AllByMenu) {
			return null;
		}
		
		return AllByMenu[menu];
	}
}