#pragma once

class ActorFrameStateHandler
{
public:
    static void UpdateActorState(RE::Actor* actor);

private:
    static void HandleIsCastingState(RE::Actor* actor);
    static void HandleIsDrawingBowState(RE::Actor* actor);
    static void HandleIsDrawingXbowState(RE::Actor* actor);
    static void HandleIsAttackingState(RE::Actor* actor);
    static void HandleIsBlockingState(RE::Actor* actor);
};
