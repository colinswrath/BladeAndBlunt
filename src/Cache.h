#pragma once

namespace Cache
{
	inline uintptr_t IsAttackingAddress;
	inline uintptr_t IsBlockingAddress;
	inline uintptr_t HasSpellAddress;
	inline uintptr_t PlayerSingletonAddress;
	inline uintptr_t fPlayerMaxResistanceAddress;

	inline void CacheAddLibAddresses()
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

		//1.6
		//1.5 = 505721
		fPlayerMaxResistanceAddress = { REL::ID(505721).address()};
	}

	inline RE::PlayerCharacter* GetPlayerSingleton()
	{
		REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
		return singleton->get();
	}

	inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> GetfPlayerMaxResistSingleton()
	{
		REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> singleton{ fPlayerMaxResistanceAddress };
		return singleton;
	}
}
