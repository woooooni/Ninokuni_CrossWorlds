#pragma once


/* Manager */
#include "CurlingGame_Manager.h"


/* Manager Components */
#include "Manager_StateMachine.h"
#include "Manager_State.h"


/* Manager States */
#include "State_CurlingGame_Intro.h"
#include "State_CurlingGame_Move_Character.h"
#include "State_CurlingGame_Choose_Direction.h"
#include "State_CurlingGame_Adjust_Intensity.h"
#include "State_CurlingGame_Launch_Stone.h"
#include "State_CurlingGame_Ending.h"


/* GameObjects */
#include "CurlingGame_Prop.h"
#include "CurlingGame_Stone.h"
#include "CurlingGame_Wall.h"
#include "CurlingGame_Arrow.h"
#include "CurlingGame_DeadZone.h"


/* UIs */
#include "UI_Minigame_Curling_Base.h"
#include "UI_Minigame_Curling_Gauge.h"
#include "UI_Minigame_Curling_Info.h"
#include "UI_Minigame_Curling_Stones.h"
#include "UI_Minigame_Curling_Title.h"
#include "UI_Minigame_Curling_Score.h"
#include "UI_Minigame_Curling_TurnArrow.h"

/* Globals */
static const enum class MG_CL_UI_TYPE 
{
	GUAGE,
	INFO_NPC,
	INFO_PLAYER,
	STONES_NPC,
	STONES_PLAYER,
	TITLE,
	SCORE_NPC,
	SCORE_PLAYER,
	TURNARROW,
	TYPEEND
};

static const wstring g_wstr_MG_Curling_Ui_ProtoTags[(_uint)MG_CL_UI_TYPE::TYPEEND] =
{ 
	L"Prototype_GameObject_UI_Minigame_Curling_Gauge",
	L"Prototype_GameObject_UI_Minigame_Curling_Info_Npc",
	L"Prototype_GameObject_UI_Minigame_Curling_Info_Player",
	L"Prototype_GameObject_UI_Minigame_Curling_Stones_Npc",
	L"Prototype_GameObject_UI_Minigame_Curling_Stones_Player",
	L"Prototype_GameObject_UI_Minigame_Curling_Title",
	L"Prototype_GameObject_UI_Minigame_Curling_Score_Npc",
	L"Prototype_GameObject_UI_Minigame_Curling_Score_Player",
	L"Prototype_GameObject_UI_Minigame_Curling_TurnArrow"
};

static const wstring g_wstr_MG_Curling_Ui_ObjTags[(_uint)MG_CL_UI_TYPE::TYPEEND] =
{
	L"UI_Minigame_Curling_Gauge",
	L"UI_Minigame_Curling_Info_Npc",
	L"UI_Minigame_Curling_Info_Player",
	L"UI_Minigame_Curling_Stones_Npc",
	L"UI_Minigame_Curling_Stones_Player",
	L"UI_Minigame_Curling_Title",
	L"UI_Minigame_Curling_Score_Npc",
	L"UI_Minigame_Curling_Score_Player",
	L"UI_Minigame_Curling_TurnArrow"
};

static const _uint g_iMaxStoneCount = 5;
