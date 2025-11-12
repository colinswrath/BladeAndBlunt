#include "Managers/StaggerHUDManager.h"
#include "Hooks.h"
#include "Settings.h"

bool StaggerHUDManager::InstallTrueHUDHook()
{
    StaggerHUDSettings::HUDInstance = StaggerHUDBase::GetSingleton();
    StaggerHUDSettings::HUDInstance->trueHUDInterface = reinterpret_cast<TRUEHUD_API::IVTrueHUD3*>(TRUEHUD_API::RequestPluginAPI(TRUEHUD_API::InterfaceVersion::V3));
    return true;
}

float StaggerHUDBase::GetMaxStaggerCooldown(RE::Actor* a_actor)
{
    return 5.0f;
}

float StaggerHUDBase::GetCurrentStaggerCooldown(RE::Actor* a_actor)
{
    if (a_actor) {
        auto settings = Settings::GetSingleton();
        if (auto a_avOwner = a_actor->AsActorValueOwner()) {
            auto staggerAV_val = StaggerHUDManager::GetActorValueIDFromName(settings->staggerAV_str.data());
            auto staggerAV = a_avOwner->GetActorValue(staggerAV_val);
            return staggerAV;
        }
    }
    return 0.0f;
}

bool StaggerHUDManager::InstallStaggerHUDHook()
{
    auto trueHUD = GetModuleHandle(L"TrueHUD");

    if (trueHUD != nullptr) {
        auto& trampoline = SKSE::GetTrampoline();
        _Update = trampoline.write_call<5>(Hooks::actorUpdate.address(), Update);

        RequestStaggerBarControl();
        SetBarStyle();
        return true;
    }
    return false;
}

void StaggerHUDManager::Update(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell)
{
    if (a_actor && !a_actor->IsPlayer()) {
        if (auto a_actorHandle = a_actor->GetHandle()) {
            if (!StaggerHUDBase::trueHUDInterface->HasInfoBar(a_actorHandle, false)) {
                StaggerHUDBase::trueHUDInterface->OverrideSpecialBarColor(a_actorHandle, TRUEHUD_API::BarColorType::BarColor, StaggerHUDSettings::staggerBarColor);
                StaggerHUDBase::trueHUDInterface->OverrideSpecialBarColor(a_actorHandle, TRUEHUD_API::BarColorType::FlashColor, StaggerHUDSettings::staggerFlashColor);
            }
        }
    }

    return _Update(a_actor, a_zPos, a_cell);
}

RE::ActorValue StaggerHUDManager::GetActorValueIDFromName(const char* av_name)
{
    using func_t = decltype(&GetActorValueIDFromName);
    REL::Relocation<func_t> func{ REL::RelocationID(26570, 27203) };
    return func(av_name);
}

bool StaggerHUDManager::RequestStaggerBarControl()
{
    if (!StaggerHUDSettings::HUDInstance->trueHUDInterface) {
        logger::info("TrueHUD API not acquired. Cannot install Stagger HUD");
        return false;
    }

    auto bar_control = StaggerHUDSettings::HUDInstance->trueHUDInterface->RequestSpecialResourceBarsControl(SKSE::GetPluginHandle());
    switch (bar_control) {
    case TRUEHUD_API::APIResult::OK:
        StaggerHUDSettings::HUDInstance->trueHUDInterface->RegisterSpecialResourceFunctions(SKSE::GetPluginHandle(), StaggerHUDBase::GetCurrentStaggerCooldown,
                                                                                            StaggerHUDBase::GetMaxStaggerCooldown, true, true);
        return true;
    case TRUEHUD_API::APIResult::AlreadyGiven:
        return true;
    case TRUEHUD_API::APIResult::AlreadyTaken:
        logger::info("TrueHUD API already taken by another plugin");
        return false;
    }
    return false;
}

void StaggerHUDManager::SetBarStyle()
{
    auto settings = Settings::GetSingleton();

    std::istringstream str_color{ settings->staggerBarColor };
    str_color >> std::hex >> StaggerHUDSettings::staggerBarColor;
    str_color.clear();
    str_color.str(settings->staggerFlashColor);
    str_color >> std::hex >> StaggerHUDSettings::staggerFlashColor;
}
