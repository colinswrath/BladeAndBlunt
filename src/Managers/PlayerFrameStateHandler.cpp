#include "Managers/PlayerFrameStateHandler.h"
#include "Settings.h"
#include "Utility/Extensions.h"
#include "Utility/Utility.h"

void PlayerFrameStateHandler::UpdatePlayerState(int frameCount)
{
    RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

    if (player->IsGodMode()) {
        RemoveAllStateSpells();
    }
    else {
        //Checks are offloaded to every X frame to avoid unnecessary work
        switch (frameCount) {
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
        }
    }
}

void PlayerFrameStateHandler::HandleIsCastingState()
{
    auto settings = Settings::GetSingleton();
    RE::PlayerCharacter* player = Cache::GetPlayerSingleton();

    if (player->IsCasting(nullptr)) {
        if (settings->IsCastingSpell && !Actor::HasSpell(player,settings->IsCastingSpell)) {
            player->AddSpell(settings->IsCastingSpell);
        }
    }
    else if (settings->IsCastingSpell && Actor::HasSpell(player,settings->IsCastingSpell)) {
        Actor::RemoveSpell(player,settings->IsCastingSpell);
    }
}

void PlayerFrameStateHandler::HandleIsDrawingBowState()
{
    auto                 settings = Settings::GetSingleton();
    RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();

    if (Actor::IsDrawingBow(player)) {

        if (!Actor::HasSpell(player,settings->BowStaminaSpell)) {
            player->AddSpell(settings->BowStaminaSpell);
        }
    }
    else if (Actor::HasSpell(player,settings->BowStaminaSpell)) {
        Actor::RemoveSpell(player,settings->BowStaminaSpell);
    }
}

void PlayerFrameStateHandler::HandleIsDrawingXbowState()
{
    const auto settings = Settings::GetSingleton();
    const auto player   = Cache::GetPlayerSingleton();

    const bool isDrawingXbow = Actor::IsDrawingBow(player, true);
    const bool hasXbowSpell  = Actor::HasSpell(player,settings->XbowStaminaSpell);

    if (isDrawingXbow && !hasXbowSpell) {
        player->AddSpell(settings->XbowStaminaSpell);
    }
    else if (!isDrawingXbow && hasXbowSpell) {
        Actor::RemoveSpell(player,settings->XbowStaminaSpell);
    }
}

void PlayerFrameStateHandler::HandleIsAttackingState()
{
    const auto settings = Settings::GetSingleton();
    const auto player   = Cache::GetPlayerSingleton();

    const bool isAttacking = Actor::IsAttacking(player);
    const bool hasAttackingSpell = Actor::HasSpell(player, settings->IsAttackingSpell);

    if (isAttacking) {
        settings->wasPowerAttacking = Utility::IsPowerAttacking(player);
        if (!hasAttackingSpell) {
            player->AddSpell(settings->IsAttackingSpell);
        }
    }
    else if (!isAttacking && hasAttackingSpell) {
        Actor::RemoveSpell(player,settings->IsAttackingSpell);
        if (settings->wasPowerAttacking) {
            settings->wasPowerAttacking = false;
            Utility::ApplySpell(player, player, settings->PowerAttackStopSpell);
        }
    }

}

void PlayerFrameStateHandler::HandleIsBlockingState()
{
    const auto settings = Settings::GetSingleton();
    const auto player   = Cache::GetPlayerSingleton();

    const bool isAttacking       = player->IsAttacking();

    if (isAttacking) {
        return;
    }

    const bool isBlocking       = player->IsBlocking();
    const bool hasBlockingSpell = Actor::HasSpell(player,settings->IsBlockingSpell);


    if (isBlocking) {
        // Parry setup
        if (!settings->IsBlockingWeaponSpellCasted) {
            settings->IsBlockingWeaponSpellCasted = true;
            Utility::ApplySpell(player, player, settings->MAGParryControllerSpell);
        }

        if (!hasBlockingSpell) {
            player->AddSpell(settings->IsBlockingSpell);
        }
    }
    else {
        settings->IsBlockingWeaponSpellCasted = false;
        if (hasBlockingSpell) {
            Actor::RemoveSpell(player,settings->IsBlockingSpell);
        }
    }
}

void PlayerFrameStateHandler::HandleIsSneakingState()
{
    const auto settings = Settings::GetSingleton();
    const auto player   = Cache::GetPlayerSingleton();

    const bool isSneakingAndMoving = player->IsSneaking() && player->IsMoving();
    const bool hasSneakingSpell    = Actor::HasSpell(player,settings->IsSneakingSpell);

    if (isSneakingAndMoving && !hasSneakingSpell && settings->enableSneakStaminaCost) {
        player->AddSpell(settings->IsSneakingSpell);
    }
    else if ((!isSneakingAndMoving || !settings->enableSneakStaminaCost) && hasSneakingSpell) {
        Actor::RemoveSpell(player,settings->IsSneakingSpell);
    }
}

void PlayerFrameStateHandler::HandleIsSwimmingState()
{
    const auto settings = Settings::GetSingleton();
    const auto player   = Cache::GetPlayerSingleton();

    const bool isSwimmingAndMoving = player->AsActorState()->IsSwimming() && player->IsMoving();
    const bool hasSwimmingSpell    = Actor::HasSpell(player,settings->IsSwimmingSpell);

    if (isSwimmingAndMoving && !hasSwimmingSpell) {
        player->AddSpell(settings->IsSwimmingSpell);
    }
    else if (!isSwimmingAndMoving && hasSwimmingSpell) {
        Actor::RemoveSpell(player,settings->IsSwimmingSpell);
    }
}

void PlayerFrameStateHandler::RemoveAllStateSpells()
{
    auto                 settings = Settings::GetSingleton();
    RE::PlayerCharacter* player   = Cache::GetPlayerSingleton();

    if (settings->IsCastingSpell)
        Actor::RemoveSpell(player,settings->IsCastingSpell);

    if (settings->BowStaminaSpell)
        Actor::RemoveSpell(player,settings->BowStaminaSpell);

    if (settings->IsAttackingSpell)
        Actor::RemoveSpell(player,settings->IsAttackingSpell);

    if (settings->XbowStaminaSpell)
        Actor::RemoveSpell(player,settings->XbowStaminaSpell);

    if (settings->IsAttackingSpell)
        Actor::RemoveSpell(player,settings->IsAttackingSpell);

    if (settings->IsBlockingSpell)
        Actor::RemoveSpell(player,settings->IsBlockingSpell);

    if (settings->IsSneakingSpell)
        Actor::RemoveSpell(player,settings->IsSneakingSpell);

    if (settings->IsSprintingSpell)
        Actor::RemoveSpell(player,settings->IsSprintingSpell);

    if (settings->IsSwimmingSpell)
        Actor::RemoveSpell(player,settings->IsSwimmingSpell);
}

void PlayerFrameStateHandler::HandleIsSprintingState()
{
    const auto   settings   = Settings::GetSingleton();
    const auto   player     = Cache::GetPlayerSingleton();

    RE::ActorPtr actorCheck = nullptr;
    bool         isMounted  = player->GetMount(actorCheck);
    auto         state      = isMounted ? actorCheck->AsActorState() : player->AsActorState();

    if (state->IsSprinting()) {
        if (isMounted && actorCheck && !actorCheck->HasSpell(settings->MountSprintingSpell)) {
            actorCheck->AddSpell(settings->MountSprintingSpell);
        }
        else {
            if (!Actor::HasSpell(player,settings->IsSprintingSpell))
                player->AddSpell(settings->IsSprintingSpell);
        }
    }
    else if (Actor::HasSpell(player,settings->IsSprintingSpell)) {
        Actor::RemoveSpell(player,settings->IsSprintingSpell);

        if (actorCheck) {
            actorCheck->RemoveSpell(settings->MountSprintingSpell);
        }
    }
}
