class ListBoxPrefabEntry<Class T>: ScriptView
{
	ButtonWidget Button;
	TextWidget Label;
	ImageWidget Icon;
	
	protected T m_Value;
	
	protected ListBoxPrefab<T> m_Owner;
		
	void ListBoxPrefabEntry(string caption, T data, string icon = "", LinearColor color = LinearColor.BLUE)
	{
		Label.SetText(caption);
		Icon.LoadImageFile(0, icon);
		Icon.SetImage(0);
		Icon.Show(icon != string.Empty);
		Button.SetColor(color);
		
		m_Value = data;
	}
		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		m_Owner.EntryOnClick(m_Value, w, x, y, button);
		return true;
	}
	
	override bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		m_Owner.EntryOnDoubleClick(m_Value, w, x, y, button);
		return true;
	}
	
	void SetOwner(ListBoxPrefab<T> owner)
	{
		m_Owner = owner;
	}
	
	ListBoxPrefab<T> GetOwner()
	{
		return m_Owner;
	}
	
	T GetData()
	{
		return m_Value;
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/Listbox/ListBoxEntryPrefab.layout";
	}
}