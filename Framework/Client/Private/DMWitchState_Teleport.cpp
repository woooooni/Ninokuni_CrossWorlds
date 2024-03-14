#include "stdafx.h"
#include "DMWitchState_Teleport.h"

#include "DMWitch.h"

CDMWitchState_Teleport::CDMWitchState_Teleport(CStateMachine* pStateMachine)
	: CDMWitchState_Base(pStateMachine)
{
}

HRESULT CDMWitchState_Teleport::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CDMWitchState_Teleport::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk"));
}

void CDMWitchState_Teleport::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_pPlayer != nullptr)
		m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());

	// 일정 프레임 이상일 때 셋 트랜스폼 시켜서 플레이어 위치 뒤로 이동시키자.
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_fRunSpeed, fTimeDelta);

	if (m_pWitch->Get_Bools(CBoss::BOSS_BOOLTYPE::BOSSBOOL_ATKAROUND))
	{
		if (m_iAtkIndex >= m_vecAtkState.size())
			m_iAtkIndex = 0;

		m_pStateMachineCom->Change_State(m_vecAtkState[m_iAtkIndex++]);
	}
	else
	{
		m_pStateMachineCom->Change_State(CDMWitch::DMWITCH_TURN);
	}
}

void CDMWitchState_Teleport::Exit_State()
{
}

CDMWitchState_Teleport* CDMWitchState_Teleport::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CDMWitchState_Teleport* pInstance = new CDMWitchState_Teleport(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CDMWitchState_Teleport");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitchState_Teleport::Free()
{
	__super::Free();
}

