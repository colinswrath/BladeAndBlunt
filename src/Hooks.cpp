#include "UpdateManager.h"

namespace Hooks
{
	void Install()
	{
		UpdateManager::Install();
		logger::info("Damage hook installed.");
	}
}
