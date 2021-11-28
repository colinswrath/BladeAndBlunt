#pragma once

#include "Conditions.h"
#include "Settings.h"

using namespace Conditions;
class UpdateManager
{
public:
	inline static void Install()
	{
		REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(36564), 0x6E};
		
		auto& trampoline = SKSE::GetTrampoline();
		_OnFrameFunction = trampoline.write_call<5>(OnFrame_Update_Hook.address(), OnFrameUpdate);

		logger::info("Installed hook for frame update");
	}

	inline static void InstallScalePatch()
	{
		REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::ID(38041), 0x1F };
		
		auto& trampoline = SKSE::GetTrampoline();
		_GetScaleFunction = trampoline.write_call<5>(Scale_Patch_Hook.address(), GetScale);

		logger::info("Installed hook for scale patch");
	}

private:
	inline static std::int32_t OnFrameUpdate(std::int64_t a1)
	{

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();
		auto playerCamera = RE::PlayerCamera::GetSingleton();

		if (IsBowDrawNoZoomCheck(player, playerCamera))
		{
			if (!HasSpell(player, settings->BowStaminaSpell))
			{
				player->AddSpell(settings->BowStaminaSpell);
			}
		}
		else
		{
			if (HasSpell(player, settings->BowStaminaSpell))
			{
				player->RemoveSpell(settings->BowStaminaSpell);
			}
		}

		if (IsAttacking(player))
		{
			if (!HasSpell(player, settings->IsAttackingSpell))
			{
				player->AddSpell(settings->IsAttackingSpell);
			}
		}
		else
		{
			if (HasSpell(player, settings->IsAttackingSpell))
			{
				player->RemoveSpell(settings->IsAttackingSpell);
			}
		}

		if (IsBlocking(player))
		{
			if (!HasSpell(player, settings->IsBlockingSpell))
			{
				player->AddSpell(settings->IsBlockingSpell);
			}
		}
		else
		{
			if (HasSpell(player, settings->IsBlockingSpell))
			{
				player->RemoveSpell(settings->IsBlockingSpell);
			}
		}

		if (player->IsSneaking() && IsMoving(player))
		{

			if(!HasSpell(player, settings->IsSneakingSpell))
				player->AddSpell(settings->IsSneakingSpell);
		}
		else
		{
			if(HasSpell(player, settings->IsSneakingSpell))
				player->RemoveSpell(settings->IsSneakingSpell);
		}

		return _OnFrameFunction(a1);
	}

	inline static REL::Relocation<decltype(OnFrameUpdate)> _OnFrameFunction;

	inline static float GetScale(RE::TESObjectREFR *a1)
	{
		if(skyrim_cast<RE::Actor*>(a1) == RE::PlayerCharacter::GetSingleton())
		{
			return a1->refScale / 100.0f;
		}
		else
		{
			return _GetScaleFunction(a1);
		}
	}

	inline static REL::Relocation<decltype(GetScale)> _GetScaleFunction;

	//Checks for bow being drawn but not zoomed
	static bool IsBowDrawNoZoomCheck(RE::PlayerCharacter* player, RE::PlayerCamera* playerCamera)
	{
		auto attackState = player->GetAttackState();

		if (playerCamera->bowZoomedIn)
		{
			return false;
		}

		switch (attackState)
		{
		case RE::ATTACK_STATE_ENUM::kBowDrawn:
		{
			auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));

			if (!equippedWeapon)
			{			
				break;
			}

			if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow || equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow)
			{
				return true;
			}
			break;
		}
		case RE::ATTACK_STATE_ENUM::kBowAttached:
		{
			auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));

			if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow)
			{
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
