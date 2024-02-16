#pragma once

class Settings
{
public:
	static Settings* GetSingleton();

	void LoadSettings();
	void LoadForms();
	void AdjustWeaponStaggerVals();
	void ReplacePowerAttackKeywords();
	void SetGlobalsAndGameSettings();

	RE::FormID IsAttackingSpellFormId;
	RE::FormID IsBlockingSpellFormId;
	RE::FormID IsSneakingSpellFormId;
	RE::FormID IsSprintingSpellFormId;
	RE::FormID MountSprintingSpellFormId;
	RE::FormID IsCastingSpellFormId;
	RE::FormID BowDrainStaminaFormId;
	RE::FormID XbowDrainStaminaFormId;
	RE::FormID BashPerkFormId;
	RE::FormID BlockPerkFormId;
	RE::FormID BlockStaggerPerkFormId;
	RE::FormID InjurySpell1FormId;
	RE::FormID InjurySpell2FormId;
	RE::FormID InjurySpell3FormId;
	RE::FormID DualWieldReplaceFormId;

	RE::SpellItem* IsAttackingSpell;
	RE::SpellItem* IsBlockingSpell;
	RE::SpellItem* IsSneakingSpell;
	RE::SpellItem* IsSprintingSpell;
	RE::SpellItem* MountSprintingSpell;
	RE::SpellItem* BowStaminaSpell;
	RE::SpellItem* XbowStaminaSpell;
	RE::SpellItem* IsCastingSpell;
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
	RE::TESGlobal* Survival_ModeEnabled;
	RE::TESGlobal* MAG_InjuriesSMOnly;

	RE::TESGlobal* MAG_levelBasedDifficulty;
	RE::TESGlobal* MAG_InjuryAndRest;
	RE::TESGlobal* HealthPenaltyUIGlobal;
	RE::TESGlobal* MAG_PowerAttackReplacement;

	RE::EffectSetting* MAG_ParryWindowEffect;
	RE::EffectSetting* MAG_InjuryCooldown1;
	RE::EffectSetting* MAG_InjuryCooldown2;

	RE::BGSKeyword* DualWieldReplaceKeyword;

	RE::TESCondition* IsPowerAttacking;

	bool enableInjuries;
	bool SMOnlyEnableInjuries;
	bool enableSneakStaminaCost;
	bool enableLevelDifficulty;
	bool replaceAttackTypeKeywords;
	bool zeroAllWeapStagger;
	bool armorScalingEnabled;
	bool starfrostInstalled;
	
	float injury1AVPercent;
	float injury2AVPercent;
	float injury3AVPercent;
	float injuryUpdateFrequency = 0.5f;

	bool IsBlockingWeaponSpellCasted = false;

	int maxFrameCheck = 6;

	static RE::FormID ParseFormID(const std::string& str);

	std::string FileName;
	
};
