#include "UpdateManager.h"
#include "ArmorRatingScaling.h"
#include "BashBlockStaminaPatch.h"

namespace Hooks
{
	bool InstallHooks()
	{
		if (!UpdateManager::Install()) { return false; }
		if (!UpdateManager::InstallScalePatch()) { return false; }
		if (!UpdateManager::InstallFBlockPatch()) { return false; }
		if (!UpdateManager::InstallSpellCapPatch()) { return false; }

		auto runtime = REL::Module::GetRuntime();
		if (runtime == REL::Module::Runtime::AE) {
			ArmorRatingScaling::InstallArmorRatingHookAE();
		} else {
			ArmorRatingScaling::InstallArmorRatingHookSE();	
		}

		if (!BashBlockStaminaPatch::InstallBlockMultHook()) {
			return false;
		}

		return true;
	}

	bool InstallBashMultHook() 
	{
		return BashBlockStaminaPatch::InstallBashMultHook();
	}
}
