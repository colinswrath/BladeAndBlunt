#include "UpdateManager.h"

namespace Hooks
{
	void Install()
	{
		UpdateManager::Install();
		UpdateManager::InstallScalePatch();
		logger::info("Update hook installed.");
	}
}
