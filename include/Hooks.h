#pragma once

namespace Hooks
{
	inline static REL::Relocation<std::uintptr_t> GetStaminaDamageHook(REL::RelocationID(25864, 26430));  //1.5,1.6,1.7
	inline static REL::Relocation<std::uintptr_t> GetStaminaBashHook(REL::RelocationID(25863,26429));    //1.5,1.6,1.7
    inline static REL::Relocation<std::uintptr_t> Player_Update_Hook{ REL::RelocationID(35565, 36564), REL::Relocate(0x1E, 0x6E) }; //1.5,1.6,1.7
    inline static REL::Relocation<std::uintptr_t> Actor_Update_Hook{ REL::RelocationID(36357, 37348), REL::Relocate(0x6D3, 0x683) };     // 0x683 in 1.7
    inline static REL::Relocation<std::uintptr_t> Actor_Update_Hook_Pre17{ REL::RelocationID(36357, 37348), REL::Relocate(0x6D3, 0x674) };     // 0x674 in 1.6
    inline static REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::RelocationID(37013, 37943), REL::Relocate(0x1A, 0x51) }; //1.5,1.6,1.7
	inline static REL::Relocation<std::uintptr_t> Block_GameSetting_Hook{ REL::RelocationID(42842,44014), REL::Relocate(0x452,0x438) };  //1.5,1.6,1.7
	inline static REL::Relocation<std::uintptr_t> fBlock_GameSetting{ REL::RelocationID(505023,374158), 0x8 }; //Check
	inline static REL::Relocation<std::uintptr_t> SpellCap_Hook{ REL::RelocationID(37792,38741), REL::Relocate(0x53,0x55) }; //1.5,1.6,1.7
	inline REL::Relocation<uintptr_t> arrow_release_handler{ REL::RelocationID(41778, 42859), REL::Relocate(0x133, 0x138) }; //1.5,1.6,1.7
    static REL::Relocation<std::uintptr_t> hitDataPop(REL::RelocationID(42842, 44014)); // 1.5,1.6,1.7
	static REL::Relocation<std::uintptr_t> armorRating2(REL::RelocationID(37605, 38558)); //1.5,1.6,1.7
    static REL::Relocation<std::uintptr_t> staggerHook(REL::RelocationID(34188, 34982), 0x61); //1.5,1.6,1.7


    bool InstallHooks();
    bool InstallBashMultHook();
    bool InstallTrueHUDHook();
    bool InstallStaggerHUDHook();
}
