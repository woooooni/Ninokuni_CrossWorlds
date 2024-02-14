#include "stdafx.h"
#include "StelliaState_Rage3Around.h"

#include "Stellia.h"

#include "GameInstance.h"

#include "Camera_Group.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CStelliaState_Rage3Around::CStelliaState_Rage3Around(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Rage3Around::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Rage3Around::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Sprint"));
	m_fAroundTime = GI->RandomInt(6.f, 12.f);
	m_fAccAroundTime = 0.f;
	m_MotionTrailDesc.fAlphaSpeed = 200.f;
	m_pStellia->Generate_MotionTrail(m_MotionTrailDesc);

	m_vStartLook = m_pTransformCom->Get_Look();
}

void CStelliaState_Rage3Around::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	m_fAccAroundTime += fTimeDelta;

	m_pTransformCom->RevolutionRotation(Vec3(m_pStellia->Get_OriginPos()), Vec3(0.f, 1.f, 0.f), XMConvertToRadians(1.f));

	if (m_fAccAroundTime >= m_fAroundTime)
	{
		m_pStateMachineCom->Change_State(CStellia::STELLIA_RAGE3AROUND_BREAK);
	}
}

void CStelliaState_Rage3Around::Exit_State()
{
	m_pStellia->Stop_MotionTrail();
}

CStelliaState_Rage3Around* CStelliaState_Rage3Around::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Rage3Around* pInstance = new CStelliaState_Rage3Around(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Rage3Around");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Rage3Around::Free()
{
	__super::Free();
}
