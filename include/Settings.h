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

	RE::TESGlobal* InjuryChance90Health;
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

	static RE::TESFile* LookupLoadedModByName(std::string_view a_modName)
    {
        for (auto& file : RE::TESDataHandler::GetSingleton()->compiledFileCollection.files) {
            if (a_modName.size() == strlen(file->fileName) && _strnicmp(file->fileName, a_modName.data(), a_modName.size()) == 0) {
                return file;
            }
        }
        return nullptr;
    }

    static const RE::TESFile* LookupLoadedLightModByName(std::string_view a_modName)
    {
        for (auto& smallFile : RE::TESDataHandler::GetSingleton()->compiledFileCollection.smallFiles) {
            if (a_modName.size() == strlen(smallFile->fileName) && _strnicmp(smallFile->fileName, a_modName.data(), a_modName.size()) == 0) {
                return smallFile;
            }
        }
        return nullptr;
    }

    template<typename T>
    T* LoadFormPointerFromIni(std::string formId, std::string filename)
    {
        if (!formId.empty()) {
            auto objectFormId = ParseFormID(formId);
            auto form         = RE::TESDataHandler::GetSingleton()->LookupForm(objectFormId, filename)->As<T>();
            return form;
        }
        else {
            return nullptr;
        }
        
    }
};
