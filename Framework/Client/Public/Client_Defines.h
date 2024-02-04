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

	enum SKILL_TYPE 
	{ 
		SWORDMAN_PERFECT_BLADE, SWORDMAN_SIPOHONINGLUNGE, SWORDMAN_SPINNING_ASSAULT, 
		SWORDMAN_ACANE_BARRIER, SWORDMAN_FROZEN_STORM, SWORDMAN_SWORD_TEMPEST,
		SWORDMAN_BURST_MEGA_SLASH,


		ENGINEER_BURSTCALL, ENGINEER_ELEMENTAL_BLAST, ENGINEER_EXPLOSIONSHOT,
		ENGINEER_FLASH_HEAL, ENGINEER_TIMELAB, ENGINEER_HEALINGTREE,
		ENGINNER_BURST_DESTRUCTION,


		DESTROYER_WHEELWIND, DESTROYER_BRUTALSTRIKE, DESTROYER_LEAFSLAM,
		DESTROYER_BATTLE_CRY, DESTROYER_IGNORE_PAIN, DESTROYER_FRENGE_CHARGE,
		DESTROYER_BURST_HYPERSTRIKE,

		// 그랑프리용 스킬(Temp)
		FLYING_MUCKCLOUD, FLYING_RUSH, FLYING_GUIDED_MISSILE, FLYING_BURST,
		SKILL_END
	};

	/* SwordMan */
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

	enum LOAD_CHARACTER_TYPE { SWORDMAN_CH, DESTROYER_CH, ENGINEER_CH, ALL_CH };

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


	enum TRIGGER_TYPE { TRIGGER_CHANGE_BGM, TRIGGER_MAP_NAME, TRIGGER_BOSS_GIANTY_ENTER, TRIGGER_BOSS_STELLIA_ENTER, TRIGGER_WHALE_ENTER, TRIGGER_WITCH_WOOD_ENTER, TRIGGER_WITCH_ESCORT1, TRIGGER_WITCH_ESCORT2, TRIGGER_END };

	
	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_TEST, LEVEL_LOADING,
		LEVEL_LOBBY, LEVEL_EVERMORE, LEVEL_KINGDOMHALL, LEVEL_ICELAND, LEVEL_WITCHFOREST,
		LEVEL_DUMMY, LEVEL_COSTUMEROOM, LEVEL_PARTS = 98, LEVEL_TOOL = 99, LEVEL_END };

	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;


	/* Camera - Default */

	enum CAMERA_TYPE 
	{ 
		FREE, 
		FOLLOW, 
		CUTSCENE_MAP, 
		CUTSCENE_BOSS, 
		ACTION, 
		TOP, 
		QUATER, 
		CAMERA_CURLING,
		CAMERA_TYPE_END 
	};

	enum CAMERA_VIEW_TYPE { BACK, SHOLDER };

	static const char* CameraCharNames[CAMERA_TYPE::CAMERA_TYPE_END]
	{ 
		"Camera_Free", 
		"Camera_Follow", 
		"Camera_CutScene_Map", 
		"Camera_CutScene_Boss", 
		"Camera_Action", 
		"Camera_Top", 
		"Camera_Quater",
		"Camera_CurlingGame"
	};
	
	const wstring CameraWstringNames[CAMERA_TYPE::CAMERA_TYPE_END]
	{ 
		L"Camera_Free", 
		L"Camera_Follow", 
		L"Camera_CutScene_Map", 
		L"Camera_CutScene_Boss", 
		L"Camera_Action",
		L"Camera_Top",
		L"Camera_Quater",
		L"Camera_CurlingGame"
	};

	static const _float Cam_Follow_Limit_Min_Y = 1.1f;
	static const _float Cam_Follow_Limit_Max_Y = 1.8f;

	static const _float Cam_Dist_Follow_Default					= 4.5f;
	static const _float Cam_Dist_Follow_Wide					= 5.5f;

	static const _float Cam_Dist_Quater_Min						= 15.f;
	static const _float Cam_Dist_Quater_Default					= 30.f;
	static const _float Cam_Dist_Quater_Max						= 35.f;

	static const _float Cam_Fov_Default						= XMConvertToRadians(60.0f);
	static const _float Cam_Fov_Free_Default				= XMConvertToRadians(60.0f);
	static const _float Cam_Fov_Follow_Default				= XMConvertToRadians(60.0f);
	static const _float Cam_Fov_Follow_Wide					= XMConvertToRadians(75.0f);
	static const _float Cam_Fov_Action_Lobby				= XMConvertToRadians(52.5f);
	static const _float Cam_Fov_Action_Talk_SuperNarrow		= XMConvertToRadians(45.f);
	static const _float Cam_Fov_Action_Talk_Narrow			= XMConvertToRadians(50.f);
	static const _float Cam_Fov_Action_Talk_Default			= XMConvertToRadians(60.f);
	static const _float Cam_Fov_CutScene_Map_Default		= XMConvertToRadians(70.0f);
	static const _float Cam_Fov_CutScene_Boss_Default		= XMConvertToRadians(70.0f);
	static const _float Cam_Fov_Top_Glanix					= XMConvertToRadians(75.0f);
	static const _float Cam_Fov_Quater_Default				= XMConvertToRadians(70.f);
	static const _float Cam_Fov_CurlingGame_Default			= XMConvertToRadians(45.0f);
	static const _float Cam_Fov_CurlingGame_Damping			= XMConvertToRadians(50.0f);
	static const _float Cam_Fov_CurlingGame_AfterDamping	= XMConvertToRadians(60.0f);


	static const Vec4	Cam_TargetOffset_Follow_SholderView_Default		= { 0.7f, 1.3f, 0.f, 1.f };
	static const Vec4	Cam_LookAtOffset_Follow_SholderView_Default		= { 0.7f, 1.3f, 0.f, 1.f };

	static const Vec4	Cam_TargetOffset_Follow_BackView_Default = { 0.f, 1.3f, 0.f, 1.f };
	static const Vec4	Cam_LookAtOffset_Follow_BackView_Default = { 0.f, 1.3f, 0.f, 1.f };

	static const Vec4	Cam_TargetOffset_CurlingGame_Default		= { 0.f, 2.f, -8.5f, 1.f }; 	   // 시작 뷰 { 0.f, 4.f, -9.5f, 1.f };
	static const Vec4	Cam_TargetOffset_CurlingGame_AfterDamping	= { 0.f, 3.f, -14.f, 1.f };
	static const Vec4	Cam_LookAtOffset_CurlingGame_Default		= { 0.f, 0.f, 0.f, 1.f };		   // 시작 뷰 { 0.f, 0.f, 0.f, 1.f };	

	static const Vec4	Cam_TargetOffset_Top_Glanix = { -20.f, 15.f, 0.f, 1.f };
	static const Vec4	Cam_TargetOffset_Top_Glanix_Zoom = { -13.f, 8.f, 0.f, 1.f };
	static const Vec4	Cam_LookAtOffset_Top_Glanix = { 0.f, 0.f, 0.f, 1.f };
	static const Vec4	Cam_LookAtOffset_Top_Glanix_Zoom = { 5.f, 0.f, 0.f, 1.f };

#define MAX_BEZIER_POINT 4

	/* Lock On - Glanix */

	/* Default */
	static const _float Cam_LockOn_Time_BlendingIn	= 0.f; 
	static const _float Cam_LockOn_Time_BlendingOut = 0.f;

	static const Vec4	Cam_Target_Offset_LockOn_Glanix		= Vec4{ 0.7f, 2.f, -6.f, 1.f };
	static const Vec4	Cam_LookAt_Offset_LockOn_Glanix		= Vec4{ 0.f, 0.5f, 0.f, 1.f };

	/* Item Pattern */
	static const _float Cam_LerpTime_LockOn_Glanix_ItemPattern_Blending_In = 0.8f;
	static const _float Cam_LerpTime_LockOn_Glanix_ItemPattern_Blending_Out = 0.5f;
	static const _float Cam_Fov_LockOn_Glanix_ItemPattern = XMConvertToRadians(75.0f);
	static const Vec4	Cam_Target_Offset_LockOn_Glanix_ItemPattern = Vec4{ 0.f, 4.5f, -10.f, 1.f };

	const Vec2 g_vWinCenter = { 800.f, 450.f };

	/* Bgm Sound */

#define BGM_START_FADEIN_DURATION 3.f
#define BGM_STOP_FADEOUT_DURATION 2.f

	enum ITEM_TYPE { ARMOR, EQUIPMENT, CONSUMPSION, COIN, ITEMTYPE_END };
}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;
extern _uint g_eLoadCharacter;
extern _uint g_ePlayCharacter;
extern _uint g_eStartLevel;
extern _uint g_iStartQuestLevel;


#define SERVER_IP		L"192.168.1.33"
#define SERVER_PORT		7777
#define IMGUI_NEW_LINE ImGui::NewLine()
#define IMGUI_SAME_LINE ImGui::SameLine()


using namespace Client;
