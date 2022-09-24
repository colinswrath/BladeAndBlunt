#pragma once
#include "Cache.h"

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
};
