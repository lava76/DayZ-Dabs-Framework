class EditBoxNumberPrefab: PrefabBase<StringEvaluater>
{
	protected float m_StepSize, m_Min, m_Max;
	
	EditBoxWidget ContentText;
	
	void EditBoxNumberPrefab(string caption, Class binding_context, string binding_name, float step_size = 1, float min = float.MIN, float max = float.MAX)
	{
		m_StepSize = step_size;
		m_Min = min;
		m_Max = max;
	}

	override bool OnMouseWheel(Widget w, int x, int y, int wheel)
	{
		float motion = wheel;
		if (m_BindingVariableType == float) {
			motion *= 0.5;
		}
		
		motion *= m_StepSize;
		
		if (KeyState(KeyCode.KC_LCONTROL)) {
			motion *= 10;
		}
		
		if (KeyState(KeyCode.KC_LSHIFT)) {
			motion *= 0.1;
		}
		
		switch (w.GetName()) {
			case "ContentText": {
				m_PrefabBaseController.Value = string.ToString(Math.Clamp(m_PrefabBaseController.Value.Parse() + motion, m_Min, m_Max));
				m_PrefabBaseController.NotifyPropertyChanged("Value");
				break;
			}
		}
		
		return true;
	}
	
	override StringEvaluater GetDefaultValue(Class binding_context, string binding_name)
	{
		switch (m_BindingVariableType) {
			case float: {
				float float_value;
				EnScript.GetClassVar(binding_context, binding_name, 0, float_value);
				float_value = Math.Clamp(float_value, m_Min, m_Max);
				return float_value.ToString();
			}
			
			case int: {
				int int_value;
				EnScript.GetClassVar(binding_context, binding_name, 0, int_value);
				int_value = Math.Clamp(int_value, m_Min, m_Max);
				return int_value.ToString();
			}
		}
		
		return string.Empty;
	}
	
	override void PrefabPropertyChanged(string property_name)
	{
		float raw_value = m_PrefabBaseController.Value.Parse();
		raw_value = Math.Clamp(raw_value, m_Min, m_Max);
		switch (m_BindingVariableType) {
			case int: {
				// needs to be exact type, engine wont cast for us
				EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, (int)raw_value); 
				break;
			}
			
			case float: {
				EnScript.SetClassVar(m_BindingContext, m_BindingName, 0, raw_value);
				break;
			}
			
			default: {
				Error("Invalid var type for NumberPrefab, use int or float");
				return;
			}
		}
		
		g_Script.Call(m_BindingContext, "PropertyChanged", m_BindingName);
	}
	
	override string GetLayoutFile() 
	{
		return "DabsFramework/gui/Layouts/prefabs/EditBoxPrefab.layout";
	}
}