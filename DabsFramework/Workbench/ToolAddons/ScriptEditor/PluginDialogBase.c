class PluginDialogBase: WorkbenchPlugin
{	
	// Sizes the dialog to max size without putting a scroll bar on the bottom
	static const string DIALOG_TAB_SIZE = "\t\t\t\t\t\t\t\t\t";
	static const string DEFAULT_EXTENSION = ".c";
	static const ref array<string> LOG_FILE_TYPES = {".log", ".rpt", ".adm", ".mdmp"};
	static const ref array<string> WB_DIR_DEFAULTS = {"Addons", "bliss", "sakhal", "dta", "platforms", "battleye"};
	static const ref array<string> SCRIPT_MODULES = {"1_core", "2_gamelib", "3_game", "4_world", "5_mission", "workbench"};
	
	protected ScriptEditor m_ScriptEditor = Workbench.GetModule("ScriptEditor");
	protected ResourceBrowser m_ResourceBrowser = Workbench.GetModule("ResourceManager");
	
	void ErrorDialog(string error)
	{
		Workbench.Dialog(string.Format("Error: %1", Type()), error);
	}

#ifdef WORKBENCH_PLUS_DEBUG
	
	override void Run()
	{
		PrintFormat("Run: %1", Type());
	}
	
	override void RunCommandline()
	{
		PrintFormat("RunCommandLine: %1", Type());
	}
	
#endif
	
	static string GetPrefix()
	{
		array<string> current_dir_split = {};
		string current_directory = GetCurrentDirectory();
		current_directory.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		current_directory.Split(SystemPath.SEPERATOR, current_dir_split);
		return current_dir_split[current_dir_split.Count() - 2];
	}
	
	static string GetDayZDirectory(LaunchSettings settings, int environmentType = -1)
	{
		if (environmentType == -1) {
			environmentType = settings.EnvironmentType;
		}
		
		if (environmentType == DayZEnvironmentType.CUSTOM) {
			string gamePath = settings.CustomGameDirectory;
			gamePath.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
			return gamePath;
		}
		
		string dayzType;
		switch (environmentType)
		{
		case DayZEnvironmentType.STABLE:
			dayzType = "dayz";
			break;
		case DayZEnvironmentType.EXPERIMENTAL:
			dayzType = "dayz exp";
			break;
		}
		
		string data = GetRegistryEntryValue(string.Format("HKLM\\SOFTWARE\\Wow6432Node\\Bohemia Interactive\\%1", dayzType), "main");
		data.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		return data;
	}
	
	static string GetRegistryEntryValue(string entry, string value)
	{
		string output_file = string.Format("%1\\%2", GetWorkbenchDirectory(), "temp.txt");
		
		string cmd = "for /F \"tokens=2*\" %a in ('reg query \"" + entry + "\" /v \"" + value + "\"') DO (Echo %b) > \"" + output_file + "\"";
		Workbench.RunCmd("cmd /c \"" + cmd + "\"", true);
		
		FileHandle handle = OpenFile(output_file, FileMode.READ);
		if (!handle) {
			return string.Empty;
		}
		
		string data, buffer;
		while (ReadFile(handle, buffer, 256) != 0) {
		    data += buffer;
		}
		
		CloseFile(handle);
		DeleteFile(output_file);
		data = data.Trim();
		data.Replace("\n", "");
		data.Replace("\r", "");
		return data;
	}
	
	static string GetSourceDataDirectory()
	{
		string abs;
		Workbench.GetAbsolutePath("$SourceData:", abs);
		abs.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		return abs;
	}
	
	static string GetCurrentDirectory()
	{
		string abs;
		Workbench.GetAbsolutePath("$CurrentDir:", abs);
		abs.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		return abs;
	}
	
	static string GetWorkbenchDirectory()
	{
		string workbench_dir;
		Workbench.GetCwd(workbench_dir);
		
		if (!FileExist(string.Format("%1\\%2", workbench_dir, "workbenchApp.exe"))) {
			return string.Empty;
		}
		
		return workbench_dir;
	}
	
	static string GetRootDirectory()
	{
		string root_dir;
		Workbench.GetAbsolutePath(string.Empty, root_dir);
		return root_dir;
	}
	
	static string GetAbsolutePath(string path)
	{
		string absolute_path;
		Workbench.GetAbsolutePath(path, absolute_path);
		return absolute_path;
	}
	
	static string GetRelativePath(string root_path, string full_path)
	{
		root_path.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		full_path.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		
		array<string> root_path_split = {};
		array<string> full_path_split = {};
		
		root_path.Split(SystemPath.SEPERATOR, root_path_split);
		full_path.Split(SystemPath.SEPERATOR, full_path_split);
		
		string result;
		for (int i = 0; i < full_path_split.Count(); i++) {
			if (i > root_path_split.Count() - 1 || full_path_split[i] != root_path_split[i]) {
				result += full_path_split[i];
				if (i != full_path_split.Count() - 1) {
					result += SystemPath.SEPERATOR;
				}
			}
		}
		
		return result;
	}
	
	static string GetDirectory(string path)
	{		
		path.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		
		array<string> path_split = {};
		path.Split(SystemPath.SEPERATOR, path_split);
		string directory;
		for (int i = 0; i < path_split.Count() - 1; i++) {
			if (path_split[i].Contains(".")) {
				// pop upon finding file. Substring because we added a seperator to the end last loop
				return directory;
			}
			
			directory += path_split[i];
			if (i != path_split.Count() - 1) {
				directory += SystemPath.SEPERATOR;
			}
		}
		
		return directory;
	}
	
	static string GetFile(string path)
	{
		path.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		
		array<string> path_split = {};
		path.Split(SystemPath.SEPERATOR, path_split);
		
		if (path_split.Count() == 0) {
			return string.Empty;
		}
		
		return path_split[path_split.Count() - 1];
	}
	
	static array<string> EnumerateDirectories(string path)
	{
		array<string> child_directories = {};
		string file_name;
		FileAttr file_attributes;
		FindFileHandle handle = FindFile(path + SystemPath.SEPERATOR + "*", file_name, file_attributes, FindFileFlags.DIRECTORIES);
		if (!handle) {
			return child_directories;
		}
		
		if (file_attributes == FileAttr.DIRECTORY) {
			child_directories.Insert(file_name);
		}
		
		while (FindNextFile(handle, file_name, file_attributes)) {
			if (file_attributes == FileAttr.DIRECTORY) {
				child_directories.Insert(file_name);
			}
		}
		
		CloseFindFile(handle);
		return child_directories;
	}
	
	static FileHandle CreateFile(string file)
	{		
		// Creates needed directories
		string absolute_file_rebuild;
		array<string> absolute_file_split = {};
		file.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		file.Split(SystemPath.SEPERATOR, absolute_file_split);
		for (int i = 0; i < absolute_file_split.Count(); i++) {
			if (absolute_file_split[i].Contains(".")) {
				break;
			}
			
			absolute_file_rebuild += absolute_file_split[i];
													// drive specifier
			if (!FileExist(absolute_file_rebuild) && !absolute_file_split[i].Contains(":")) {
				Workbench.RunCmd("cmd /c mkdir " + absolute_file_rebuild);
			}
			
			absolute_file_rebuild += SystemPath.SEPERATOR_ALT;
		}
		
		// Create the file
		int result = Workbench.RunCmd(string.Format("cmd /c copy NUL \"%1\"", file), true);
		if (result != 0) {
			return null;
		}
				
		return OpenFile(file, FileMode.WRITE);
	}
	
	static int RunCommandPrompt(string cmd, bool wait = false)
	{
		return Workbench.RunCmd(string.Format("cmd /c %1", cmd), wait);
	}
	
	static int PromiseSymLink(string source, string target)
	{
		target.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);
		array<string> path_split = {};
		target.Split(SystemPath.SEPERATOR, path_split);
		string path_reconstruct;
		for (int i = 0; i < path_split.Count(); i++) {
			path_reconstruct += path_split[i] + SystemPath.SEPERATOR;
			if (!FileExist(path_reconstruct) && i < path_split.Count() - 1) {
				//path_reconstruct = path_reconstruct.Substring(0, path_reconstruct.Length() - 1);
				PrintFormat("Creating directory: %1", path_reconstruct);
				
				Print(MakeDirectory(path_reconstruct));
			}
		}
		
		if (!FileExist(target)) {
			return RunCommandPrompt(string.Format("mklink /j \"%2\" \"%1\"", source, target), true);
		}
		
		return 0;
	}
	
	static void KillTask(string task_name)
	{
		Workbench.RunCmd(string.Format("taskkill /F /IM %1 /T", task_name), true);
	}
	
	static void CleanLogFolder(string folder)
	{
		array<string> all = {};
		all.InsertAll(Directory.EnumerateFiles(folder, "*.RPT"));
		all.InsertAll(Directory.EnumerateFiles(folder, "*.log"));
		all.InsertAll(Directory.EnumerateFiles(folder, "*.mdmp"));
		foreach (string file: all) {
			array<string> datetime_split = {};
			file.Split("_", datetime_split);
			if (datetime_split.Count() <= 1) {
				DeleteFile(file);
				continue;
			} 
			
			string year_month_day = datetime_split[datetime_split.Count() - 2];
			string hour_minute_second = datetime_split[datetime_split.Count() - 1];
			array<string> ymd_split = {};
			year_month_day.Split("-", ymd_split);
			int year = ymd_split[0].ToInt();
			int day = ymd_split[2].ToInt();
			int month = ymd_split[1].ToInt();
			
			array<string> hms_split = {};
			hour_minute_second.Split("-", hms_split);
			int hour = hms_split[0].ToInt();
			int minute = hms_split[1].ToInt();
			int second = hms_split[2].ToInt();
			DateTime date_time = DateTime.Create(year, month, day, hour, minute, second);
			TimeSpan age = DateTime.Now(false) - date_time;
			//PrintFormat("%2 - age %1", age.Format(), file);
			
			if (age > TimeSpan.HOUR) {
				DeleteFile(file);
			}
		}
		
		all.Clear();
		all.InsertAll(Directory.EnumerateFiles(folder, "*.ADM"));
		foreach (string file2: all) {
			DeleteFile(file2);
		}
	}
	
	static void CopyFiles(string source, string destination)
	{	
		string filename;
		FileAttr fileattr;
		FindFileHandle hdnl = FindFile(source + SystemPath.SEPERATOR + "*", filename, fileattr, FindFileFlags.ALL);
		if (fileattr == FileAttr.DIRECTORY) {
			MakeDirectory(destination + SystemPath.SEPERATOR + filename);
			CopyFiles(source + SystemPath.SEPERATOR + filename, destination + SystemPath.SEPERATOR + filename);
		} else {
			CopyFile(source + SystemPath.SEPERATOR + filename, destination + SystemPath.SEPERATOR + filename);
		}
		
		while (FindNextFile(hdnl, filename, fileattr)) {
			if (fileattr == FileAttr.DIRECTORY) {
				MakeDirectory(destination + SystemPath.SEPERATOR + filename);
				CopyFiles(source + SystemPath.SEPERATOR + filename, destination + SystemPath.SEPERATOR + filename);
			} else {
				CopyFile(source + SystemPath.SEPERATOR + filename, destination + SystemPath.SEPERATOR + filename);
			}
		}
		
		CloseFindFile(hdnl);
	}
	
	static int GetScriptModuleFromFile(string file)
	{
		file.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);	
		file.ToLower();
		
		array<string> tokens = {};
		file.Split(SystemPath.SEPERATOR, tokens);
		foreach (string token: tokens) {
			int search_result = SCRIPT_MODULES.Find(token);
			if (search_result != -1) {
				return search_result;	
			}
		}
		
		return -1;
	}
	
	// 0 is 1_Core, 5 is workbench
	static string GetCurrentScriptModulePath(int module)
	{
		switch (module) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4: return GetDirectory(GetCurrentDirectory()) + "Scripts" + SystemPath.SEPERATOR + SCRIPT_MODULES[module];
			case 5: return GetCurrentDirectory();
		}
		
		return string.Empty;
	}
	
	static string StripScriptModuleFromPath(string file)
	{				
		file.Replace(SystemPath.SEPERATOR_ALT, SystemPath.SEPERATOR);	
		file.ToLower();
		
		string result;
		array<string> tokens = {};
		file.Split(SystemPath.SEPERATOR, tokens);
		for (int i = tokens.Count() - 1; i >= 0; i--) {
			if (SCRIPT_MODULES.Find(tokens[i]) != -1) {
				return result;
			}
			
			result = SystemPath.SEPERATOR + tokens[i] + result;
		}
		
		return result;
	}
	
 	static bool GetClassFromFileAndCursorPosition(string current_file, int current_line, out string class_name, out bool is_modded)
	{
		FileHandle current_file_handle = OpenFile(current_file, FileMode.READ);
		if (!current_file_handle) {
			return false;
		}
		
		array<string> file_contents = {};
		string data, buffer;
	    while (ReadFile(current_file_handle, buffer, 256) != 0) {
	        data += buffer;
		}
	
	    data.Split("\n", file_contents);
	    CloseFile(current_file_handle);
		
		// Bounding check
		current_line = Math.Min(current_line, file_contents.Count() - 1);
		string token, line;
		
		// First, search UP /\
		for (int i = current_line; i >= 0; i--) {			
			line = file_contents[i];
			if (!line.ParseStringEx(token)) {
				continue;
			}
			
			if (token == "modded") {
				line.ParseStringEx(token);
				is_modded = true;
			}
			
			if (token == "class" && line.ParseStringEx(class_name)) {
				return true;
			}
			
			is_modded = false;
		}
		
		// Second, search DOWN \/
		for (int j = current_line; j < file_contents.Count(); j++) {
			line = file_contents[j];
			if (!line.ParseStringEx(token)) {
				continue;
			}
			
			if (token == "modded") {
				line.ParseStringEx(token);
				is_modded = true;
			}
			
			if (token == "class" && line.ParseStringEx(class_name)) {
				return true;
			}
			
			is_modded = false;
		}
		
		return false;
	}
}