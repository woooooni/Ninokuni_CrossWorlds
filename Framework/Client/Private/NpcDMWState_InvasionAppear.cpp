#include "stdafx.h"
#include "NpcDMWState_InvasionAppear.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

CNpcDMWState_InvasionAppear::CNpcDMWState_InvasionAppear(CStateMachine* pStateMachine)
	: CNpcDMWState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_InvasionAppear::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fDuration = 2.5f;
	m_fDestY = 12.f; // 변경시 카메라 연출을 위한 높이도 수정 필요(카메라 담당에게 말해주세요)

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(0.f, 30.f, 65.f, 1.f));// 아직 구도 안맞아서 일단 z 더 앞으로
	return S_OK;
}

void CNpcDMWState_InvasionAppear::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex, MIN_TWEEN_DURATION);

	m_tLerpDesc.fCurValue = 30.f;
	m_tLerpDesc.Start(m_tLerpDesc.fCurValue, m_fDestY, m_fDuration, LERP_MODE::SMOOTHER_STEP);
}

void CNpcDMWState_InvasionAppear::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	if (m_tLerpDesc.bActive)
	{
		m_tLerpDesc.fCurValue = m_tLerpDesc.Update(fTimeDelta);
		Vec4 vWitchPos = m_pTransformCom->Get_Position();
		vWitchPos.y = m_tLerpDesc.fCurValue;
		
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWitchPos);
	}



	if (m_pModelCom->Is_Finish() && !m_pModelCom->Is_Tween())
		m_pStateMachineCom->Change_State(CDreamMazeWitch_Npc::WITCHSTATE_INVASION_IDLE);
}

void CNpcDMWState_InvasionAppear::Exit_State()
{
}

CNpcDMWState_InvasionAppear* CNpcDMWState_InvasionAppear::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcDMWState_InvasionAppear* pInstance = new CNpcDMWState_InvasionAppear(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcDMWState_InvasionAppear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcDMWState_InvasionAppear::Free()
{
	__super::Free();
}

