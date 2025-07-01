#pragma once

#include "api/TrueHUDAPI.h"

namespace StaggerHUD
{
    class StaggerHUD
    {
    public:
        static StaggerHUD* GetSingleton()
        {
            static StaggerHUD manager;
            return &manager;
        }

        static float GetMaxStagger(RE::Actor* a_actor);
        static float GetCurrentStaggerCooldown(RE::Actor* a_actor);

        inline static TRUEHUD_API::IVTrueHUD3* trueHUDInterface;

    private:
        constexpr StaggerHUD() noexcept = default;
        StaggerHUD(const StaggerHUD&)   = delete;
        StaggerHUD(StaggerHUD&&)        = delete;

        ~StaggerHUD() = default;

        StaggerHUD& operator=(const StaggerHUD&) = delete;
        StaggerHUD& operator=(StaggerHUD&&)      = delete;
    };

    bool InstallTrueHUDHook();
    bool RequestStaggerBarControl();

    static StaggerHUD* StaggerAVHUD = nullptr;

} // namespace StaggerHUD
