#pragma once
//TODO Hardcoded offsets, put into separate header.
namespace Conditions
{

	//static REL::Relocation<float*> secondsSinceLastFrameWorldTime{ REL::ID(523660) };
	static REL::Relocation<float*> secondsSinceLastFrameWorldTime{ REL::Offset(0x030064C8) };

	static bool IsAttacking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsAttacking);

		//REL::Relocation<func_t> func{ REL::ID(37637) };
		REL::Relocation<func_t> func{ REL::Offset(0x064CD90) };
		//64CD90
		//71EAD8
		return func(actor);
	}

	inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

	static bool IsBlocking(RE::Actor* actor)
	{
		using func_t = decltype(&Conditions::IsBlocking);
		
		//REL::Relocation<func_t> func{ REL::ID(36927) };
		REL::Relocation<func_t> func{ REL::Offset(0x006302F0) };

		return func(actor);
	}

	inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

	static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
	{
		using func_t = decltype(&Conditions::HasSpell);

		//REL::Relocation<func_t> func{ REL::ID(37828) };
		REL::Relocation<func_t> func{ REL::Offset(0x00658B40) };

		return func(actor, spell);
	}
	inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

	static bool IsMoving(RE::PlayerCharacter* player)
	{
		return (!static_cast<bool>(player->actorState1.movingForward) && !static_cast<bool>(player->actorState1.movingBack) && !static_cast<bool>(player->actorState1.movingLeft) && !static_cast<bool>(player->actorState1.movingRight));
	}
};
