#pragma once
#include "Cache.h"
#include "Settings.h"

//Originally intended to just implement some condition functions but iv been placing extensions/utility here as well
namespace Conditions
{ 
	static bool IsAttacking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsAttacking);
		REL::Relocation<func_t> func{ Cache::IsAttackingAddress };
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

	static bool IsBlocking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsBlocking);
		REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

	static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
	{	
		using func_t = decltype(&Conditions::HasSpell);

		REL::Relocation<func_t> func{ Cache::HasSpellAddress };

		return func(actor, spell);
	}
	inline static REL::Relocation<decltype(HasSpell)> _HasSpell;


	static bool IsMoving(RE::PlayerCharacter* player)
	{
		auto playerState = player->AsActorState();
		return (static_cast<bool>(playerState->actorState1.movingForward) || static_cast<bool>(playerState->actorState1.movingBack) || static_cast<bool>(playerState->actorState1.movingLeft) || static_cast<bool>(playerState->actorState1.movingRight));
	}

	static RE::TESObjectWEAP* GetUnarmedWeapon()
	{
		auto** singleton{ reinterpret_cast<RE::TESObjectWEAP**>(Cache::getUnarmedWeaponAddress) };
		return *singleton;
	}

	static inline bool PlayerHasActiveMagicEffect(RE::EffectSetting* a_effect)
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		auto activeEffects = player->AsMagicTarget()->GetActiveEffectList();
		RE::EffectSetting* setting = nullptr;
		for (auto& effect : *activeEffects) {
			setting = effect ? effect->GetBaseObject() : nullptr;
			if (setting) {
				if (setting == a_effect) {
					return true;
				}
			}
		}
		return false;
	}

	static inline float GetMaxHealth()
	{
		auto player = RE::PlayerCharacter::GetSingleton();

		return player->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, RE::ActorValue::kHealth) +
		       player->AsActorValueOwner()->GetPermanentActorValue(RE::ActorValue::kHealth);
	}

	static inline bool IsPowerAttacking(RE::Actor* actor) 
	{
		auto settings = Settings::GetSingleton();
		return settings->IsPowerAttacking->IsTrue(actor, nullptr);
	}

	//Credit: KernalsEgg for ApplySpell and IsPermanent
	//extensions
	static bool IsPermanent(RE::MagicItem* item)
	{
		switch (item->GetSpellType()) {
		case RE::MagicSystem::SpellType::kDisease:
		case RE::MagicSystem::SpellType::kAbility:
		case RE::MagicSystem::SpellType::kAddiction:
			{
				return true;
			}
		default:
			{
				return false;
			}
		}
	}
	
	static inline void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
	{
		if (IsPermanent(spell)) {
			target->AddSpell(spell);
		} else {
			caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
		}
	}
};
