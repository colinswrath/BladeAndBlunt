#pragma once

namespace Conditions
{
	//1.6 = 410199
	//1.5.97 = 523660
	static REL::Relocation<float*> secondsSinceLastFrameWorldTime{ REL::ID(523660) };

	static bool IsAttacking(RE::Actor* actor)
	{
		//1.6 = 38590
		//1.5.97 = 37637
		using func_t = decltype(&Conditions::IsAttacking);
		REL::Relocation<func_t> func{ REL::ID(38590) };
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

	static bool IsBlocking(RE::Actor* actor)
	{
		//1.6 = 37952
		//1.5.97 = 36927
		using func_t = decltype(&Conditions::IsBlocking);
		REL::Relocation<func_t> func{ REL::ID(37952) };
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

	static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
	{
		//1.6 = 38782
		//1.5.97 = 37828
		using func_t = decltype(&Conditions::HasSpell);

		REL::Relocation<func_t> func{ REL::ID(38782) };

		return func(actor, spell);
	}
	inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

	static bool IsMoving(RE::PlayerCharacter* player)
	{
		return (!static_cast<bool>(player->actorState1.movingForward) && !static_cast<bool>(player->actorState1.movingBack) && !static_cast<bool>(player->actorState1.movingLeft) && !static_cast<bool>(player->actorState1.movingRight));
	}
};
