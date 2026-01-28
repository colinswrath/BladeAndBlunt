#pragma once

namespace Cache
{
	inline uintptr_t IsAttackingAddress;
	inline uintptr_t IsBlockingAddress;
    inline uintptr_t HasSpellAddress;
    inline uintptr_t RemoveSpellAddress;
    inline uintptr_t IsCastingAddress;
    inline uintptr_t PlayerSingletonAddress;
    inline uintptr_t UISingletonAddress;
    inline uintptr_t PlayerCameraSingletonAddress;
	inline uintptr_t fPlayerMaxResistanceAddress;
	inline uintptr_t getUnarmedWeaponAddress;
	static float& g_deltaTime = (*(float*)RELOCATION_ID(523660, 410199).address());

	inline void CacheAddLibAddresses()
	{
		IsAttackingAddress      = REL::RelocationID(37637, 38590).address();
		IsBlockingAddress       = REL::RelocationID(36927, 37952).address();
		HasSpellAddress         = REL::RelocationID(37828, 38782).address();
        RemoveSpellAddress      = RELOCATION_ID(37772, 38717).address();
        IsCastingAddress        = RELOCATION_ID(37810, 38759).address(); 
		PlayerSingletonAddress  = REL::RelocationID(517014, 403521).address();
        UISingletonAddress      = REL::RelocationID(514178, 400327).address();
        PlayerCameraSingletonAddress = REL::RelocationID(514642, 400802).address();
		fPlayerMaxResistanceAddress = { REL::RelocationID(505721, 375300).address()};
		getUnarmedWeaponAddress = { REL::RelocationID(514923, 401061).address() };
	}

	inline RE::PlayerCharacter* GetPlayerSingleton()
	{
		REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
		return singleton->get();
	}

    inline RE::UI* GetUISingleton()
    {
        REL::Relocation<RE::UI**> singleton{ UISingletonAddress };
        return *singleton;
    }

    inline RE::PlayerCamera* GetPlayerCameraSingleton()
    {
        REL::Relocation<RE::PlayerCamera**> singleton{ PlayerCameraSingletonAddress };
        return *singleton;
    }

	inline REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> GetfPlayerMaxResistSingleton()
	{
		REL::Relocation<RE::SettingT<RE::GameSettingCollection>*> singleton{ fPlayerMaxResistanceAddress };
		return singleton;
	}
}
