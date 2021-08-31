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
	std::string bowStaminaFormID((ini.GetValue("","BowDrainStaminaFormId", "")));

	std::string fileName(ini.GetValue("", "sModFileName", ""));

	if(!attackingSpellFormID.empty())
	{
		IsAttackingSpellFormId = RE::FormID(ParseUInt32(attackingSpellFormID));
	}

	if(!blockingSpellFormID.empty())
	{
		IsBlockingSpellFormId = RE::FormID(ParseUInt32(blockingSpellFormID));
	}

	if(!sneakingSpellFormID.empty())
	{
		IsSneakingSpellFormId = RE::FormID(ParseUInt32(sneakingSpellFormID));
	}

	if (!bowStaminaFormID.empty())
	{
		BowDrainStaminaFormId = RE::FormID(ParseUInt32(bowStaminaFormID));
	}

	FileName = fileName;
}


std::uint32_t Settings::ParseUInt32(const std::string& str)
{
	std::uint32_t result;
	std::istringstream ss{ str };
	ss >> result;
	return result;
}


void Settings::LoadForms()
{
	if (IsBlockingSpellFormId)
		IsBlockingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsBlockingSpellFormId, FileName));

	if(IsAttackingSpellFormId)
		IsAttackingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsAttackingSpellFormId, FileName));
	
	if(IsSneakingSpellFormId)
		IsSneakingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsSneakingSpellFormId, FileName));

	if(BowDrainStaminaFormId)
		BowDrainStaminaSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(BowDrainStaminaFormId, FileName));

}
