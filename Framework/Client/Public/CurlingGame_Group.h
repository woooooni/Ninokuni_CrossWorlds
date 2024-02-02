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


/* GameObjects */
#include "CurlingGame_Prop.h"
#include "CurlingGame_Stone.h"
#include "CurlingGame_Wall.h"
#include "CurlingGame_Arrow.h"
#include "CurlingGame_DeadZone.h"


/* UIs */
#include "UI_Minigame_Curling_Base.h"
#include "UI_Minigame_Curling_Gauge.h"


/* Globals */
static const enum MG_CURLING_UI_TYPE 
{ 
	GUAGE, 
	MINIGAME_CURLING_UI_TYPEEND 
};

static const wstring g_wstr_MG_Curling_Ui_ProtoTags[MINIGAME_CURLING_UI_TYPEEND] = 
{ 
	L"Prototype_GameObject_UI_Minigame_Curling_Gauge" 
};

static const wstring g_wstr_MG_Curling_Ui_ObjTags[MINIGAME_CURLING_UI_TYPEEND] =
{
	L"UI_Minigame_Curling_Gauge"
};
