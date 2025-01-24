modded class MainMenu
{
	protected ButtonWidget m_PatreonButton;
	
	override void Refresh()
	{
		super.Refresh();
		
		Widget settings_panel = layoutRoot.FindAnyWidget("settings_panel_root");
		if (!settings_panel || m_PatreonButton) {
			return;
		}
		
		float s_sp_w, s_sp_h, sp_w, sp_h;
		settings_panel.GetSize(sp_w, sp_h);
		settings_panel.GetScreenSize(s_sp_w, s_sp_h);
		s_sp_h *= (37.0 / 40.0);
		
		int children_count;
		Widget child = settings_panel.GetChildren();
		while (child) {
			children_count++;
			float c_x, c_y;
			child.GetPos(c_x, c_y);
			child.SetPos(c_x * 0.823, c_y);

			float cs_x, cs_y;
			child.GetScreenSize(cs_x, cs_y);
			
			child = child.GetSibling();
		}
		
		settings_panel.SetSize(sp_w + (sp_w / children_count), s_sp_h);
		
		m_PatreonButton = ButtonWidget.Cast(GetGame().GetWorkspace().CreateWidget(ButtonWidgetTypeID, 0, 0, s_sp_h, s_sp_h, WidgetFlags.HEXACTSIZE | WidgetFlags.VEXACTSIZE | WidgetFlags.VEXACTPOS | WidgetFlags.NOFOCUS | WidgetFlags.VISIBLE | WidgetFlags.CLIPCHILDREN, -1, 0, settings_panel));
		if (!m_PatreonButton) {
			return;
		}
		
		m_PatreonButton.SetName("patreon_button");
		m_PatreonButton.SetPos(0.15 - (s_sp_h / sp_w), 0);
		
		ImageWidget patreon_image_widget = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidget(ImageWidgetTypeID, 0, 0, 1, 1, WidgetFlags.STRETCH | WidgetFlags.SOURCEALPHA | WidgetFlags.BLEND | WidgetFlags.VISIBLE | WidgetFlags.IGNOREPOINTER, -1, 0, m_PatreonButton));		
		if (patreon_image_widget) {
			patreon_image_widget.SetSize(1, 1);
			patreon_image_widget.SetName("patreon_button_image");
			Symbols.HAND_HOLDING_DOLLAR.Load(patreon_image_widget);
			// what a terrible icon patreon
			//patreon_image_widget.LoadImageFile(0, "set:brands image:patreon");
			//patreon_image_widget.SetImage(0);
		}
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if (super.OnClick(w, x, y, button)) {
			return true;
		}
		
		if (button == MouseState.LEFT) {
			if (w == m_PatreonButton) {
				GetGame().OpenURL("https://patreon.com/inclementdab?utm_medium=unknown&utm_source=join_link&utm_campaign=creatorshare_creator&utm_content=copyLink");
				return true;
			}
		}

		return false;
	}
	
	override bool IsFocusable(Widget w)
	{
		if (super.IsFocusable(w)) {
			return true;
		}

		return (w == m_PatreonButton);
	}
}