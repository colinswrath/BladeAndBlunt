#pragma once

#include "api/TrueHUDAPI.h"

class StaggerHUDBase
{
public:
    static StaggerHUDBase* GetSingleton()
    {
        static StaggerHUDBase manager;
        return &manager;
    }

    static float GetMaxStaggerCooldown(RE::Actor* a_actor);
    static float GetCurrentStaggerCooldown(RE::Actor* a_actor);

    inline static TRUEHUD_API::IVTrueHUD3* trueHUDInterface;

private:
    constexpr StaggerHUDBase() noexcept = default;
    StaggerHUDBase(const StaggerHUDBase&) = delete;
    StaggerHUDBase(StaggerHUDBase&&) = delete;

    ~StaggerHUDBase() = default;

    StaggerHUDBase& operator=(const StaggerHUDBase&) = delete;
    StaggerHUDBase& operator=(StaggerHUDBase&&) = delete;
};

class StaggerHUDManager
{
public:
    static bool InstallStaggerHUDHook();
    static bool InstallTrueHUDHook();
    static bool RequestStaggerBarControl();
    static void SetBarStyle();

private:
    static void Update(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell);
    inline static REL::Relocation<decltype(Update)> _Update;
};

namespace StaggerHUDSettings
{
    static StaggerHUDBase* HUDInstance;
    static uint32_t staggerBarColor;
    static uint32_t staggerFlashColor;
    inline static bool isInstalled = false;
} // namespace StaggerHUDSettings
