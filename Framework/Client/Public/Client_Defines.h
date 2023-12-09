#pragma once

/* 클라이언트에 존재하는 모든 클래스가 인클루드해야할 내용을 담는다. */
#include <process.h>
#include "Engine_Defines.h"
namespace Client
{
	enum LAYER_TYPE
	{
		LAYER_TERRAIN,
		LAYER_SKYBOX,
		LAYER_CAMERA,
		LAYER_BACKGROUND,
		LAYER_TREE,
		LAYER_PROP,
		LAYER_GRASS,
		LAYER_ROAD,
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
		LAYER_END
	};


	enum OBJ_TYPE
	{
		OBJ_BACKGROUND,
		OBJ_BUILDING,
		OBJ_CAMERA,
		OBJ_CHARACTER,
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
		OBJ_TREE,
		OBJ_GRASS,
		OBJ_NPC,
		OBJ_END
	};

	enum LEVELID { LEVEL_STATIC, LEVEL_LOGO, LEVEL_TOOL, LEVEL_LOADING, LEVEL_DUMMY, LEVEL_END };

	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

}

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

#define SERVER_IP		L"192.168.1.33"
#define SERVER_PORT		7777
#define IMGUI_NEW_LINE ImGui::NewLine()
#define IMGUI_SAME_LINE ImGui::SameLine()

//#ifdef _DEBUG
//
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
////#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") // 디버그 모드에서 콘솔창 출력.
//
//#ifndef DBG_NEW 
//#ifndef ENGINE_EXPORTS
//#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
//#define new DBG_NEW 
//#endif
//#endif
//
//#endif // _DEBUG

using namespace Client;
