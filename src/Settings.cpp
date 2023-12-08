#include "Settings.h"
#include <SimpleIni.h>
#include <sstream>

Settings* Settings::GetSingleton()
{
	static Settings settings;
	return &settings;
}

void Settings::LoadSettings()
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(R"(.\Data\SKSE\Plugins\BladeAndBlunt.ini)");

	enableInjuries = ini.GetBoolValue("", "bEnableInjuries", true);
	SMOnlyEnableInjuries = ini.GetBoolValue("", "bEnableInjuriesOnlyWithSM", false);
	enableSneakStaminaCost = ini.GetBoolValue("", "bEnableSneakStaminaCost", true);
	enableLevelDifficulty = ini.GetBoolValue("", "bLevelBasedDifficulty", true);
	zeroAllWeapStagger = ini.GetBoolValue("", "bZeroAllWeaponStagger", true);
	armorScalingEnabled = ini.GetBoolValue("", "bArmorRatingScalingEnabled", true);
	replaceAttackTypeKeywords = ini.GetBoolValue("", "bReplaceNewAttackTypeKeywords", true);

	std::string attackingSpellFormID((ini.GetValue("", "IsAttackingSpellFormId", "")));
	std::string blockingSpellFormID((ini.GetValue("", "IsBlockingSpellFormId", "")));
	std::string sneakingSpellFormID((ini.GetValue("", "IsSneakingSpellFormId", "")));
	std::string bowStaminaSpellFormID((ini.GetValue("", "BowStaminaSpellFormId", "")));
	std::string xbowStaminaSpellFormID((ini.GetValue("", "XbowStaminaSpellFormId", "")));
	std::string IsCastingFormId((ini.GetValue("", "IsCastingSpellFormId", "IsCastingSpellFormId")));
	std::string bashPerkFormId((ini.GetValue("", "BashStaminaPerkFormId", "")));
	std::string blockPerkFormId((ini.GetValue("", "BlockStaminaPerkFormId", "")));
	std::string blockStaggerPerkFormId((ini.GetValue("", "BlockStaggerPerkFormId", "")));
	std::string dualWieldKeywordFormId((ini.GetValue("", "DualWieldReplacementKeyword", "")));

	std::string injurySpell1FormID((ini.GetValue("", "InjurySpell1FormID", "")));
	std::string injurySpell2FormID((ini.GetValue("", "InjurySpell2FormID", "")));
	std::string injurySpell3FormID((ini.GetValue("", "InjurySpell3FormID", "")));

	injury1AVPercent = static_cast<float>(ini.GetDoubleValue("", "fInjury1AVPercent", 0.10));
	injury2AVPercent = static_cast<float>(ini.GetDoubleValue("", "fInjury2AVPercent", 0.25));
	injury3AVPercent = static_cast<float>(ini.GetDoubleValue("", "fInjury3AVPercent", 0.50));

	std::string fileName(ini.GetValue("", "sModFileName", ""));

	if(!attackingSpellFormID.empty()){
		IsAttackingSpellFormId = ParseFormID(attackingSpellFormID);
	}

	if(!blockingSpellFormID.empty()){
		IsBlockingSpellFormId = ParseFormID(blockingSpellFormID);
	}

	if(!sneakingSpellFormID.empty()){
		IsSneakingSpellFormId = ParseFormID(sneakingSpellFormID);
	}

	if (!bowStaminaSpellFormID.empty()){
		BowDrainStaminaFormId = ParseFormID(bowStaminaSpellFormID);
	}

	if (!xbowStaminaSpellFormID.empty()) {
		XbowDrainStaminaFormId = ParseFormID(xbowStaminaSpellFormID);
	}

	if (!IsCastingFormId.empty()) {
		IsCastingSpellFormId = ParseFormID(IsCastingFormId);
	}

	if (!bashPerkFormId.empty()) {
		BashPerkFormId = ParseFormID(bashPerkFormId);
	}

	if (!blockPerkFormId.empty()) {
		BlockPerkFormId = ParseFormID(blockPerkFormId);
	}

	if (!blockStaggerPerkFormId.empty()) {
		BlockStaggerPerkFormId = ParseFormID(blockStaggerPerkFormId);
	}

	if (!injurySpell1FormID.empty()) {
		InjurySpell1FormId = ParseFormID(injurySpell1FormID);
	}
	if (!injurySpell2FormID.empty()) {
		InjurySpell2FormId = ParseFormID(injurySpell2FormID);
	}
	if (!injurySpell3FormID.empty()) {
		InjurySpell3FormId = ParseFormID(injurySpell3FormID);
	}
	if (!dualWieldKeywordFormId.empty()) {
		DualWieldReplaceFormId = ParseFormID(dualWieldKeywordFormId);
	}

	FileName = fileName;
}


RE::FormID Settings::ParseFormID(const std::string& str)
{
	RE::FormID result;
	std::istringstream ss{ str };
	ss >> std::hex >> result;
	return result;
}

void Settings::AdjustWeaponStaggerVals() 
{
	if (zeroAllWeapStagger) {
		logger::info("Adjusting weapon stagger values");
		int16_t totalWeaps = 0;

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		if (dataHandler) {
			for (const auto& foundWeap : dataHandler->GetFormArray<RE::TESObjectWEAP>()) {
				if (foundWeap && !foundWeap->weaponData.flags.any(RE::TESObjectWEAP::Data::Flag::kNonPlayable)) {
					foundWeap->weaponData.staggerValue = 0.0f;
					totalWeaps++;
				}
			}
		}

		logger::info(FMT_STRING("Stagger values adjusted: {} weapons"), totalWeaps);
	}
}

void Settings::ReplacePowerAttackKeywords()
{
	if (replaceAttackTypeKeywords) {

		logger::info("Swapping power attack types");
		std::unordered_map<std::string, RE::BGSKeyword*> swaps;

		//Hardcoded for now. This is all we need for b&b
		swaps["attackPowerStartH2HCombo"] = DualWieldReplaceKeyword;
		swaps["attackPowerStartDualWield"] = DualWieldReplaceKeyword;
		auto targetEvent = std::string("PowerAttackTypeStanding");

		auto dataHandler = RE::TESDataHandler::GetSingleton();
		auto races = dataHandler->GetFormArray<RE::TESRace>();

		for (auto raceCandidate : races) {
			auto& attackDataMap = raceCandidate->attackDataMap.get()->attackDataMap;

			for (auto& attackDataPtr : attackDataMap) {
				std::string eventName = std::string(attackDataPtr.first.c_str());

				auto eventIt = swaps.find(eventName);

				if (eventIt != swaps.end()) {
					auto attackType = attackDataPtr.second.get()->data.attackType;

					if (attackType) {
						auto attackTypeName = std::string(attackType->GetFormEditorID());
						auto raceName = std::string(raceCandidate->GetFormEditorID());
						if (attackTypeName == targetEvent) {
							attackDataPtr.second.get()->data.attackType = eventIt->second;
							auto replacedName = std::string(DualWieldReplaceKeyword->GetFormEditorID());
							logger::info("Updated race: " + raceName + " - event: " + eventName + " - " + attackTypeName + " -> " + replacedName);
						}
					}
				}
			}
		}

		logger::info("Power attack types swapped");
	}
}

void Settings::LoadForms()
{
	auto dataHandler = RE::TESDataHandler::GetSingleton();
	logger::info("Loading forms");
	if (IsBlockingSpellFormId)
		IsBlockingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsBlockingSpellFormId, FileName));

	if(IsAttackingSpellFormId)
		IsAttackingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsAttackingSpellFormId, FileName));

	if(IsSneakingSpellFormId)
		IsSneakingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsSneakingSpellFormId, FileName));

	if(BowDrainStaminaFormId)
		BowStaminaSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(BowDrainStaminaFormId, FileName));

	if (XbowDrainStaminaFormId)
		XbowStaminaSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(XbowDrainStaminaFormId, FileName));

	if (IsCastingSpellFormId)
		IsCastingSpell = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(IsCastingSpellFormId, FileName));

	if (BashPerkFormId)
		BashStaminaPerk = dataHandler->LookupForm(BashPerkFormId, "Update.esm")->As<RE::BGSPerk>();

	if (BlockPerkFormId)
		BlockStaminaPerk = dataHandler->LookupForm(BlockPerkFormId, "Update.esm")->As<RE::BGSPerk>();

	if (BlockStaggerPerkFormId)
		BlockStaggerPerk = dataHandler->LookupForm(BlockStaggerPerkFormId, "Update.esm")->As<RE::BGSPerk>();

	if (InjurySpell3FormId)
		InjurySpell3 = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(InjurySpell3FormId, FileName));

	if (InjurySpell1FormId)
		InjurySpell1 = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(InjurySpell1FormId, FileName));

	if (InjurySpell2FormId)
		InjurySpell2 = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(InjurySpell2FormId, FileName));

	if (InjurySpell3FormId)
		InjurySpell3 = skyrim_cast<RE::SpellItem*>(dataHandler->LookupForm(InjurySpell3FormId, FileName));

	if (DualWieldReplaceFormId)
		DualWieldReplaceKeyword = dataHandler->LookupForm(DualWieldReplaceFormId, "Update.esm")->As<RE::BGSKeyword>();

	//Hardcoded loads
	MAGParryControllerSpell = dataHandler->LookupForm(ParseFormID("0x817"), FileName)->As<RE::SpellItem>();
	MAGParryStaggerSpell = dataHandler->LookupForm(ParseFormID("0x816"), FileName)->As<RE::SpellItem>();
	MAGBlockStaggerSpell = dataHandler->LookupForm(ParseFormID("0x855"), FileName)->As<RE::SpellItem>();
	MAGBlockStaggerSpell2 = dataHandler->LookupForm(ParseFormID("0x858"), FileName)->As<RE::SpellItem>();
	MAGCrossbowStaminaDrainSpell = dataHandler->LookupForm(ParseFormID("0x873"), FileName)->As<RE::SpellItem>();
	InjuryChance25Health = dataHandler->LookupForm(ParseFormID("0xADA180"), "Update.esm")->As<RE::TESGlobal>();
	InjuryChance50Health = dataHandler->LookupForm(ParseFormID("0xADA181"), "Update.esm")->As<RE::TESGlobal>();
	MAG_InjuryCooldown1 = dataHandler->LookupForm(ParseFormID("0x84F"), FileName)->As<RE::EffectSetting>();
	MAG_InjuryCooldown2 = dataHandler->LookupForm(ParseFormID("0x850"), FileName)->As<RE::EffectSetting>();
	MAG_ParryWindowEffect = dataHandler->LookupForm(ParseFormID("0x815"), FileName)->As<RE::EffectSetting>();
	MAG_InjuriesSMOnly = dataHandler->LookupForm(ParseFormID("0x88E"), FileName)->As<RE::TESGlobal>();

	MAG_levelBasedDifficulty = dataHandler->LookupForm(ParseFormID("0x854"), FileName)->As<RE::TESGlobal>();
	MAG_PowerAttackReplacement = dataHandler->LookupForm(ParseFormID("0xA9B"), FileName)->As<RE::TESGlobal>();
	MAG_InjuryAndRest = dataHandler->LookupForm(ParseFormID("0x83F"), FileName)->As<RE::TESGlobal>();
	HealthPenaltyUIGlobal = dataHandler->LookupForm(RE::FormID(0x2EDE), "Update.esm")->As<RE::TESGlobal>();

	auto smGlobal = dataHandler->LookupForm(RE::FormID(0x826), "ccqdrsse001-survivalmode.esl");

	if (smGlobal) {
		Survival_ModeEnabled = smGlobal->As<RE::TESGlobal>();
	}

	if (dataHandler->LookupModByName("Starfrost.esp"))
	{
		starfrostInstalled = true;
	}	

	SetGlobalsAndGameSettings();

	auto isPowerAttacking = new RE::TESConditionItem;
	isPowerAttacking->data.comparisonValue.f = 1.0f;
	isPowerAttacking->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPowerAttacking;

	auto isPowerAttackingCond = new RE::TESCondition;
	isPowerAttackingCond->head = isPowerAttacking;
	IsPowerAttacking = isPowerAttackingCond;

	logger::info("Forms loaded");

}

void Settings::SetGlobalsAndGameSettings() 
{
	MAG_levelBasedDifficulty->value = enableLevelDifficulty;
	MAG_InjuryAndRest->value = enableInjuries;
	MAG_InjuriesSMOnly->value = SMOnlyEnableInjuries;
	MAG_PowerAttackReplacement->value = replaceAttackTypeKeywords;

	//Set fMaxArmorRating game setting
	auto gameSettings = RE::GameSettingCollection::GetSingleton();
	auto maxRatingSetting = gameSettings->GetSetting("fMaxArmorRating");

	if (armorScalingEnabled) {
		logger::info("Setting max armor rating to 90");
		maxRatingSetting->data.f = 90.0f;
	} else {
		logger::info("Setting max armor rating to 75");
		maxRatingSetting->data.f = 75.0f;
	}
}

