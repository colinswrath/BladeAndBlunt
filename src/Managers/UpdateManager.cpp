#include "Managers/UpdateManager.h"


std::int32_t UpdateManager::OnFrameUpdate(std::int64_t a1)
{
    auto settings = Settings::GetSingleton();

        if (frameCount > settings->maxFrameCheck) {
            frameCount = 0;
        }
        else {
            PlayerFrameStateHandler::UpdatePlayerState(frameCount);

            // Check difficulty global
        }

        if (!Cache::GetUISingleton()->GameIsPaused()) {
            // Check for tasks to perform. ie. delaying forcing out of bow draw use the task system
            TaskManager::GetSingleton().Update();

            if (Cache::g_deltaTime > 0) {
                lastTime += Cache::g_deltaTime;
                if (lastTime >= settings->injuryUpdateFrequency) {
                    settings->MAG_DifficultyGlobal->value = static_cast<float>(Cache::GetPlayerSingleton()->GetGameStatsData().difficulty);
                    auto inj = InjuryPenaltyHandler::GetSingleton();
                    inj->CheckInjuryAvPenalty();
                    lastTime = 0;
                }
            }
        }

    frameCount++;
    return _OnFrameFunction(a1);
}

bool UpdateManager::Install()
{
    auto& trampoline = SKSE::GetTrampoline();
    _OnFrameFunction = trampoline.write_call<5>(Hooks::Player_Update_Hook.address(), OnFrameUpdate);

    frameCount = 0;
    logger::info("Installed hook for frame update");
    return true;
}
