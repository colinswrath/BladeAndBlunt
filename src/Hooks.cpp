#include "FrameUpdateManager.h"

namespace Hooks
{
	void Install()
	{
		FrameUpdateManager::Install();
		logger::info("Damage hook installed.");
	}
}
