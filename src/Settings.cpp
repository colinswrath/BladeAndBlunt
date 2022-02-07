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

	std::string attackingSpellFormID((ini.GetValue("", "IsAttackingSpellFormId", "")));
	std::string blockingSpellFormID((ini.GetValue("", "IsBlockingSpellFormId", "")));
	std::string sneakingSpellFormID((ini.GetValue("", "IsSneakingSpellFormId", "")));
	std::string bowStaminaSpellFormID((ini.GetValue("","BowStaminaSpellFormId", "")));

	std::string fileName(ini.GetValue("", "sModFileName", ""));

	if(!attackingSpellFormID.empty())
	{
		IsAttackingSpellFormId = ParseFormID(attackingSpellFormID);
	}

	if(!blockingSpellFormID.empty())
	{
		IsBlockingSpellFormId = ParseFormID(blockingSpellFormID);
	}

	if(!sneakingSpellFormID.empty())
	{
		IsSneakingSpellFormId = ParseFormID(sneakingSpellFormID);
	}

	if (!bowStaminaSpellFormID.empty())
	{
		BowDrainStaminaFormId = ParseFormID(bowStaminaSpellFormID);
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


void Settings::LoadForms()
{
	logger::info("Loading forms");
	if (IsBlockingSpellFormId)
		IsBlockingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsBlockingSpellFormId, FileName));

	if(IsAttackingSpellFormId)
		IsAttackingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsAttackingSpellFormId, FileName));
	
	if(IsSneakingSpellFormId)
		IsSneakingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsSneakingSpellFormId, FileName));

	if(BowDrainStaminaFormId)
		BowStaminaSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(BowDrainStaminaFormId, FileName));
	logger::info("Forms loaded");

}
