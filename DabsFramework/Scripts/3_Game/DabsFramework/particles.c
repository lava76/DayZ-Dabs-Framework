modded class ParticleList 
{
	static const int DF_SNOW = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_snow");
	static const int DF_SNOWFOG_THIN = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_snow_fog_thin");
	static const int DF_SNOWFOG_MEDIUM = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_snow_fog_medium");
	static const int DF_FIREWORK_RED = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_red");
	static const int DF_FIREWORK_BLUE = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_blue");
	static const int DF_FIREWORK_CYAN = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_cyan");
	static const int DF_FIREWORK_GREEN = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_green");
	static const int DF_FIREWORK_ORANGE = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_orange");
	static const int DF_FIREWORK_PURPLE = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_purple");
	static const int DF_FIREWORK_SILVER = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_silver");
	static const int DF_FIREWORK_WHITE = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_white");
	static const int DF_FIREWORK_YELLOW = RegisterParticle("DabsFramework/GUI/graphics/particles/", "df_firework_yellow");
}

class DF_Snow : EffectParticle 
{
	void DF_Snow()
	{
		SetParticleID(ParticleList.DF_SNOW);
	}
}

class DF_SnowFog_Thin : EffectParticle
{
	void DF_SnowFog_Thin()
	{
		SetParticleID(ParticleList.DF_SNOWFOG_THIN);
	}
}

class DF_SnowFog_Medium : EffectParticle
{
	void DF_SnowFog_Medium()
	{
		SetParticleID(ParticleList.DF_SNOWFOG_MEDIUM);
	}
}

class DF_FireWorkRed : EffectParticle
{
	void DF_FireWorkRed()
	{
		SetParticleID(ParticleList.DF_FIREWORK_RED);
	}
}

class DF_FireWorkBlue : EffectParticle
{
	void DF_FireWorkBlue()
	{
		SetParticleID(ParticleList.DF_FIREWORK_BLUE);
	}
}

class FireWorkCyan : EffectParticle
{
	void DF_FireWorkCyan()
	{
		SetParticleID(ParticleList.DF_FIREWORK_CYAN);
	}
}

class DF_FireWorkGreen : EffectParticle
{
	void DF_FireWorkGreen()
	{
		SetParticleID(ParticleList.DF_FIREWORK_GREEN);
	}
}

class DF_FireWorkOrange : EffectParticle
{
	void DF_FireWorkOrange()
	{
		SetParticleID(ParticleList.DF_FIREWORK_ORANGE);
	}
}

class DF_FireWorkPurple : EffectParticle
{
	void DF_FireWorkPurple()
	{
		SetParticleID(ParticleList.DF_FIREWORK_PURPLE);
	}
}

class DF_FireWorkSilver : EffectParticle
{
	void DF_FireWorkSilver()
	{
		SetParticleID(ParticleList.DF_FIREWORK_SILVER);
	}
}

class DF_FireWorkWhite : EffectParticle
{
	void DF_FireWorkWhite()
	{
		SetParticleID(ParticleList.DF_FIREWORK_WHITE);
	}
}

class DF_FireWorkYellow : EffectParticle
{
	void DF_FireWorkYellow()
	{
		SetParticleID(ParticleList.DF_FIREWORK_YELLOW);
	}
}