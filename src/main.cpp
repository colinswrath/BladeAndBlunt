#include "Hooks.h"
#include "Utility/Cache.h"
#include "Events.h"
#include "Serialization.h"

void InitListener(SKSE::MessagingInterface::Message* a_msg)
{
	auto settings = Settings::GetSingleton();
	switch (a_msg->type)
	{
	case SKSE::MessagingInterface::kNewGame:
	case SKSE::MessagingInterface::kPostLoadGame:
		Settings::GetSingleton()->SetGlobalsAndGameSettings();
		break;
	case SKSE::MessagingInterface::kPostLoad:
		if (!Hooks::InstallBashMultHook()) {
			logger::error("Bash hook installation failed.");
		} else {
			logger::info("Bash hook installed");
		}

        if (!Hooks::InstallTrueHUDHook()) {
            logger::warn("Failed to obtain TrueHUD API");
        } else {
            logger::info("Obtained TrueHUD API");
        }

		break;
	case SKSE::MessagingInterface::kDataLoaded:
		if (settings) {
			settings->LoadForms();
			settings->AdjustWeaponStaggerVals();
			settings->ReplacePowerAttackKeywords();
            settings->ToggleSMIFromBnB();

            if (!Hooks::InstallStaggerHUDHook()) {
                logger::warn("Stagger HUD installation failed.");
            }
            else {
                logger::info("Stagger HUD installed");
            }
		}
        OnHitEventHandler::Register();

		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	SKSE::Init(skse);
    logger::init();
    const auto plugin{ SKSE::PluginDeclaration::GetSingleton() };
    const auto version{ plugin->GetVersion() };

    logger::info("{} {} loading...", plugin->GetName(), version);

	SKSE::AllocTrampoline(320);
	Cache::CacheAddLibAddresses();
	Settings::GetSingleton()->LoadSettings();
	if (!Hooks::InstallHooks())
	{
		logger::error("Hook installation failed.");
		return false;
	}

	auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener(InitListener))
	{
		return false;
	}

	if (auto serialization = SKSE::GetSerializationInterface()) {
		serialization->SetUniqueID(Serialization::ID);
		serialization->SetLoadCallback(&Serialization::LoadCallback);
        serialization->SetSaveCallback(&Serialization::SaveCallback);
        serialization->SetRevertCallback(&Serialization::RevertCallback);
	}

	logger::info("Blade and Blunt loaded.");
	spdlog::default_logger()->flush();
	return true;
}
