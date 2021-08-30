#pragma once

#include "Conditions.h"
#include "Settings.h"

using namespace Conditions;
class FrameUpdateManager
{
public:
	inline static void Install()
	{
	
		REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(35565) , 0x1E };
		
		auto& trampoline = SKSE::GetTrampoline();
		_OnFrameFunction = trampoline.write_call<5>(OnFrame_Update_Hook.address(), OnFrameUpdate);

		logger::info("Installed hook for frame update");
	}

	inline static void InstallBowDrawnHook()
	{
		REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(39375) , 0x6B6 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<5>(OnFrame_Update_Hook.address(),IsBowDrawnCheck());
		logger::info("Installed hook for bow drawn");
	}

private:
	inline static std::int32_t OnFrameUpdate(std::int64_t a1)
	{

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		auto settings = Settings::GetSingleton();

		if (IsAttacking(player))
		{
			if (!HasSpell(player, settings->IsAttackingSpell))
			{
				player->AddSpell(settings->IsAttackingSpell);
				logger::info("IsAttacking start");
			}
		}
		else
		{
			if (HasSpell(player, settings->IsAttackingSpell))
			{
				player->RemoveSpell(settings->IsAttackingSpell);
				logger::info("IsAttacking finish");
			}
		}

		if (IsBlocking(player))
		{
			if (!HasSpell(player, settings->IsBlockingSpell))
			{
				player->AddSpell(settings->IsBlockingSpell);
				logger::info("IsBlocking Start");
			}
		}
		else
		{
			if (HasSpell(player, settings->IsBlockingSpell))
			{
				player->RemoveSpell(settings->IsBlockingSpell);
				logger::info("IsBlocking Finish");
			}
		}
		
		if (player->IsSneaking())
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

	inline static bool IsBowDrawnCheck()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto attackState = player->GetAttackState();

		auto playerCamera = RE::PlayerCamera::GetSingleton();
		if (playerCamera->bowZoomedIn)
		{
			return true;
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

			if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow)
			{
				return true;
			}
			break;
		}
		case RE::ATTACK_STATE_ENUM::kBowAttached:
		{
			return true;
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
