#include "Managers/ActorFrameStateHandler.h"
#include "Settings.h"
#include "Utility/Utility.h"
#include "Utility/Extensions.h"

void ActorFrameStateHandler::UpdateActorState(RE::Actor* actor)
{
    //Check if we need to update
    if (!actor || !actor->Is3DLoaded() || !actor->IsAIEnabled() || actor->IsPlayerRef()) {
        return; //no update
    }

    HandleIsAttackingState(actor);
    HandleIsBlockingState(actor);
    HandleIsDrawingBowState(actor);
    HandleIsDrawingXbowState(actor);
    HandleIsCastingState(actor);

    // Checks are offloaded to every X frame to avoid unnecessary work
    /*switch (frameCount) {
    case 1:
        HandleIsCastingState();
        break;
    case 2:
        HandleIsDrawingBowState();
        break;
    case 3:
        HandleIsDrawingXbowState();
        break;
    case 4:
        HandleIsAttackingState();
        HandleIsBlockingState();
        break;
    case 5:
        HandleIsSneakingState();
        HandleIsSwimmingState();
        break;
    case 6:
        HandleIsSprintingState();
        break;
    default:
        break;
    }*/
}

void ActorFrameStateHandler::HandleIsCastingState(RE::Actor* actor)
{
    auto                 settings = Settings::GetSingleton();

    if (Actor::IsCasting(actor, nullptr)) {
        if (settings->MAG_CastStaminaStuntSpellNPC && !Actor::HasSpell(actor, settings->MAG_CastStaminaStuntSpellNPC)) {
            actor->AddSpell(settings->MAG_CastStaminaStuntSpellNPC);
        }
    }
    else if (settings->MAG_CastStaminaStuntSpellNPC && Actor::HasSpell(actor, settings->IsCastingSpell)) {
        Actor::RemoveSpell(actor, settings->MAG_CastStaminaStuntSpellNPC);
    }
}

void ActorFrameStateHandler::HandleIsDrawingBowState(RE::Actor* actor)
{
    auto                 settings = Settings::GetSingleton();
    
    if (Actor::IsDrawingBow(actor)) {
        if (!Actor::HasSpell(actor, settings->MAG_BowStaminaStuntSpellNPC)) {
            actor->AddSpell(settings->MAG_BowStaminaStuntSpellNPC);
        }
    }
    else if (Actor::HasSpell(actor, settings->MAG_BowStaminaStuntSpellNPC)) {
        Actor::RemoveSpell(actor, settings->MAG_BowStaminaStuntSpellNPC);
    }
}

void ActorFrameStateHandler::HandleIsDrawingXbowState(RE::Actor* actor)
{
    const auto settings = Settings::GetSingleton();

    const bool isDrawingXbow = Actor::IsDrawingBow(actor, true);
    const bool hasXbowSpell  = Actor::HasSpell(actor, settings->MAG_CrossbowStaminaStuntSpellNPC);

    if (isDrawingXbow && !hasXbowSpell) {
        actor->AddSpell(settings->MAG_CrossbowStaminaStuntSpellNPC);
    }
    else if (!isDrawingXbow && hasXbowSpell) {
        Actor::RemoveSpell(actor,settings->MAG_CrossbowStaminaStuntSpellNPC);
    }
}

void ActorFrameStateHandler::HandleIsAttackingState(RE::Actor* actor)
{
    const auto settings = Settings::GetSingleton();

    const bool isAttacking       = Actor::IsAttacking(actor);
    const bool hasAttackingSpell = Actor::HasSpell(actor, settings->MAG_AttackStaminaStuntSpellNPC);

    if (isAttacking) {
        if (!hasAttackingSpell) {
            actor->AddSpell(settings->MAG_AttackStaminaStuntSpellNPC);
        }
    }
    else if (!isAttacking && hasAttackingSpell) {
        Actor::RemoveSpell(actor, settings->MAG_AttackStaminaStuntSpellNPC);
    }
}

void ActorFrameStateHandler::HandleIsBlockingState(RE::Actor* actor)
{
    const auto settings = Settings::GetSingleton();

    if (Actor::IsAttacking(actor)) {
        return;
    }

    const bool isBlocking       = Actor::IsBlocking(actor);
    const bool hasBlockingSpell = Actor::HasSpell(actor, settings->MAG_BlockStaminaStuntSpellNPC);

    if (isBlocking) {
        if (!hasBlockingSpell) {
            actor->AddSpell(settings->MAG_BlockStaminaStuntSpellNPC);
        }
    }
    else {
        if (hasBlockingSpell) {
            Actor::RemoveSpell(actor,settings->MAG_BlockStaminaStuntSpellNPC);
        }
    }
}
