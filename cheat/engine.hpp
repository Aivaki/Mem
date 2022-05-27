#pragma once

#include "csgoMemory.hpp"


/* Engine class */


/* Memory class */
using namespace csgoMemory;
using namespace updater;

#pragma region engine
#ifndef engineDefine
#define engineDefine

namespace engine
{
	struct Engine
	{
	private:
		/* Dependencies */
		Updater Updater;
	public:

		/* Engine functions */
		DWORD getEnginePointer(const CSGOMemory& mem) const noexcept(false) {
			return *mem.Read<DWORD>(mem.getEngineBase() + Updater.dwClientState);
		}

		[[nodiscard]] void forceUpdate(const CSGOMemory& mem) noexcept(false) {
			mem.Write<int>(getEnginePointer(mem) + 0x16C, -1);
		}
		
		[[nodiscard]] bool isInGame(const CSGOMemory& mem) const noexcept(false) {
			if (mem.Read<int>(getEnginePointer(mem) + Updater.dwClientState_State) == 6) {
				return true;
			}
			return false;
		}
	};
}

#endif
#pragma endregion
