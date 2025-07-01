#include "patches/StaggerHUD.h"
#include "Hooks.h"
#include "Settings.h"

namespace StaggerHUD
{

    bool InstallTrueHUDHook()
    {
        StaggerAVHUD                   = StaggerHUD::GetSingleton();
        StaggerAVHUD->trueHUDInterface = reinterpret_cast<TRUEHUD_API::IVTrueHUD3*>(TRUEHUD_API::RequestPluginAPI(TRUEHUD_API::InterfaceVersion::V3));
        if (StaggerAVHUD->trueHUDInterface) {
            return true;
        }
        return false;
    }

    float StaggerHUD::GetMaxStagger(RE::Actor* a_actor)
    {
        return 5.0f;
    }

    float StaggerHUD::GetCurrentStaggerCooldown(RE::Actor* a_actor)
    {
        if (a_actor) {
            auto settings = Settings::GetSingleton();
            if (auto a_avOwner = a_actor->AsActorValueOwner()) {
                if (auto a_actorHandle = a_actor->GetHandle()) {
                    auto         staggerAV_val = Hooks::GetActorValueIDFromName(settings->staggerAV_str.data());
                    auto         staggerAV     = a_avOwner->GetActorValue(staggerAV_val);
                    unsigned int staggerBarColor;
                    unsigned int staggerFlashColor;

                    std::istringstream sbc { settings->staggerBarColor };
                    sbc >> std::hex >> staggerBarColor;
                    std::istringstream sfc{ settings->staggerFlashColor };
                    sfc >> std::hex >> staggerFlashColor;

                    StaggerAVHUD->trueHUDInterface->OverrideSpecialBarColor(a_actorHandle, TRUEHUD_API::BarColorType::BarColor, staggerBarColor);
                    StaggerAVHUD->trueHUDInterface->OverrideSpecialBarColor(a_actorHandle, TRUEHUD_API::BarColorType::FlashColor, staggerFlashColor);
                    logger::info("{}: {}", settings->staggerAV_str, staggerAV);
                    return GetMaxStagger(a_actor) + staggerAV + FLT_MIN;
                }
            }
        }
        return 0.0f;
    }

    bool RequestStaggerBarControl()
    {
        if (!StaggerAVHUD) {
            logger::info("TrueHUD API not acquired. Cannot install Stagger HUD");
            return false;
        }

        auto bar_control = StaggerAVHUD->trueHUDInterface->RequestSpecialResourceBarsControl(SKSE::GetPluginHandle());
        switch (bar_control) {
        case TRUEHUD_API::APIResult::OK:
        case TRUEHUD_API::APIResult::AlreadyGiven:
            StaggerAVHUD->trueHUDInterface->RegisterSpecialResourceFunctions(SKSE::GetPluginHandle(), StaggerAVHUD->GetCurrentStaggerCooldown, StaggerAVHUD->GetMaxStagger, true, true);
            return true;
        case TRUEHUD_API::APIResult::AlreadyTaken:
            logger::info("TrueHUD API already taken by another plugin");
            return false;
        }
        return false;
    }

} // namespace StaggerHUD
