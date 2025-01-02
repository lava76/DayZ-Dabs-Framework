class EditorSaveData
{
	[NonSerialized()]
	static const string BIN_CHECK = "EditorBinned";
	
	static const int Version = 8;
	string MapName;
	string AuthorId; // Steamid64 of original author
	ref array<string> CreditIds = {}; // SteamId64 of all users that have edited / saved this build
	DateTime LastModified;
	DateTime CreationDate;
	vector CameraPosition;
	
	ref array<ref EditorObjectData> EditorObjects = {};
	ref array<ref EditorDeletedObjectData> EditorHiddenObjects = {};	
	ref array<ref EditorCameraTrackData> CameraTracks = {};
		
	void ~EditorSaveData()
	{
		delete EditorObjects;
		delete EditorHiddenObjects;
		delete CameraTracks;
	}
	
	static EditorSaveData CreateNew(string uid, DateTime now)
	{
		EditorSaveData new_save_data = new EditorSaveData();
		new_save_data.AuthorId = uid;
		new_save_data.CreditIds.Insert(uid);
		new_save_data.CreationDate = now;
		return new_save_data;
	}
		
	void Write(Serializer serializer, int version)
	{
		serializer.Write(BIN_CHECK);
		serializer.Write(Version);
		serializer.Write(MapName);
		serializer.Write(CameraPosition);
		
		serializer.Write(AuthorId);
		serializer.Write(CreditIds.Count());
		foreach (auto credit: CreditIds) {
			serializer.Write(credit);
		}
		
		serializer.Write(LastModified);
		serializer.Write(CreationDate);
		
		serializer.Write(EditorObjects.Count());
		foreach (EditorObjectData data: EditorObjects) {
			data.Write(serializer, Version);
		}
		
		serializer.Write(EditorHiddenObjects.Count());
		foreach (EditorDeletedObjectData deleted_data: EditorHiddenObjects) {
			deleted_data.Write(serializer, Version);
		}

		serializer.Write(CameraTracks.Count());
		foreach (EditorCameraTrackData camera_track: CameraTracks) {
			camera_track.Write(serializer, Version);
		}
	}
	
	bool Read(Serializer serializer)
	{
		string bincheck;
		serializer.Read(bincheck);
		if (bincheck != BIN_CHECK) {
			return false;
		}
		
		int read_version;
		serializer.Read(read_version);
		serializer.Read(MapName);
		serializer.Read(CameraPosition);
		
		// Forcing this towards the beginning of a file to make it readable easier
		if (read_version >= 7) {
			serializer.Read(AuthorId);
			int credits_count;
			serializer.Read(credits_count);
			for (int l = 0; l < credits_count; l++) {
				string credit;
				serializer.Read(credit);
				CreditIds.Insert(credit);
			}
			
			serializer.Read(LastModified);
			serializer.Read(CreationDate);
		} else {
			LastModified = DateTime.Now();
			CreationDate = DateTime.Now();
		}
		
		int editor_object_count;
		serializer.Read(editor_object_count);
		for (int i = 0; i < editor_object_count; i++) {
			EditorObjectData data();
			data.Read(serializer, read_version);
			EditorObjects.Insert(data);
		}
		
		int editor_deleted_object_count;
		serializer.Read(editor_deleted_object_count);
		for (int j = 0; j < editor_deleted_object_count; j++) {
			EditorDeletedObjectData deleted_data();
			deleted_data.Read(serializer, read_version);
			EditorHiddenObjects.Insert(deleted_data);
		}

		if (read_version <= 5) {
			return true;
		}

		int camera_track_count;
		serializer.Read(camera_track_count);
		for (int k = 0; k < camera_track_count; k++) {
			EditorCameraTrackData camera_track = new EditorCameraTrackData();
			camera_track.Read(serializer, read_version);
			CameraTracks.Insert(camera_track);
		}		
		
		return true;
	}
	
	static bool IsBinnedFile(string file)
	{
		FileSerializer serializer();
		
		if (!serializer.Open(file)) {
			return false;
		}
		
		string bincheck;
		if (!serializer.Read(bincheck)) {
			serializer.Close();
			return false;
		}
		
		serializer.Close();
		
		return (bincheck == BIN_CHECK);
	}
	
	static EditorSaveData LoadFromFile(string file)
	{
		bool binned = IsBinnedFile(file);
		
		FileSerializer serializer = new FileSerializer();
		EditorSaveData save_data = new EditorSaveData();		
					
		if (binned) {	
			if (!serializer.Open(file, FileMode.READ)) {
				Error("Failed to open file " + file);
				return null;
			}
			
			if (!save_data.Read(serializer)) {
				Error("Failed to read file " + file);
				serializer.Close();
				return null;
			}
			
		} else {
			FileHandle fh = OpenFile(file, FileMode.READ);
			string jsonData;
			string error;
	
			if (!fh) {
				Error("Could not open file " + file);
				return null;
			}
		
			string line;
			while (FGets(fh, line) > 0) {
				jsonData = jsonData + "\n" + line;
			}
			
			CloseFile(fh);
	
			bool success = JsonSerializer().ReadFromString(save_data, jsonData, error);
			if (error != string.Empty || !success) {
				Error(error);
				return null;
			}
		}
		
		serializer.Close();		
		return save_data;
	}
}