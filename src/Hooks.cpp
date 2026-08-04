#include "Managers/UpdateManager.h"
#include "Managers/NPCUpdateManager.h"
#include "Managers/StaggerHUDManager.h"
#include "patches/ArmorRatingScaling.h"
#include "patches/BashBlockStaminaPatch.h"
#include "Events.h"
#include "patches/MiscPatches.h"
#include "patches/BlockPowerAttackScalingPatch.h"

namespace Hooks
{
	bool InstallHooks()
	{
        if (!UpdateManager::Install()) {return false;}
        if (!NPCUpdateManager::Install()) {return false;}
		if (!MiscPatches::MiscPatches::InstallScalePatch()) { return false; }
		if (!MiscPatches::MiscPatches::InstallFBlockPatch()) { return false; }
		if (!MiscPatches::MiscPatches::InstallSpellCapPatch()) { return false; }
        if (!MiscPatches::MiscPatches::InstallStaggerFormulaPatch()) { return false; }

        AnimEventHandler::Install();
		WeaponFireHandler::InstallArrowReleaseHook();

		auto runtime = REL::Module::GetRuntime();
		if (runtime == REL::Module::Runtime::AE) {
		    if (Settings::GetSingleton()->armorScalingEnabled) {
				logger::info("Installing ar hook AE");
				ArmorRatingScaling::InstallArmorRatingHookAE();
                logger::info("Installed ar hook AE");
		    }

            BlockPowerAttackScalingPatch::InstallBlockPowerAttackPatch();

		} else {
            if (Settings::GetSingleton()->armorScalingEnabled) {
                logger::info("Installing ar hook SE");
                ArmorRatingScaling::InstallArmorRatingHookSE();
		        logger::info("Installed ar hook SE");
            }
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

    bool InstallTrueHUDHook()
    {
        return StaggerHUDManager::InstallTrueHUDHook();
    }

    bool InstallStaggerHUDHook()
    {
        return StaggerHUDManager::InstallStaggerHUDHook();
    }
}
