#pragma once

namespace Cache
{
	inline uintptr_t IsAttackingAddress;
	inline uintptr_t IsBlockingAddress;
	inline uintptr_t HasSpellAddress;
	inline uintptr_t PlayerSingletonAddress;

	inline void CacheAddLibAddresss()
	{
		//1.6 = 38590
		//1.5.97 = 37637
		IsAttackingAddress = REL::ID(37637).address();

		//1.6 = 37952
		//1.5.97 = 36927
		IsBlockingAddress = REL::ID(36927).address();

		//1.6 = 38782
		//1.5.97 = 37828
		HasSpellAddress = REL::ID(37828).address();

		//1.6 = 403521
		//1.5 = 517014
		PlayerSingletonAddress = REL::ID(517014).address();
	}

	inline RE::PlayerCharacter* GetPlayerSingleton()
	{
		REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
		return singleton->get();
	}
}
