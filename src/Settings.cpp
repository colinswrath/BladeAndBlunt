#include "Settings.h"
#include <SimpleIni.h>

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
	std::string sneakingSpellFormID((ini.GetValue("","IsSneakingSpellFormId", "")));
	std::string fileName(ini.GetValue("", "sModFileName", ""));

	if(!attackingSpellFormID.empty())
	{
		IsAttackingSpellFormId = RE::FormID(std::stoul(attackingSpellFormID, NULL, 16));
	}

	if(!blockingSpellFormID.empty())
	{
		IsBlockingSpellFormId = RE::FormID(std::stoul(blockingSpellFormID,NULL,16));
	}

	if(!sneakingSpellFormID.empty())
	{
		IsSneakingSpellFormId = RE::FormID(std::stoul(sneakingSpellFormID, NULL, 16));
	}

	FileName = fileName;
}



void Settings::LoadForms()
{
	if (IsBlockingSpellFormId)
	{

		IsBlockingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsBlockingSpellFormId, FileName));
		if (IsBlockingSpell)
		{
			logger::info("Spell found");
		}
	}
	if(IsAttackingSpellFormId)
		IsAttackingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsAttackingSpellFormId, FileName));
	
	if(IsSneakingSpellFormId)
		IsSneakingSpell = skyrim_cast<RE::SpellItem*>(RE::TESDataHandler::GetSingleton()->LookupForm(IsSneakingSpellFormId, FileName));

}
