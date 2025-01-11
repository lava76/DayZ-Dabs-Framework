modded class DayZGame
{
    protected ref array<PlayerIdentity> m_GameIdentities = {};

	protected ref map<typename, ref MissionSetting> m_MissionSettings = new map<typename, ref MissionSetting>();

#ifndef SERVER
    override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, target, rpc_type, ctx);

        switch (rpc_type) {
            case MissionSetting.RPC_SYNC: {
                string mission_setting_type;
                ctx.Read(mission_setting_type);

                typename mission_setting_typename = mission_setting_type.ToType();
                if (!mission_setting_typename) {
                    ErrorEx(string.Format("INVALID MissionSetting type: %1", mission_setting_type));
                    break;
                }

                MissionSetting mission_setting = MissionSetting.Cast(mission_setting_typename.Spawn());
                if (!mission_setting.Read(ctx, 0)) {
                    ErrorEx(string.Format("INVALID MissionSetting data: %1", mission_setting_type));
                    break;
                }

                m_MissionSettings[mission_setting_typename] = mission_setting;
                break;
            }
        }
    }
#endif

	override void OnEvent(EventType eventTypeId, Param params)
	{
        super.OnEvent(eventTypeId, params);

        switch (eventTypeId) {
            case ClientPrepareEventTypeID: {
                ClientPrepareEventParams client_prepare_event_params = ClientPrepareEventParams.Cast(params);
#ifdef DIAG_DEVELOPER
                PrintFormat("EOnClientPrepare: (identity=%1, use_db=%2, position=%3, yaw=%4, preload_timeout=%5)", client_prepare_event_params.param1, client_prepare_event_params.param2, client_prepare_event_params.param3, client_prepare_event_params.param4, client_prepare_event_params.param5);
#endif
                PlayerIdentity identity = client_prepare_event_params.param1;
                if (identity && m_GameIdentities.Find(identity) == -1) {
                    m_GameIdentities.Insert(identity);
                    OnIdentityCreated(identity);
                }

                break;
            }
        }
    }

    protected void OnIdentityCreated(notnull PlayerIdentity identity)
    {
		// Update identity with mission settings
        foreach (MissionSetting mission_setting: m_MissionSettings) {
            if (mission_setting) {
                mission_setting.Sync(identity);
            }
        }
    }

    protected void OnMissionPathSet(string path)
    {
        if (!GetGame().IsMultiplayer() || GetGame().IsDedicatedServer()) {
            foreach (typename mission_setting_type, string mission_setting_file: RegisterMissionSetting<Class>.s_RegisteredInstances) {
				string mission_setting_file_verified = mission_setting_file;
				if (!SystemPath.IsPathRooted(mission_setting_file_verified)) {
					if (GetGame().IsDedicatedServer()) {
						mission_setting_file_verified = SystemPath.Mission(mission_setting_file_verified);
					} else {
						mission_setting_file_verified = SystemPath.Profile(mission_setting_file_verified);
					}
				}
				
                bool file_exists = File.Exists(mission_setting_file_verified);
                JsonSerializer json_file_serializer = new JsonSerializer();
                MissionSetting mission_setting = MissionSetting.Cast(mission_setting_type.Spawn());
                if (!mission_setting) {
                    ErrorEx(string.Format("failed to create mission setting type: %1, file: %2", mission_setting_type, mission_setting_file_verified));
                    break;
                }
                
                if (!file_exists) {
                    bool save_success = mission_setting.Save();
                    if (!save_success) {
                        ErrorEx(string.Format("failed to save mission settings: %1", mission_setting_file_verified));
                    }
                } else {
                    string file_text = File.ReadAllText(mission_setting_file_verified);
                    if (!file_text) {
                        ErrorEx(string.Format("empty json file found"));
                        break;
                    }

                    string json_error;
                    if (!json_file_serializer.ReadFromString(mission_setting, file_text, json_error)) {
                        PrintFormat("json error, file: %1, error: %2", mission_setting_file_verified, json_error);
                        break;
                    }
                }

                m_MissionSettings[mission_setting_type] = mission_setting;
            }
        }		
    }

	override void SetMissionPath(string path)
	{
		super.SetMissionPath(path);

        GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnMissionPathSet, 0, 0, path);
	}

	MissionSetting GetMissionSetting(typename mission_settings)
	{
		return m_MissionSettings[mission_settings];
	}
}