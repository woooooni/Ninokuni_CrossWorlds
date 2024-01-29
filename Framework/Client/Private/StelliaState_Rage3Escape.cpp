#include "stdafx.h"
#include "StelliaState_Rage3Escape.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3Escape::CStelliaState_Rage3Escape(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Escape::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_fTurnTime = 1.5f;
	m_fTurnSpeed = 2.5f;

	return S_OK;
}

void CStelliaState_Rage3Escape::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_BossSkill06_New_Loop"));
}

void CStelliaState_Rage3Escape::Tick_State(_float fTimeDelta)
{
	m_fAccTurnTime += fTimeDelta;

	// 스텔리아가 도망간다.
	if (m_fAccTurnTime < m_fTurnTime)
	{
		m_pTransformCom->Turn(Vec3(0.f, 1.f, 0.f), m_fTurnSpeed, fTimeDelta);
	}
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fSprintSpeed, fTimeDelta);


	// 스텔리아가 AroundDist에 도달한다면 다시 원점을 보게 Turn.
	Vec4 vCenterToStellia = m_pStellia->Get_OriginPos() - (Vec4)m_pTransformCom->Get_Position();
	if (fabs(vCenterToStellia.Length()) > m_fAroundDist + 5.f)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3TURN_OC);
	}
}

void CStelliaState_Rage3Escape::Exit_State()
{
}

CStelliaState_Rage3Escape* CStelliaState_Rage3Escape::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Escape* pInstance = new CStelliaState_Rage3Escape(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Escape");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Escape::Free()
{
	__super::Free();
}
