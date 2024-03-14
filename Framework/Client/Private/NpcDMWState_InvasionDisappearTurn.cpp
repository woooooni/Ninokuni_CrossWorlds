#include "stdafx.h"
#include "NpcDMWState_InvasionDisappearTurn.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

CNpcDMWState_InvasionDisappearTurn::CNpcDMWState_InvasionDisappearTurn(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_InvasionDisappearTurn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];
	
	m_fLerpDuration = 1.5f;
	
	m_eLerpMode = LERP_MODE::SMOOTHER_STEP;

	return S_OK;
}

void CNpcDMWState_InvasionDisappearTurn::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);

	m_fLerpHeight.Start(
		Vec3(m_pTransformCom->Get_Position()).y,
		Vec3(m_pTransformCom->Get_Position()).y + 5.f,
		m_fLerpDuration,
		m_eLerpMode);
}

void CNpcDMWState_InvasionDisappearTurn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);	

	if (m_fLerpHeight.bActive)
	{
		m_fLerpHeight.Update(fTimeDelta);

		Vec4 vPos = m_pTransformCom->Get_Position();
		vPos.y = m_fLerpHeight.fCurValue;
		m_pTransformCom->Set_Position(vPos.OneW());
	}
	else
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_INVASION_DISAPPEAR);
}

void CNpcDMWState_InvasionDisappearTurn::Exit_State()
{
}

CNpcDMWState_InvasionDisappearTurn* CNpcDMWState_InvasionDisappearTurn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_InvasionDisappearTurn* pInstance = new CNpcDMWState_InvasionDisappearTurn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_InvasionDisappearTurn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_InvasionDisappearTurn::Free()
{
	__super::Free();
}

