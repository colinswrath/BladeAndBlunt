#include "FrameUpdateManager.h"

namespace Hooks
{
	void Install()
	{
		FrameUpdateManager::Install();
		FrameUpdateManager::InstallBowDrawnHook();
		logger::info("Damage hook installed.");
	}
}
