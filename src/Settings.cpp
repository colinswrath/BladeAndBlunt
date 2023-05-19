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
	enableSneakStaminaCost = ini.GetBoolValue("", "bEnableSneakStaminaCost", true);
	enableLevelDifficulty = ini.GetBoolValue("", "bLevelBasedDifficulty", true);
	zeroAllWeapStamina = ini.GetBoolValue("", "bZeroAllWeaponStamina", true);

	std::string attackingSpellFormID((ini.GetValue("", "IsAttackingSpellFormId", "")));
	std::string blockingSpellFormID((ini.GetValue("", "IsBlockingSpellFormId", "")));
	std::string sneakingSpellFormID((ini.GetValue("", "IsSneakingSpellFormId", "")));
	std::string bowStaminaSpellFormID((ini.GetValue("", "BowStaminaSpellFormId", "")));
	std::string bashPerkFormId((ini.GetValue("", "BashStaminaPerkFormId", "")));
	std::string blockPerkFormId((ini.GetValue("", "BlockStaminaPerkFormId", "")));
	std::string blockStaggerPerkFormId((ini.GetValue("", "BlockStaggerPerkFormId", "")));
	std::string injurySpell1FormID((ini.GetValue("", "InjurySpell1FormID", "")));
	std::string injurySpell2FormID((ini.GetValue("", "InjurySpell2FormID", "")));
	std::string injurySpell3FormID((ini.GetValue("", "InjurySpell3FormID", "")));

	

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
	if (zeroAllWeapStamina) {
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

	MAGParryControllerSpell = dataHandler->LookupForm(ParseFormID("0x817"), FileName)->As<RE::SpellItem>();
	MAGParryStaggerSpell = dataHandler->LookupForm(ParseFormID("0x816"), FileName)->As<RE::SpellItem>();
	
	MAGBlockStaggerSpell = dataHandler->LookupForm(ParseFormID("0x855"), FileName)->As<RE::SpellItem>();
	MAGBlockStaggerSpell2 = dataHandler->LookupForm(ParseFormID("0x858"), FileName)->As<RE::SpellItem>();
	MAGCrossbowStaminaDrainSpell = dataHandler->LookupForm(ParseFormID("0x873"), FileName)->As<RE::SpellItem>();

	InjuryChance25Health = dataHandler->LookupForm(ParseFormID("0x852"), FileName)->As<RE::TESGlobal>();
	InjuryChance50Health = dataHandler->LookupForm(ParseFormID("0x853"), FileName)->As<RE::TESGlobal>();

	MAG_InjuryCooldown1 = dataHandler->LookupForm(ParseFormID("0x84F"), FileName)->As<RE::EffectSetting>();
	MAG_InjuryCooldown2 = dataHandler->LookupForm(ParseFormID("0x850"), FileName)->As<RE::EffectSetting>();

	MAG_ParryWindowEffect = dataHandler->LookupForm(ParseFormID("0x815"), FileName)->As<RE::EffectSetting>();

	MAG_levelBasedDifficulty = dataHandler->LookupForm(ParseFormID("0x854"), FileName)->As<RE::TESGlobal>();
	MAG_levelBasedDifficulty->value = enableLevelDifficulty;

	auto isPowerAttacking = new RE::TESConditionItem;
	isPowerAttacking->data.comparisonValue.f = 1.0f;
	isPowerAttacking->data.functionData.function = RE::FUNCTION_DATA::FunctionID::kIsPowerAttacking;

	auto isPowerAttackingCond = new RE::TESCondition;
	isPowerAttackingCond->head = isPowerAttacking;
	IsPowerAttacking = isPowerAttackingCond;

	logger::info("Forms loaded");

}
