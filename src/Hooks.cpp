#include "UpdateManager.h"

namespace Hooks
{
	void Install()
	{
		UpdateManager::Install();
		UpdateManager::InstallScalePatch();
		UpdateManager::InstallFBlockPatch();
		logger::info("Update hook installed.");
	}
}
