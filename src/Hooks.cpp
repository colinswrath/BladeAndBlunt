#include "UpdateManager.h"

namespace Hooks
{
	void Install()
	{
		UpdateManager::Install();
		UpdateManager::InstallBowDrawnHook();
		logger::info("Damage hook installed.");
	}
}
