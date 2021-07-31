class LiquidManager
{
	protected ref TTypenameArray m_LiquidTypes = {};
	protected ref map<typename, ref LiquidBase> m_Liquids = new map<typename, ref LiquidBase>();
	
	void LiquidManager()
	{
		RegisterLiquids(m_LiquidTypes);
		
		foreach (typename liquid: m_LiquidTypes) {
			m_Liquids[liquid] = LiquidBase.Cast(liquid.Spawn());
		}
	}
	
	void ~LiquidManager()
	{
		delete m_LiquidTypes;
		delete m_Liquids;
	}
	
	LiquidBase Get(typename liquid)
	{
		return m_Liquids[liquid];
	}
	
	void Set(typename liquid, LiquidBase liquid_base)
	{
		m_Liquids[liquid] = liquid_base;
	}
	
	TTypenameArray GetLiquidTypes()
	{
		return m_Liquids;
	}
	
	void RegisterLiquids(out TTypenameArray liquid_types)
	{
		liquid_types.Insert(Gasoline);
		liquid_types.Insert(Water);
	}
}

class LiquidBase
{
	private void LiquidBase();
	private void ~LiquidBase();
	
	float Combustability()
	{
		return 0;
	}
	
	float Toxicity()
	{
		return 0;
	}
	
	float Extinguishability()
	{
		return 0;
	}
	
	// Induce effects
	void OnConsume();
}

class Gasoline: LiquidBase
{
	override float Combustability()
	{
		return 0.75;
	}
	
	override float Toxicity()
	{
		return 0.85;
	}
}

class Water: LiquidBase
{
	override float Extinguishability()
	{
		return 1.0;
	}
}

class DummyTank
{
	ref map<typename, float> m_FluidAmounts = new map<typename, float>();
	protected float m_Level;
	
	bool Fill(typename liquid, float amount)
	{
		if (m_Level >= 1.0) {
			return false;
		}
		
		m_Level += amount;
		m_FluidAmounts[liquid] = m_FluidAmounts[liquid] + amount;
		return true;
	}
}