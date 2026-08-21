#pragma once

#include "Hooks.h"
#include "Managers/ActorFrameStateHandler.h"
#include "Utility/Utility.h"

using namespace Utility;


class NPCUpdateManager
{
public:
    static bool Install();

private:
    static std::int32_t OnUpdate(RE::Actor* a_actor, float a_zPos, RE::TESObjectCELL* a_cell);

    inline static REL::Relocation<decltype(OnUpdate)> _OnUpdateFunction;
};
