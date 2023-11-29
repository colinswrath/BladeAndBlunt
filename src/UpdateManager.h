#pragma once

#include "Conditions.h"
#include "InjuryPenaltyManager.h"
#include "Hooks.h"

static float lastTime;

using namespace Conditions;

class UpdateManager
{
public:
	inline static int frameCount;
	inline static bool Install()
	{
		auto& trampoline = SKSE::GetTrampoline();
		_OnFrameFunction = trampoline.write_call<5>(Hooks::OnFrame_Update_Hook.address(), OnFrameUpdate);

		UpdateManager::frameCount = 0;
		logger::info("Installed hook for frame update");
		return true;
	}

private:
	inline static std::int32_t OnFrameUpdate(std::int64_t a1)
	{	
		auto settings = Settings::GetSingleton();
		if (UpdateManager::frameCount > 6)
		{
			UpdateManager::frameCount = 0;
			RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
			auto playerCamera = RE::PlayerCamera::GetSingleton();

			if (IsBowDrawNoZoomCheck(player, playerCamera)) {
				if (!HasSpell(player, settings->BowStaminaSpell)) {
					player->AddSpell(settings->BowStaminaSpell);
				}
			}
			else if (HasSpell(player, settings->BowStaminaSpell)) {
				player->RemoveSpell(settings->BowStaminaSpell);
			}

			if (IsXbowDrawCheck(player, playerCamera)) {
				if (!HasSpell(player, settings->XbowStaminaSpell)) {
					player->AddSpell(settings->XbowStaminaSpell);
				}
			} else if (HasSpell(player, settings->XbowStaminaSpell)) {
				player->RemoveSpell(settings->XbowStaminaSpell);
			}

			if (IsAttacking(player)) {
				if (!HasSpell(player, settings->IsAttackingSpell)) {
					player->AddSpell(settings->IsAttackingSpell);
				}
			}
			else {
				if (HasSpell(player, settings->IsAttackingSpell))
				{
					player->RemoveSpell(settings->IsAttackingSpell);
				}

				if (IsBlocking(player)) {
					auto leftHand = player->GetEquippedObject(true);
					//Parry setup
					if ((!leftHand || leftHand->IsWeapon()) && !settings->IsBlockingWeaponSpellCasted) {
						settings->IsBlockingWeaponSpellCasted = true;
						Conditions::ApplySpell(player, player, settings->MAGParryControllerSpell);
					}

					if (!HasSpell(player, settings->IsBlockingSpell))
					{
						player->AddSpell(settings->IsBlockingSpell);
					}
				}
				else
				{
					if (HasSpell(player, settings->IsBlockingSpell)) {
						player->RemoveSpell(settings->IsBlockingSpell);
					}
					settings->IsBlockingWeaponSpellCasted = false;
				}
			}
			if (player->IsSneaking() && IsMoving(player))
			{
				if(!HasSpell(player, settings->IsSneakingSpell) && settings->enableSneakStaminaCost)
					player->AddSpell(settings->IsSneakingSpell);
			}
			else if(HasSpell(player, settings->IsSneakingSpell)) {
					player->RemoveSpell(settings->IsSneakingSpell);
			}
		}

		//Updates for AV
		if (!RE::UI::GetSingleton()->GameIsPaused()) {
			if (Cache::g_deltaTime > 0) {
				lastTime += Cache::g_deltaTime;
				if (lastTime >= settings->injuryUpdateFrequency) {
					auto inj = InjuryPenaltyHandler::GetSingleton();
					inj->CheckInjuryAvPenalty();
					lastTime = 0;
				}
			}
		}

		UpdateManager::frameCount++;
		return _OnFrameFunction(a1);
	}

	inline static REL::Relocation<decltype(OnFrameUpdate)> _OnFrameFunction;

	static bool IsXbowDrawCheck(RE::PlayerCharacter* player, RE::PlayerCamera* playerCamera)
	{
		auto attackState = player->AsActorState()->GetAttackState();

		if (playerCamera->bowZoomedIn) {
			return false;
		}

		auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));
		if (!equippedWeapon) {
			return false;
		}

		switch (attackState) {
			case RE::ATTACK_STATE_ENUM::kBowDrawn:
			{
				if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow) {
					return true;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		return false;
	}

	static bool IsBowDrawNoZoomCheck(RE::PlayerCharacter* player, RE::PlayerCamera* playerCamera)
	{
		auto attackState = player->AsActorState()->GetAttackState();

		if (playerCamera->bowZoomedIn)
		{
			return false;
		}

		auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));
		if (!equippedWeapon) {
			return false;
		}

		switch (attackState) {
			case RE::ATTACK_STATE_ENUM::kBowDrawn:{
				
				if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow){
					return true;
				}
				break;
			}
			case RE::ATTACK_STATE_ENUM::kBowAttached:{

				if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow) {
					return true;
				}
				break;
			}
			default:
			{
				break;
			}
		}
		return false;
	}
};
