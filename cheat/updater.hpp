#pragma once

#define CURL_STATICLIB
#include <curl\curl.h>

#include <Windows.h>
#include <vector>
#include <regex>
#include <thread>
#include <string>


/* Offsets updater

   This is done, to put it mildly, not in the best way, but it works :)
   This was done a long time ago, and I have no desire to improve it

   All offsets are here to make life easier for other developers

*/


#pragma region updater
#ifndef updaterDefine
#define updaterDefine

namespace updater
{
	class Updater
	{
	private:
		/* Variables */
		CURL* curl;
		CURLcode res;

		std::string readBuffer;
		std::string offsets;
	private:

		/* Offset parsing functions */
		static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) noexcept(true) {
			((std::string*)userp)->append((char*)contents, size * nmemb);
			return size * nmemb;
		}
		//
		std::string downloadOffsets() noexcept(true) {
			if (curl) {
				curl_easy_setopt(curl, CURLOPT_URL, "https://raw.githubusercontent.com/frk1/hazedumper/master/csgo.hpp");
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
				res = curl_easy_perform(curl);
			}
			return readBuffer;
		}

	private:

		/* Date regular */
		std::vector<std::string> dateRegular() noexcept(true) {
			std::regex dateRegex(R"(\w*-\w*-\w*)");
			std::sregex_token_iterator first{ offsets.begin(), offsets.end(), dateRegex, 0 }, last;

			return { first, last };
		}

		/* First regular */
		std::vector<std::string> firstRegular() noexcept(true) {
			std::regex firstRegex("([^ ]+);");
			std::sregex_token_iterator first{ offsets.begin(), offsets.end(), firstRegex, 0 }, last;

			return { first, last };
		}

		/* Second regular */
		std::vector<std::string> secondRegular() noexcept(true) {
			std::regex secondRegex(R"(\d\w\d*\w*\b)");
			std::string offsets;
			for (auto const& o : firstRegular()) { offsets += o; }

			std::sregex_token_iterator first{ offsets.begin(), offsets.end(), secondRegex, 0 }, last;

			return { first, last };
		}

		/* Final offsets array */
		inline static std::vector<std::string> offsetsArray;

	private:

		/* Conversion functions */
		DWORD stringToInt64_t(std::string str) noexcept(true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			return std::strtoll(str.c_str(), NULL, 10);
		}
		//
		DWORD stringToDword(std::string str) noexcept(true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			return std::strtoul(str.c_str(), NULL, 16);
		}

	public:

		/* Getting the date of the last update */
		[[nodiscard]] std::string updateDate_string() noexcept(true) {
			return dateRegular()[0];
		}

	public:

		/* Constructor and destructor */
		Updater() {

			curl = curl_easy_init();

			offsets = downloadOffsets();

			offsetsArray = secondRegular();

			timestamp = stringToInt64_t(offsetsArray[0]);
			cs_gamerules_data = stringToDword(offsetsArray[1]);
			m_ArmorValue = stringToDword(offsetsArray[2]);
			m_Collision = stringToDword(offsetsArray[3]);
			m_CollisionGroup = stringToDword(offsetsArray[4]);
			m_Local = stringToDword(offsetsArray[5]);
			m_MoveType = stringToDword(offsetsArray[6]);
			m_OriginalOwnerXuidHigh = stringToDword(offsetsArray[7]);
			m_OriginalOwnerXuidLow = stringToDword(offsetsArray[8]);
			m_SurvivalGameRuleDecisionTypes = stringToDword(offsetsArray[9]);
			m_SurvivalRules = stringToDword(offsetsArray[10]);
			m_aimPunchAngle = stringToDword(offsetsArray[11]);
			m_aimPunchAngleVel = stringToDword(offsetsArray[12]);
			m_angEyeAnglesX = stringToDword(offsetsArray[13]);
			m_angEyeAnglesY = stringToDword(offsetsArray[14]);
			m_bBombDefused = stringToDword(offsetsArray[15]);
			m_bBombPlanted = stringToDword(offsetsArray[16]);
			m_bBombTicking = stringToDword(offsetsArray[17]);
			m_bFreezePeriod = stringToDword(offsetsArray[18]);
			m_bGunGameImmunity = stringToDword(offsetsArray[19]);
			m_bHasDefuser = stringToDword(offsetsArray[20]);
			m_bHasHelmet = stringToDword(offsetsArray[21]);
			m_bInReload = stringToDword(offsetsArray[22]);
			m_bIsDefusing = stringToDword(offsetsArray[23]);
			m_bIsQueuedMatchmaking = stringToDword(offsetsArray[24]);
			m_bIsScoped = stringToDword(offsetsArray[25]);
			m_bIsValveDS = stringToDword(offsetsArray[26]);
			m_bSpotted = stringToDword(offsetsArray[27]);
			m_bSpottedByMask = stringToDword(offsetsArray[28]);
			m_bStartedArming = stringToDword(offsetsArray[29]);
			m_bUseCustomAutoExposureMax = stringToDword(offsetsArray[30]);
			m_bUseCustomAutoExposureMin = stringToDword(offsetsArray[31]);
			m_bUseCustomBloomScale = stringToDword(offsetsArray[32]);
			m_clrRender = stringToDword(offsetsArray[33]);
			m_dwBoneMatrix = stringToDword(offsetsArray[34]);
			m_fAccuracyPenalty = stringToDword(offsetsArray[35]);
			m_fFlags = stringToDword(offsetsArray[36]);
			m_flC4Blow = stringToDword(offsetsArray[37]);
			m_flCustomAutoExposureMax = stringToDword(offsetsArray[38]);
			m_flCustomAutoExposureMin = stringToDword(offsetsArray[39]);
			m_flCustomBloomScale = stringToDword(offsetsArray[40]);
			m_flDefuseCountDown = stringToDword(offsetsArray[41]);
			m_flDefuseLength = stringToDword(offsetsArray[42]);
			m_flFallbackWear = stringToDword(offsetsArray[43]);
			m_flFlashDuration = stringToDword(offsetsArray[44]);
			m_flFlashMaxAlpha = stringToDword(offsetsArray[45]);
			m_flLastBoneSetupTime = stringToDword(offsetsArray[46]);
			m_flLowerBodyYawTarget = stringToDword(offsetsArray[47]);
			m_flNextAttack = stringToDword(offsetsArray[48]);
			m_flNextPrimaryAttack = stringToDword(offsetsArray[49]);
			m_flSimulationTime = stringToDword(offsetsArray[50]);
			m_flTimerLength = stringToDword(offsetsArray[51]);
			m_hActiveWeapon = stringToDword(offsetsArray[52]);
			m_hBombDefuser = stringToDword(offsetsArray[53]);
			m_hMyWeapons = stringToDword(offsetsArray[54]);
			m_hObserverTarget = stringToDword(offsetsArray[55]);
			m_hOwner = stringToDword(offsetsArray[56]);
			m_hOwnerEntity = stringToDword(offsetsArray[57]);
			m_hViewModel = stringToDword(offsetsArray[58]);
			m_iAccountID = stringToDword(offsetsArray[59]);
			m_iClip1 = stringToDword(offsetsArray[60]);
			m_iCompetitiveRanking = stringToDword(offsetsArray[61]);
			m_iCompetitiveWins = stringToDword(offsetsArray[62]);
			m_iCrosshairId = stringToDword(offsetsArray[63]);
			m_iDefaultFOV = stringToDword(offsetsArray[64]);
			m_iEntityQuality = stringToDword(offsetsArray[65]);
			m_iFOV = stringToDword(offsetsArray[66]);
			m_iFOVStart = stringToDword(offsetsArray[67]);
			m_iGlowIndex = stringToDword(offsetsArray[68]);
			m_iHealth = stringToDword(offsetsArray[69]);
			m_iItemDefinitionIndex = stringToDword(offsetsArray[70]);
			m_iItemIDHigh = stringToDword(offsetsArray[71]);
			m_iMostRecentModelBoneCounter = stringToDword(offsetsArray[72]);
			m_iObserverMode = stringToDword(offsetsArray[73]);
			m_iShotsFired = stringToDword(offsetsArray[74]);
			m_iState = stringToDword(offsetsArray[75]);
			m_iTeamNum = stringToDword(offsetsArray[76]);
			m_lifeState = stringToDword(offsetsArray[77]);
			m_nBombSite = stringToDword(offsetsArray[78]);
			m_nFallbackPaintKit = stringToDword(offsetsArray[79]);
			m_nFallbackSeed = stringToDword(offsetsArray[80]);
			m_nFallbackStatTrak = stringToDword(offsetsArray[81]);
			m_nForceBone = stringToDword(offsetsArray[82]);
			m_nTickBase = stringToDword(offsetsArray[83]);
			m_nViewModelIndex = stringToDword(offsetsArray[84]);
			m_rgflCoordinateFrame = stringToDword(offsetsArray[85]);
			m_szCustomName = stringToDword(offsetsArray[86]);
			m_szLastPlaceName = stringToDword(offsetsArray[87]);
			m_thirdPersonViewAngles = stringToDword(offsetsArray[88]);
			m_vecOrigin = stringToDword(offsetsArray[89]);
			m_vecVelocity = stringToDword(offsetsArray[90]);
			m_vecViewOffset = stringToDword(offsetsArray[91]);
			m_viewPunchAngle = stringToDword(offsetsArray[92]);
			m_zoomLevel = stringToDword(offsetsArray[93]);
			anim_overlays = stringToDword(offsetsArray[94]);
			clientstate_choked_commands = stringToDword(offsetsArray[95]);
			clientstate_delta_ticks = stringToDword(offsetsArray[96]);
			clientstate_last_outgoing_command = stringToDword(offsetsArray[97]);
			clientstate_net_channel = stringToDword(offsetsArray[98]);
			convar_name_hash_table = stringToDword(offsetsArray[99]);
			dwClientState = stringToDword(offsetsArray[100]);
			dwClientState_GetLocalPlayer = stringToDword(offsetsArray[101]);
			dwClientState_IsHLTV = stringToDword(offsetsArray[102]);
			dwClientState_Map = stringToDword(offsetsArray[103]);
			dwClientState_MapDirectory = stringToDword(offsetsArray[104]);
			dwClientState_MaxPlayer = stringToDword(offsetsArray[105]);
			dwClientState_PlayerInfo = stringToDword(offsetsArray[106]);
			dwClientState_State = stringToDword(offsetsArray[107]);
			dwClientState_ViewAngles = stringToDword(offsetsArray[108]);
			dwEntityList = stringToDword(offsetsArray[109]);
			dwForceAttack = stringToDword(offsetsArray[110]);
			dwForceAttack2 = stringToDword(offsetsArray[111]);
			dwForceBackward = stringToDword(offsetsArray[112]);
			dwForceForward = stringToDword(offsetsArray[113]);
			dwForceJump = stringToDword(offsetsArray[114]);
			dwForceLeft = stringToDword(offsetsArray[115]);
			dwForceRight = stringToDword(offsetsArray[116]);
			dwGameDir = stringToDword(offsetsArray[117]);
			dwGameRulesProxy = stringToDword(offsetsArray[118]);
			dwGetAllClasses = stringToDword(offsetsArray[119]);
			dwGlobalVars = stringToDword(offsetsArray[120]);
			dwGlowObjectManager = stringToDword(offsetsArray[121]);
			dwInput = stringToDword(offsetsArray[122]);
			dwInterfaceLinkList = stringToDword(offsetsArray[123]);
			dwLocalPlayer = stringToDword(offsetsArray[124]);
			dwMouseEnable = stringToDword(offsetsArray[125]);
			dwMouseEnablePtr = stringToDword(offsetsArray[126]);
			dwPlayerResource = stringToDword(offsetsArray[127]);
			dwRadarBase = stringToDword(offsetsArray[128]);
			dwSensitivity = stringToDword(offsetsArray[129]);
			dwSensitivityPtr = stringToDword(offsetsArray[130]);
			dwSetClanTag = stringToDword(offsetsArray[131]);
			dwViewMatrix = stringToDword(offsetsArray[132]);
			dwWeaponTable = stringToDword(offsetsArray[133]);
			dwWeaponTableIndex = stringToDword(offsetsArray[134]);
			dwYawPtr = stringToDword(offsetsArray[135]);
			dwZoomSensitivityRatioPtr = stringToDword(offsetsArray[136]);
			dwbSendPackets = stringToDword(offsetsArray[137]);
			dwppDirect3DDevice9 = stringToDword(offsetsArray[138]);
			find_hud_element = stringToDword(offsetsArray[139]);
			force_update_spectator_glow = stringToDword(offsetsArray[140]);
			interface_engine_cvar = stringToDword(offsetsArray[141]);
			is_c4_owner = stringToDword(offsetsArray[142]);
			m_bDormant = stringToDword(offsetsArray[143]);
			m_flSpawnTime = stringToDword(offsetsArray[144]);
			m_pStudioHdr = stringToDword(offsetsArray[145]);
			m_pitchClassPtr = stringToDword(offsetsArray[146]);
			m_yawClassPtr = stringToDword(offsetsArray[147]);
			model_ambient_min = stringToDword(offsetsArray[148]);
			set_abs_angles = stringToDword(offsetsArray[149]);
			set_abs_origin = stringToDword(offsetsArray[150]);
		}
		//
		~Updater() {
			curl_easy_cleanup(curl);
		}

	public:


		/* All offsets */
		inline static int64_t timestamp;

		inline static DWORD cs_gamerules_data;
		inline static DWORD m_ArmorValue;
		inline static DWORD m_Collision;
		inline static DWORD m_CollisionGroup;
		inline static DWORD m_Local;
		inline static DWORD m_MoveType;
		inline static DWORD m_OriginalOwnerXuidHigh;
		inline static DWORD m_OriginalOwnerXuidLow;
		inline static DWORD m_SurvivalGameRuleDecisionTypes;
		inline static DWORD m_SurvivalRules;
		inline static DWORD m_aimPunchAngle;
		inline static DWORD m_aimPunchAngleVel;
		inline static DWORD m_angEyeAnglesX;
		inline static DWORD m_angEyeAnglesY;
		inline static DWORD m_bBombDefused;
		inline static DWORD m_bBombPlanted;
		inline static DWORD m_bBombTicking;
		inline static DWORD m_bFreezePeriod;
		inline static DWORD m_bGunGameImmunity;
		inline static DWORD m_bHasDefuser;
		inline static DWORD m_bHasHelmet;
		inline static DWORD m_bInReload;
		inline static DWORD m_bIsDefusing;
		inline static DWORD m_bIsQueuedMatchmaking;
		inline static DWORD m_bIsScoped;
		inline static DWORD m_bIsValveDS;
		inline static DWORD m_bSpotted;
		inline static DWORD m_bSpottedByMask;
		inline static DWORD m_bStartedArming;
		inline static DWORD m_bUseCustomAutoExposureMax;
		inline static DWORD m_bUseCustomAutoExposureMin;
		inline static DWORD m_bUseCustomBloomScale;
		inline static DWORD m_clrRender;
		inline static DWORD m_dwBoneMatrix;
		inline static DWORD m_fAccuracyPenalty;
		inline static DWORD m_fFlags;
		inline static DWORD m_flC4Blow;
		inline static DWORD m_flCustomAutoExposureMax;
		inline static DWORD m_flCustomAutoExposureMin;
		inline static DWORD m_flCustomBloomScale;
		inline static DWORD m_flDefuseCountDown;
		inline static DWORD m_flDefuseLength;
		inline static DWORD m_flFallbackWear;
		inline static DWORD m_flFlashDuration;
		inline static DWORD m_flFlashMaxAlpha;
		inline static DWORD m_flLastBoneSetupTime;
		inline static DWORD m_flLowerBodyYawTarget;
		inline static DWORD m_flNextAttack;
		inline static DWORD m_flNextPrimaryAttack;
		inline static DWORD m_flSimulationTime;
		inline static DWORD m_flTimerLength;
		inline static DWORD m_hActiveWeapon;
		inline static DWORD m_hBombDefuser;
		inline static DWORD m_hMyWeapons;
		inline static DWORD m_hObserverTarget;
		inline static DWORD m_hOwner;
		inline static DWORD m_hOwnerEntity;
		inline static DWORD m_hViewModel;
		inline static DWORD m_iAccountID;
		inline static DWORD m_iClip1;
		inline static DWORD m_iCompetitiveRanking;
		inline static DWORD m_iCompetitiveWins;
		inline static DWORD m_iCrosshairId;
		inline static DWORD m_iDefaultFOV;
		inline static DWORD m_iEntityQuality;
		inline static DWORD m_iFOV;
		inline static DWORD m_iFOVStart;
		inline static DWORD m_iGlowIndex;
		inline static DWORD m_iHealth;
		inline static DWORD m_iItemDefinitionIndex;
		inline static DWORD m_iItemIDHigh;
		inline static DWORD m_iMostRecentModelBoneCounter;
		inline static DWORD m_iObserverMode;
		inline static DWORD m_iShotsFired;
		inline static DWORD m_iState;
		inline static DWORD m_iTeamNum;
		inline static DWORD m_lifeState;
		inline static DWORD m_nBombSite;
		inline static DWORD m_nFallbackPaintKit;
		inline static DWORD m_nFallbackSeed;
		inline static DWORD m_nFallbackStatTrak;
		inline static DWORD m_nForceBone;
		inline static DWORD m_nTickBase;
		inline static DWORD m_nViewModelIndex;
		inline static DWORD m_rgflCoordinateFrame;
		inline static DWORD m_szCustomName;
		inline static DWORD m_szLastPlaceName;
		inline static DWORD m_thirdPersonViewAngles;
		inline static DWORD m_vecOrigin;
		inline static DWORD m_vecVelocity;
		inline static DWORD m_vecViewOffset;
		inline static DWORD m_viewPunchAngle;
		inline static DWORD m_zoomLevel;
		inline static DWORD anim_overlays;
		inline static DWORD clientstate_choked_commands;
		inline static DWORD clientstate_delta_ticks;
		inline static DWORD clientstate_last_outgoing_command;
		inline static DWORD clientstate_net_channel;
		inline static DWORD convar_name_hash_table;
		inline static DWORD dwClientState;
		inline static DWORD dwClientState_GetLocalPlayer;
		inline static DWORD dwClientState_IsHLTV;
		inline static DWORD dwClientState_Map;
		inline static DWORD dwClientState_MapDirectory;
		inline static DWORD dwClientState_MaxPlayer;
		inline static DWORD dwClientState_PlayerInfo;
		inline static DWORD dwClientState_State;
		inline static DWORD dwClientState_ViewAngles;
		inline static DWORD dwEntityList;
		inline static DWORD dwForceAttack;
		inline static DWORD dwForceAttack2;
		inline static DWORD dwForceBackward;
		inline static DWORD dwForceForward;
		inline static DWORD dwForceJump;
		inline static DWORD dwForceLeft;
		inline static DWORD dwForceRight;
		inline static DWORD dwGameDir;
		inline static DWORD dwGameRulesProxy;
		inline static DWORD dwGetAllClasses;
		inline static DWORD dwGlobalVars;
		inline static DWORD dwGlowObjectManager;
		inline static DWORD dwInput;
		inline static DWORD dwInterfaceLinkList;
		inline static DWORD dwLocalPlayer;
		inline static DWORD dwMouseEnable;
		inline static DWORD dwMouseEnablePtr;
		inline static DWORD dwPlayerResource;
		inline static DWORD dwRadarBase;
		inline static DWORD dwSensitivity;
		inline static DWORD dwSensitivityPtr;
		inline static DWORD dwSetClanTag;
		inline static DWORD dwViewMatrix;
		inline static DWORD dwWeaponTable;
		inline static DWORD dwWeaponTableIndex;
		inline static DWORD dwYawPtr;
		inline static DWORD dwZoomSensitivityRatioPtr;
		inline static DWORD dwbSendPackets;
		inline static DWORD dwppDirect3DDevice9;
		inline static DWORD find_hud_element;
		inline static DWORD force_update_spectator_glow;
		inline static DWORD interface_engine_cvar;
		inline static DWORD is_c4_owner;
		inline static DWORD m_bDormant;
		inline static DWORD m_flSpawnTime;
		inline static DWORD m_pStudioHdr;
		inline static DWORD m_pitchClassPtr;
		inline static DWORD m_yawClassPtr;
		inline static DWORD model_ambient_min;
		inline static DWORD set_abs_angles;
		inline static DWORD set_abs_origin;
	};
}

#endif
#pragma endregion