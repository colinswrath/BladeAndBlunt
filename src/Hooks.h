#pragma once

namespace Hooks
{
	//1.6 = REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(36564), 0x6E };
	inline static REL::Relocation<std::uintptr_t> OnFrame_Update_Hook{ REL::ID(35565), 0x1E };

	//1.6 = REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::ID(38041), 0x1F };
	inline static REL::Relocation<std::uintptr_t> Scale_Patch_Hook{ REL::ID(37013), 0x1A };

	//1.6 = REL::Relocation<std::uintptr_t> Block_GameSetting_Hook{ REL::ID(44014), 0x438 };
	//1.6 = REL::Relocation<std::uintptr_t> fBlock_GameSetting { REL::ID(374158) };
	inline static REL::Relocation<std::uintptr_t> Block_GameSetting_Hook{ REL::ID(42842), 0x452 };
	inline static REL::Relocation<std::uintptr_t> fBlock_GameSetting{ REL::ID(505023), 0x8 };

	// 1.6 REL::Relocation<std::uintptr_t> SpellCap_Hook{ REL::ID(38741), 0x55 };
	inline static REL::Relocation<std::uintptr_t> SpellCap_Hook{ REL::ID(37792), 0x53 };

	bool Install();
}
