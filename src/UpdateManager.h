#pragma once

#include "Conditions.h"
#include "Settings.h"

using namespace Conditions;
class UpdateManager
{
public:
	inline static int frameCount;
	inline static void Install()
	{
		//1.6 = REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(36564), 0x6E };
		REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(35565), 0x1E};
		
		auto& trampoline = SKSE::GetTrampoline();
		_OnFrameFunction = trampoline.write_call<5>(OnFrame_Update_Hook.address(), OnFrameUpdate);

		UpdateManager::frameCount = 0;
		logger::info("Installed hook for frame update");
	}

	inline static void InstallScalePatch()
	{
		//1.6 = REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::ID(38041), 0x1F };
		REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::ID(37013), 0x1A };
		
		auto& trampoline = SKSE::GetTrampoline();
		_GetScaleFunction = trampoline.write_call<5>(Scale_Patch_Hook.address(), GetScale);

		logger::info("Installed hook for scale patch");
	}

	inline static void InstallFBlockPatch()
	{
		//1.6 = REL::Relocation<std::uintptr_t> Block_GameSetting_Hook{ REL::ID(44014), 0x438 };
		//1.6 = REL::Relocation<std::uintptr_t> fBlock_GameSetting { REL::ID(374158) };

		REL::Relocation<std::uintptr_t> Block_GameSetting_Hook{ REL::ID(42842), 0x452 };
		REL::Relocation<std::uintptr_t> fBlock_GameSetting{ REL::ID(505023), 0x8 };

		const std::int32_t fBlockOffset = static_cast<std::int32_t>(fBlock_GameSetting.address() - (Block_GameSetting_Hook.address() + 0x8));
		
		REL::safe_write(Block_GameSetting_Hook.address() + 0x4, fBlockOffset);

		logger::info("Block max hook installed");
	}

	inline static void InstallSpellCapPatch()
	{
		// 1.6 REL::Relocation<std::uintptr_t> SpellCap_Hook{ REL::ID(38741), 0x55 };
		std::uint8_t noopPatch[] = { 0x90, 0x90, 0x90 };
		REL::Relocation<std::uintptr_t> SpellCap_Hook{ REL::ID(37792), 0x53};

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(SpellCap_Hook.address(), AbsorbCapPatch);
		REL::safe_write(SpellCap_Hook.address() + 0x5, noopPatch, 3);

		logger::info("Absorb cap hook installed.");
	}
	

private:
	inline static std::int32_t OnFrameUpdate(std::int64_t a1)
	{
		if (UpdateManager::frameCount > 4)
		{
			UpdateManager::frameCount = 0;
			RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
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
		}
		UpdateManager::frameCount++;
		return _OnFrameFunction(a1);
	}

	inline static REL::Relocation<decltype(OnFrameUpdate)> _OnFrameFunction;

	inline static float GetScale(RE::TESObjectREFR *a1)
	{
		auto scale = _GetScaleFunction(a1);
		if(skyrim_cast<RE::Actor*>(a1) == RE::PlayerCharacter::GetSingleton())
		{
			return a1->refScale / 100.0f;
		}
		else
		{
			return scale;
		}
	}

	inline static REL::Relocation<decltype(GetScale)> _GetScaleFunction;

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

	inline static std::int32_t AbsorbCapPatch(RE::ActorValueOwner *akAvOwner, RE::ActorValue akValue)
	{			
		auto cap = (int32_t)akAvOwner->GetActorValue(akValue);
			
		float playerMax = Cache::GetfPlayerMaxResistSingleton()->GetFloat();
	
		auto max = (int32_t)playerMax;
		
		if (cap > playerMax)
		{
			return max;
		}
		else
		{
			return cap;
		}	
	}
};
