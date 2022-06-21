#pragma once

#include "vector.hpp"


/* Main class - LocalPlayer */


/* Dependencies */
using namespace vector;
using namespace entity;
using namespace engine;

#pragma region local player
#ifndef localPlayerDefine
#define localPlayerDefine

namespace localPlayer
{
	struct LocalPlayer
	{
	private:
		/* Dependencies */
		Updater Updater;
		Vector Vector;
		Entity Entity;
		Engine Engine;
	public:

		/* Getters */
		DWORD getLocalPlayer(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<DWORD>(mem.getClientBase() + Updater.dwLocalPlayer);
		};
		//
		DWORD getClosestTarget(int fov, const CSGOMemory& mem) noexcept(true) {

			D3DXVECTOR3 viewAngles = getLocalViewAngles(mem);
			D3DXVECTOR3 localEyePos = getLocalEyePos(mem);

			DWORD bestEntity = NULL;

			for (int i = 0; i < 64; i++) {
				if (Entity.isValid(Entity.getEntBase(i, mem), mem)
					&& Entity.getEntTeam(Entity.getEntBase(i, mem), mem) != getLocalTeam(mem)
					&& i != NULL) {

					D3DXVECTOR3 angle = Vector.calcAngle(localEyePos, Entity.getEntBonePos(Entity.getEntBase(i, mem), aimBone, mem));
					D3DXVECTOR3 cAngles = Vector.clampAngles(Vector.clamAngles(angle - viewAngles));
					
					auto delta = static_cast<int>(sqrt(cAngles.x * cAngles.x + cAngles.y * cAngles.y));

					if (delta < fov) {
						fov = delta;
						bestEntity = Entity.getEntBase(i, mem);
					}
				}
			}
			return bestEntity;
		}
		//
		int getLocalFlags(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(getLocalPlayer(mem) + Updater.m_fFlags);
		};
		//
		int getLocalHealth(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(getLocalPlayer(mem) + Updater.m_iHealth);
		};
		//
		int getLocalTeam(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(getLocalPlayer(mem) + Updater.m_iTeamNum);
		};
		//
		int getShotsFired(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(getLocalPlayer(mem) + Updater.m_iShotsFired);
		};
		//
		int getLocalCrossID(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<int>(getLocalPlayer(mem) + Updater.m_iCrosshairId);
		};
		//
		D3DXVECTOR3 getLocalPos(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<D3DXVECTOR3>(getLocalPlayer(mem) + Updater.m_vecOrigin);
		};
		//
		D3DXVECTOR3 getLocalEyePos(const CSGOMemory& mem) const noexcept(true) {
			D3DXVECTOR3 localPos = getLocalPos(mem);
			localPos += *mem.Read<D3DXVECTOR3>(getLocalPlayer(mem) + Updater.m_vecViewOffset);
			return localPos;
		};
		//
		D3DXVECTOR3 getLocalViewAngles(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<D3DXVECTOR3>(Engine.getEnginePointer(mem) + Updater.dwClientState_ViewAngles);
		};
		//
		D3DXMATRIX getLocalViewMatrix(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<D3DXMATRIX>(mem.getClientBase() + Updater.dwViewMatrix);
		};
		//
		D3DXVECTOR3 getLocalPunchAngles(const CSGOMemory& mem) const noexcept(true) {
			return *mem.Read<D3DXVECTOR3>(getLocalPlayer(mem) + Updater.m_aimPunchAngle);
		};


		/* Setters */
		void setLocalViewAngles(const D3DXVECTOR3& angles, const CSGOMemory& mem) noexcept(true) {
			mem.Write<D3DXVECTOR3>(Engine.getEnginePointer(mem) + Updater.dwClientState_ViewAngles, angles);
		}
		//
		void setFlashMaxAlpha(float& alpha, const CSGOMemory& mem) noexcept(true) {
			mem.Write<float>(getLocalPlayer(mem) + Updater.m_flFlashMaxAlpha, alpha);
		}
		
		
		/* Weapon filter function
		   No longer working :(   */
		[[nodiscard]] bool isValidWeapon(const CSGOMemory& mem) {
			short myWeapon = *mem.Read<int>(Entity.getActiveWeapon(getLocalPlayer(mem), mem) + Updater.m_iItemDefinitionIndex);

			switch (myWeapon) {
				case 41 /* Knife CT */
					|| 42	/* Knife CT2 */
					|| 43	/* Flashbang */
					|| 44	/* HE grenade */
					|| 45	/* Smoke grenade */
					|| 46	/* Molotov */
					|| 47	/* Decoy */
					|| 48	/* Incendiary grenade */
					|| 49	/* C4 explosive */
					|| 57	/* Medi-shot */
					|| 59	/* Knife T */
					|| 68	/* Tactical awareness grenade */
					|| 69	/* Bare hands */
					|| 72	/* Tablet */
					|| 74	/* Knife */
					|| 75	/* Axe */
					|| 76	/* Hammer */
					|| 78	/* Wrench */
					|| 80	/* Ghost knife */
					|| 81	/* Fire bomb */
					|| 82	/* Diversion device */
					|| 83	/* Frag grenade */
					|| 84	/* Snowball */
					|| 85	/* Bump mine */
					|| 500	/* Bayonet */
					|| 503	/* Classic knife */
					|| 505	/* Flip knife */
					|| 506	/* Gut knife */
					|| 507	/* Karambit */
					|| 508	/* M9 bayonet */
					|| 509	/* Huntsman knife */
					|| 512	/* Falchion knife */
					|| 514	/* Bowie knife */
					|| 515	/* Butterfly knife */
					|| 516	/* Shadow daggers */
					|| 517	/* Paracord knife */
					|| 518	/* Survival knife */
					|| 519	/* Ursus knife */
					|| 520	/* Navaja knife */
					|| 521	/* Nomad knife */
					|| 522	/* Stiletto knife */
					|| 523	/* Talon knife */
					|| 525	/* Skeleton knife */
					|| 5027	/* Studded bloodhund gloves */
					|| 5028	/* T gloves */
					|| 5029	/* CT gloves */
					|| 5030	/* Sporty gloves */
					|| 5031	/* Slick gloves */
					|| 5032	/* Leather handwraps */
					|| 5033	/* Motorcycle gloves */
					|| 5034	/* Specialist gloves */
					|| 5035:/* Studded hydra gloves */
					return false;
				default:
					return true;
			}
		} 


		/* Final aimbot function */
		void aimbot(const CSGOMemory& mem) noexcept(true) {
			for (;;) {
				if (aimActive) {
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					aim(getClosestTarget(this->aimFov, mem), this->aimBone, this->aimSmooth, mem);
				}
				else { std::this_thread::sleep_for(std::chrono::milliseconds(1000)); }
			}
		}

		/* Aim getters */
		bool getAimActive() const noexcept(true) {
			return this->aimActive;
		}
		//
		int getAimBone() const noexcept(true) {
			return this->aimBone;
		}
		//
		int getAimSmooth() const noexcept(true) {
			return this->aimSmooth;
		}
		//
		int getAimFov() const noexcept(true) {
			return this->aimFov;
		}

		/* Aim setters */
		void setAimActive(bool value) noexcept(true) {
			this->aimActive = value;
		}
		//
		void setAimBone(int value) noexcept(true) {
			this->aimBone = value;
		}
		//
		void setAimSmooth(int value) noexcept(true) {
			if (value > 100) value = 100;
			if (value <= 0) value = 0;

			this->aimSmooth = value;
		}
		//
		void setAimFov(int value) noexcept(true) {
			if (value > 360) value = 360;
			if (value <= 0) value = 0;

			this->aimFov = value;
		}

	private:

		/* Aimbot function */
		void aim(const DWORD& player, const int& bone, const int& smooth, const CSGOMemory& mem) noexcept(true) {
			if (player != NULL && Entity.isValid(player, mem) ) {

				D3DXVECTOR3 aimAngles = Entity.getEntBonePos(player, bone, mem) - getLocalEyePos(mem);
				Vector.vectorAngles(aimAngles, aimAngles);
				auto delta = aimAngles - getLocalViewAngles(mem);
				delta = Vector.clamAngles(delta);
				auto finalAngles = (getLocalViewAngles(mem) + delta / static_cast<float>(smooth));

				if (Entity.getSpotted(player, mem)) {
					Vector.normalizeAngles(finalAngles);
					setLocalViewAngles(finalAngles, mem);
				}
				else { Entity.setSpotted(player, false, mem); }
			}
		}

		/* Aim variables */
		bool aimActive = false;
		int aimBone = 0;
		int aimSmooth = 0;
		int aimFov = 0;
		
	};
}

#endif
#pragma endregion
