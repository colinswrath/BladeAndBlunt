#pragma once

#include "Conditions.h"
#include "Settings.h"

using namespace Conditions;
class FrameUpdateManager
{
public:
	inline static void Install()
	{
	
		REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(35565) , 0x1E};
		
		auto& trampoline = SKSE::GetTrampoline();
		_OnFrameFunction = trampoline.write_call<5>(OnFrame_Update_Hook.address(), OnFrameUpdate);

		logger::info("Installed hook for frame update");
	}

	inline static void InstallBowDrawnHook()
	{
		REL::Relocation<std::uintptr_t> BowDrawHook{ REL::ID(39375) , 0x6B6 };
		REL::Relocation<std::uintptr_t> TestAlAl{ REL::ID(39375), 0x6BB };
		std::array<std::uint8_t, 2> test{ 0x84, 0xC0 };

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(BowDrawHook.address(),IsBowDrawnCheck);


		REL::safe_write<std::uint8_t>(TestAlAl.address(), test);
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

	static bool IsBowDrawnCheck()
	{
		auto player = RE::PlayerCharacter::GetSingleton();
		auto attackState = player->GetAttackState();
		auto settings = Settings::GetSingleton();

		auto playerCamera = RE::PlayerCamera::GetSingleton();
		if (playerCamera->bowZoomedIn)
		{
			if (HasSpell(player,settings->BowDrainStaminaSpell))
			{
				player->RemoveSpell(settings->BowDrainStaminaSpell);
			}
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

			if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow || equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kCrossbow)
			{
				if (!HasSpell(player, settings->BowDrainStaminaSpell))
				{				
					player->AddSpell(settings->BowDrainStaminaSpell);
					//player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, settings->BowDrainStamMagnitude);
				}
				return false;
			}
			break;
		}
		case RE::ATTACK_STATE_ENUM::kBowAttached:
		{
			auto equippedWeapon = skyrim_cast<RE::TESObjectWEAP*>(player->GetEquippedObject(false));

			if (equippedWeapon->GetWeaponType() == RE::WEAPON_TYPE::kBow)
			{
				if (!HasSpell(player, settings->BowDrainStaminaSpell))
				{
					player->AddSpell(settings->BowDrainStaminaSpell);
					//player->RestoreActorValue(RE::ACTOR_VALUE_MODIFIER::kDamage, RE::ActorValue::kStamina, settings->BowDrainStamMagnitude);
				}
				return false;
			}
			break;
		}
		
		default:
		{
			break;
		}
		}
		if (HasSpell(player, settings->BowDrainStaminaSpell))
		{
			player->RemoveSpell(settings->BowDrainStaminaSpell);
		}
		return false;
	}

	
};
