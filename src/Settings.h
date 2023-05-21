class Settings
{
public:
	static Settings* GetSingleton();

	void LoadSettings();
	void LoadForms();
	void AdjustWeaponStaggerVals();

	RE::FormID IsAttackingSpellFormId;
	RE::FormID IsBlockingSpellFormId;
	RE::FormID IsSneakingSpellFormId;
	RE::FormID BowDrainStaminaFormId;
	RE::FormID XbowDrainStaminaFormId;
	RE::FormID BashPerkFormId;
	RE::FormID BlockPerkFormId;
	RE::FormID BlockStaggerPerkFormId;
	RE::FormID InjurySpell1FormId;
	RE::FormID InjurySpell2FormId;
	RE::FormID InjurySpell3FormId;

	RE::SpellItem* IsAttackingSpell;
	RE::SpellItem* IsBlockingSpell;
	RE::SpellItem* IsSneakingSpell;
	RE::SpellItem* BowStaminaSpell;
	RE::SpellItem* XbowStaminaSpell;
	RE::SpellItem* MAGParryStaggerSpell;
	RE::SpellItem* MAGBlockStaggerSpell;
	RE::SpellItem* MAGBlockStaggerSpell2;
	RE::SpellItem* MAGParryControllerSpell;
	RE::SpellItem* MAGCrossbowStaminaDrainSpell;

	RE::SpellItem* InjurySpell1;
	RE::SpellItem* InjurySpell2;
	RE::SpellItem* InjurySpell3;
	
	RE::BGSPerk* BashStaminaPerk;
	RE::BGSPerk* BlockStaminaPerk;
	RE::BGSPerk* BlockStaggerPerk;

	RE::TESGlobal* InjuryChance50Health;
	RE::TESGlobal* InjuryChance25Health;

	RE::TESGlobal* MAG_levelBasedDifficulty;

	RE::EffectSetting* MAG_ParryWindowEffect;
	RE::EffectSetting* MAG_InjuryCooldown1;
	RE::EffectSetting* MAG_InjuryCooldown2;

	RE::TESCondition* IsPowerAttacking;

	bool enableInjuries;
	bool enableSneakStaminaCost;
	bool enableLevelDifficulty;
	bool zeroAllWeapStamina;
	bool armorScalingEnabled;

	bool IsBlockingWeaponSpellCasted = false;

	static RE::FormID ParseFormID(const std::string& str);

	std::string FileName;
	
};
