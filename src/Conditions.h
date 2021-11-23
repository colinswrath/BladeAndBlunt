#pragma once
//TODO Hardcoded offsets, put into separate header.
namespace Conditions
{

	static REL::Relocation<float*> secondsSinceLastFrameWorldTime{ REL::ID(410199) };

	static bool IsAttacking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsAttacking);

		REL::Relocation<func_t> func{ REL::ID(38590) };

		return func(actor);
	}

	inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

	static bool IsBlocking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsBlocking);

		REL::Relocation<func_t> func{ REL::ID(37952) };

		return func(actor);
	}

	inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

	static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
	{
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
