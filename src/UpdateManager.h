#pragma once

#include "Conditions.h"
#include "Settings.h"
#include "Hooks.h"

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

	inline static bool InstallScalePatch()
	{	
		auto& trampoline = SKSE::GetTrampoline();
		_GetScaleFunction = trampoline.write_call<5>(Hooks::Scale_Patch_Hook.address(), GetScale);

		logger::info("Installed hook for scale patch");
		return true;
	}

	inline static bool InstallFBlockPatch()
	{
		const std::int32_t fBlockOffset = static_cast<std::int32_t>(Hooks::fBlock_GameSetting.address() - (Hooks::Block_GameSetting_Hook.address() + 0x8));
		
		REL::safe_write(Hooks::Block_GameSetting_Hook.address() + 0x4, fBlockOffset);

		logger::info("Block max hook installed");
		return true;
	}

	inline static bool InstallSpellCapPatch()
	{
		const std::uint8_t expectedBytes[] = {0x41, 0xFF, 0x50, 0x08, 0xF3, 0x0F, 0x2C, 0xC0};
		std::uint8_t noopPatch[] = { 0x90, 0x90, 0x90 };

		if (std::memcmp(reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(Hooks::SpellCap_Hook.address())), expectedBytes, sizeof(expectedBytes)))
		{
			logger::error("ERROR: Expected byte mismatch on Spell Absorb Cap hook");
			return false;
		}

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_call<5>(Hooks::SpellCap_Hook.address(), AbsorbCapPatch);
		REL::safe_write(Hooks::SpellCap_Hook.address() + 0x5, noopPatch, 3);

		logger::info("Absorb cap hook installed.");
		return true;
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
			
			return a1->GetReferenceRuntimeData().refScale / 100.0f;
		}
		else
		{
			return scale;
		}
	}

	inline static REL::Relocation<decltype(GetScale)> _GetScaleFunction;

	static bool IsBowDrawNoZoomCheck(RE::PlayerCharacter* player, RE::PlayerCamera* playerCamera)
	{
		auto attackState = player->AsActorState()->GetAttackState();

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
