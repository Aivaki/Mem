#pragma once

#include "engine.hpp"


/* Entity class */


#pragma region entity
#ifndef entityDefine
#define entityDefine

namespace entity
{
	struct Entity
	{
	private:
		/* Dependencies */
		Updater Updater;
	private:
		typedef struct {
			float Matrix[3][4];
		} Matrix3x4_t;
	public:

		/* Getters */
		DWORD getEntBase(int index, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<DWORD>(mem.getClientBase() + Updater.dwEntityList + (index * 0x10));
		}
		//
		DWORD getEntBoneMatrix(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<DWORD>(playerBase + Updater.m_dwBoneMatrix);
		}
		//
		DWORD getActiveWeapon(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			DWORD weaponIndex = *mem.Read<DWORD>(playerBase + Updater.m_hActiveWeapon) & 0xFFF;
			return *mem.Read<DWORD>((mem.getClientBase() + Updater.dwEntityList + weaponIndex * 0x10) - 0x10);
		}
		//
		D3DXVECTOR3 getEntPos(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<D3DXVECTOR3>(playerBase + Updater.m_vecOrigin);
		}
		//
		D3DXVECTOR3 getEntEyePos(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return ((Entity::getEntPos(playerBase, mem)) + (*mem.Read<D3DXVECTOR3>(playerBase + Updater.m_vecViewOffset)));
		}
		//
		D3DXVECTOR3 getEntBonePos(DWORD playerBase, int boneID, const CSGOMemory& mem) const noexcept(true) {
			Matrix3x4_t boneMatrix = *mem.Read<Matrix3x4_t>(getEntBoneMatrix(playerBase, mem) + boneID * 0x30);

			return {
				boneMatrix.Matrix[0][3],
				boneMatrix.Matrix[1][3],
				boneMatrix.Matrix[2][3]
			};
		}
		//
		int getEntHp(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(playerBase + Updater.m_iHealth);
		}
		//
		int getEntTeam(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(playerBase + Updater.m_iTeamNum);
		}
		//
		int getLifeState(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(playerBase + Updater.m_lifeState);
		}
		//
		int getEntAmmo(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			auto weapon = getActiveWeapon(playerBase, mem);
			return *mem.Read<int>(weapon + Updater.m_iClip1);
		}
		//
		bool isAlive(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			const auto HP = getEntHp(playerBase, mem);

			if (HP > 0 && HP <= 100)
			    return true;
			return false;
		}
		//
		bool isValid(DWORD playerBase, const CSGOMemory& mem) noexcept(true) {
			if ((isAlive(playerBase, mem) && getEntTeam(playerBase, mem) != 0) && !isDormant(playerBase, mem) && getLifeState(playerBase, mem) == 0)
				return true;
			return false;
		}
		//
		bool getSpotted(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<bool>(playerBase + Updater.m_bSpotted);
		}
		//
		bool getEntHelmet(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<bool>(playerBase + Updater.m_bHasHelmet);
		}

		bool getEntImmunity(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<bool>(playerBase + Updater.m_bGunGameImmunity);
		}

		bool isDormant(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			bool dormant = *mem.Read<bool>(playerBase + Updater.m_bDormant);
			if (dormant) return true;

			return false;
		}
		//
		char* getEntName(DWORD playerBase, const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<char*>(playerBase + Updater.m_szCustomName);
		}
		//
		void setSpotted(DWORD playerBase, bool value, const CSGOMemory& mem) noexcept(true) {
			mem.Write<bool>(playerBase + Updater.m_bSpotted, value);
		}
	};
}

#endif
#pragma endregion