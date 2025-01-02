static int DeletedObjectId;

/* 
	For future reference, it is possible now with GetDebugName() to obtain the id and p3d of an object safely
	this would not be a terrible way to ensure deleted objects without a Type dont get improperly removed
	however we need to backport all old information and handle empty Types from the past

	Future reference here: were gonna use shape name!
*/



class EditorDeletedObjectData: SerializableBase
{
	[NonSerialized()]
	int ID;
	
	// Depreciated
	string Type;
	
	string ModelName;
	
	vector Position;
	int Flags;
	
	[NonSerialized()]
	vector Orientation; // explicitly for expansion .map file
	
	[NonSerialized()]
	Object WorldObject;
	
	void EditorDeletedObjectData()
	{
		DeletedObjectId++;
		ID = DeletedObjectId;
	}
	
	int GetID()
	{
		return ID;
	}
	
	static EditorDeletedObjectData Create(string type, vector position, vector orientation = vector.Zero)
	{		
		EditorDeletedObjectData data = new EditorDeletedObjectData();
		data.Type = type;
		data.Position = position;
		data.Orientation = orientation;
	
		data.WorldObject = data.FindObject();		
		if (data.WorldObject) {
			data.ModelName = data.WorldObject.GetShapeName();
		}
		
		return data;
	}
	
	static EditorDeletedObjectData Create(Object object)
	{
		if (!object) {
			return null;
		}
		
		EditorDeletedObjectData data = new EditorDeletedObjectData();
		data.Type = object.GetType();
		data.ModelName = object.GetShapeName();
		data.Position = object.GetWorldPosition();
		data.WorldObject = object;
		data.Orientation = object.GetOrientation();
		return data;
	}
	
	Object FindObject(float radius = 0.1)
	{
		array<Object> objects = {};
		GetGame().GetObjectsAtPosition3D(Position, radius, objects, null);			
		
		foreach (Object object: objects) {
			if (!object) {
				continue;
			}

			// New way of doing it
			if (ModelName && ModelName == object.GetShapeName()) {
				return object;
			}
			
			// Deprecated
			string type;
			g_Game.ObjectGetType(object, type);
			if (type == Type) {
				return object;
			}
		}
		
		Error(string.Format("[DF] Failed to find %1:%4 in %2m of %3", Type, radius, Position, ModelName)); 
		return null;
	}
	
	override void Write(Serializer serializer, int version)
	{
		serializer.Write(Type);
		serializer.Write(Position);
		serializer.Write(Flags);
		serializer.Write(ModelName);
	}
	
	override bool Read(Serializer serializer, int version)
	{
		serializer.Read(Type);
		serializer.Read(Position);
		serializer.Read(Flags);
		
		if (version >= 6) {
			serializer.Read(ModelName);
		}
		
		return true;
	}
}