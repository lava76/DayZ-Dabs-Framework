class EditorCameraTrackData: EditorObjectData
{
	float Time;
	
	static EditorCameraTrackData Create(notnull Camera camera, float time, EditorObjectFlags flags = EFE_DEFAULT)
	{
		EditorCameraTrackData data = new EditorCameraTrackData();
		data.Type = "DSLRCamera";
		data.Position = camera.GetPosition();
		data.Orientation = camera.GetOrientation();
		data.Time = time;
		data.Flags = flags;
		return data;
	}

	override void Write(Serializer serializer, int version)
	{
		super.Write(serializer, version);
		serializer.Write(Time);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		if (!super.Read(serializer, version)) {
			return false;
		}
		
		serializer.Read(Time);
		return true;
	}
}