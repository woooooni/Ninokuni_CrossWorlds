#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>
#include "Engine_Defines.h"
namespace Client
{
	enum CHARACTER_TYPE { SWORD_MAN, DESTROYER, ENGINEER, CHARACTER_END };
	static const wstring wstrCharacterTypeNames[CHARACTER_TYPE::CHARACTER_END]{ L"SWORDMAN", L"DESTROYER", L"ENGINEER" };
	
	enum PART_TYPE { HEAD, HAIR, FACE, BODY, PART_END };
	static const wstring wstrPartTypeNames[PART_TYPE::PART_END]{ L"HEAD", L"HAIR", L"FACE", L"BODY" };

	enum WEAPON_TYPE { SWORD, HAMMER, RIFLE, WEAPON_END };

	/* ShadowMan */
	enum SWORDMAN_HEAD_MODEL_TYPE { HEAD_TEMP, SWORDMAN_HEAD_MODEL_TYPE_END };
	static const wstring wstrSwordManHeadTypeNames[SWORDMAN_HEAD_MODEL_TYPE::SWORDMAN_HEAD_MODEL_TYPE_END]{ L"HEAD_TEMP" };

	enum SWORDMAN_HAIR_MODEL_TYPE { HAIR_TEMP, SWORDMAN_HAIR_MODEL_TYPE_END };
	static const wstring wstrSwordManHairTypeNames[SWORDMAN_HAIR_MODEL_TYPE::SWORDMAN_HAIR_MODEL_TYPE_END]{ L"HAIR_TEMP" };

	enum SWORDMAN_FACE_MODEL_TYPE { FACE_TEMP, SWORDMAN_FACE_MODEL_TYPE_END };
	static const wstring wstrSwordManFaceTypeNames[SWORDMAN_FACE_MODEL_TYPE::SWORDMAN_FACE_MODEL_TYPE_END]{ L"FACE_TEMP" };

	enum SWORDMAN_BODY_MODEL_TYPE { BODY_TEMP, SWORDMAN_BODY_MODEL_TYPE_END };
	static const wstring wstrSwordManBodyTypeNames[SWORDMAN_BODY_MODEL_TYPE::SWORDMAN_BODY_MODEL_TYPE_END]{ L"BODY_TEMP" };
	

	enum DESTROYER_HEAD_MODEL_TYPE { };
	enum DESTROYER_HAIR_MODEL_TYPE { };
	enum DESTROYER_FACE_MODEL_TYPE { };
	enum DESTROYER_BODY_MODEL_TYPE { };

	enum ENGINEER_HEAD_MODEL_TYPE { };
	enum ENGINEER_HAIR_MODEL_TYPE { };
	enum ENGINEER_FACE_MODEL_TYPE { };
	enum ENGINEER_BODY_MODEL_TYPE { };

	enum WITCH_HEAD_MODEL_TYPE { };
	enum WITCH_HAIR_MODEL_TYPE { };
	enum WITCH_FACE_MODEL_TYPE { };
	enum WITCH_BODY_MODEL_TYPE { };
	
	enum LAYER_TYPE
	{
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_TREEROCK,
		LAYER_PROP,
		LAYER_GRASS,
		LAYER_WATER,
		LAYER_BUILDING,
		LAYER_MOUNTAIN,
		LAYER_PLAYER,
		LAYER_CHARACTER,
		LAYER_MONSTER,
		LAYER_NPC,
		LAYER_PROJECTILE,
		LAYER_EFFECT,
		LAYER_GROUND,
		LAYER_TRAIL,
		LAYER_UI,
		LAYER_ETC,
		LAYER_WEAPON,
		LAYER_DYNAMIC,
		LAYER_END
	};


	enum OBJ_TYPE
	{
		OBJ_BACKGROUND,
		OBJ_BUILDING,
		OBJ_CAMERA,
		OBJ_CHARACTER,
		OBJ_CHARACTER_PROJECTILE,
		OBJ_CHARACTER_SKILL,
		OBJ_WEAPON,
		OBJ_PLAYER,
		OBJ_MONSTER,
		OBJ_PROP,
		OBJ_EFFECT,
		OBJ_PARTICLE,
		OBJ_TERRAIN,
		OBJ_UI,
		OBJ_DUMMY,
		OBJ_GROUND,
		OBJ_TRAIL,
		OBJ_TREEROCK,
		OBJ_GRASS,
		OBJ_NPC,
		OBJ_DYNAMIC,
		OBJ_SPAWNER,
		OBJ_END
	};

	
	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_TEST, LEVEL_LOADING,
		LEVEL_LOBBY, LEVEL_EVERMORE, LEVEL_KINGDOMHALL, LEVEL_ICELAND, LEVEL_WITCHFOREST,
		LEVEL_DUMMY, LEVEL_PARTS = 98, LEVEL_TOOL = 99, LEVEL_END };

	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;


	/* Camera */

	static const _float Cam_Dist_Follow_Default = 3.2f;

	static const _float Cam_Fov_Free_Default = XMConvertToRadians(60.0f);
	static const _float Cam_Fov_Follow_Default = XMConvertToRadians(60.0f);
	static const _float Cam_Fov_CutScene_Default = XMConvertToRadians(70.0f);

	enum CAMERA_TYPE { FREE, FOLLOW, CUTSCENE, CAMERA_TYPE_END };

	const wstring		CameraWstringNames[CAMERA_TYPE::CAMERA_TYPE_END]{ L"Camera_Free", L"Camera_Follow", L"Camera_CutScene"};
	
	static const char*	CameraCharNames[CAMERA_TYPE::CAMERA_TYPE_END]{ "Camera_Free", "Camera_Follow", "Camera_CutScene" };

	enum CAMERA_EVENT_TYPE { LERP_FOV, LERP_DIST, SHAKE, CAMERA_EVENT_TYPE_END };

#define MAX_BEZIER_POINT 4

	typedef struct tagCameraEventDesc
	{					
								/* LERP_FOV,		LERP_DIST,		SHAKE */
		_float fTag1 = 0.f;		/* fTargetValue,	fTargetValue,	fAmplitude*/
		_float fTag2 = 0.f;		/* fTime,			fTime,			fFrequency */
		_float fTag3 = 0.f;		/*		,				,			fDuration */				
		_float fTag4 = 0.f;
		
		void* ptr1	= nullptr;
		void* ptr2	= nullptr;

		_int iTag1 = 0.f;		/* eMode */
		_int iTag2 = 0.f;

	}CAMERA_EVENT_DESC;

	typedef struct tagCameraCutScenePathDesc
	{
		Vec3 vCamPositions[MAX_BEZIER_POINT];
		Vec3 vCamLookAts[MAX_BEZIER_POINT];

		_float		fDuration;
		LERP_MODE	eLerpMode;

		LERP_TIME_DESC	m_tTimeDesc;

		void Start()
		{
			m_tTimeDesc.Start(fDuration, eLerpMode);
		}

		void Update(const _float fTimeDelta)
		{
			m_tTimeDesc.Update(fTimeDelta);
		}

	}CAMERA_CUSTCENE_PATH_DESC;

	typedef struct tagCameraCutSceneDesc
	{
		vector<CAMERA_CUSTCENE_PATH_DESC> tPaths;

		_int iCurPathIndex = -1;

		_bool bPlay = false;

		void Start()
		{
			iCurPathIndex = 0;

			bPlay = true;

			tPaths[iCurPathIndex].Start();
		}

		void Update(const _float fTimeDelta)
		{
			if (!bPlay)
				return;

			if (!tPaths[iCurPathIndex].m_tTimeDesc.bActive)
			{
				if (iCurPathIndex < tPaths.size() - 1)
				{
					iCurPathIndex++;
					tPaths[iCurPathIndex].m_tTimeDesc.Start(tPaths[iCurPathIndex].fDuration, tPaths[iCurPathIndex].eLerpMode);
				}
				else
				{
					bPlay = false;
					return;
				}
			}
		
			tPaths[iCurPathIndex].Update(fTimeDelta);
		}

	}CAMERA_CUTSCENE_DESC;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

#define SERVER_IP		L"192.168.1.33"
#define SERVER_PORT		7777
#define IMGUI_NEW_LINE ImGui::NewLine()
#define IMGUI_SAME_LINE ImGui::SameLine()


using namespace Client;
