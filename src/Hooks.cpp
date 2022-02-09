#include "UpdateManager.h"

namespace Hooks
{
	bool Install()
	{
		if (!UpdateManager::Install()) { return false; }
		if (!UpdateManager::InstallScalePatch()) { return false; }
		if (!UpdateManager::InstallFBlockPatch()) { return false; }
		if (!UpdateManager::InstallSpellCapPatch()) { return false; }
	}
}
