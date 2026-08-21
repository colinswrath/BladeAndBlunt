#include "Managers/NPCUpdateManager.h"


std::int32_t NPCUpdateManager::OnUpdate(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell)
{
    ActorFrameStateHandler::UpdateActorState(a_actor);

    return _OnUpdateFunction(a_actor, a_zPos, a_cell);
}

bool NPCUpdateManager::Install()
{
    logger::info("Installing Actor update");

    auto& trampoline = SKSE::GetTrampoline();
    _OnUpdateFunction = trampoline.write_call<5>(Hooks::Actor_Update_Hook.address(), OnUpdate);
    logger::info("Actor Update Hook installed");
    return true;
}
