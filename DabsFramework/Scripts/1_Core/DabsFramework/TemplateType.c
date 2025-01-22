
// Gets typename from Templated type
// This is useful when getting this error...
// 'Can't get typename from forward declaration 'Class'. Use Type("name") instead....'
class TemplateType<Class T>: Managed
{
	private void TemplateType();
	private void ~TemplateType();

	T value;

	static typename GetType()
	{
		auto templateType = new TemplateType<T>();
		typename varType = templateType.Type().GetVariableType(0);
		return varType;
	}
}