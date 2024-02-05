#include "stdafx.h"
#include "StelliaState_Rage2Loop.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"


CStelliaState_Rage2Loop::CStelliaState_Rage2Loop(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage2Loop::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fRespawnTime = 4.f;

	return S_OK;
}

void CStelliaState_Rage2Loop::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_TurnStand"));

	m_pStellia->Set_StelliaHit(false);
}

void CStelliaState_Rage2Loop::Tick_State(_float fTimeDelta)
{
	//m_fTime += fTimeDelta;

	//if (m_fTime >= m_fRespawnTime)
	//{
	//	m_fTime = m_fRespawnTime - m_fTime;
	//	__super::Generate_Explosion(5);
	//}

	//if (m_pModelCom->Get_CurrAnimationFrame() >= 45 && !m_pModelCom->Is_Tween())
	//{
	//	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Start"));
	//}
}

void CStelliaState_Rage2Loop::Exit_State()
{
}

CStelliaState_Rage2Loop* CStelliaState_Rage2Loop::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage2Loop* pInstance = new CStelliaState_Rage2Loop(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage2Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage2Loop::Free()
{
	__super::Free();
}
