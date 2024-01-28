#include "stdafx.h"
#include "NpcDMWState_InvasionDisappearTurn.h"

#include "DreamMazeWitch_Npc.h"

#include "Animation.h"

CNpcDMWState_InvasionDisappearTurn::CNpcDMWState_InvasionDisappearTurn(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcDMWState_InvasionDisappearTurn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	m_fTurnSpeed = 5.f;

	return S_OK;
}

void CNpcDMWState_InvasionDisappearTurn::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);

	m_vDestLook = -m_pTransformCom->Get_Look();
}

void CNpcDMWState_InvasionDisappearTurn::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_float fDotProduck = XMVector3Dot(m_pTransformCom->Get_Look(), m_vDestLook).m128_f32[0];
	_float fRadianAngle = acosf(fDotProduck / (XMVector3Length(m_pTransformCom->Get_Look()).m128_f32[0] * XMVector3Length(m_vDestLook).m128_f32[0]));
	_float fDegreeAngle = XMConvertToDegrees(fRadianAngle);

	if (fabs(fRadianAngle) > 1.f)
		m_pTransformCom->Turn(Vec3(0.f, 1.f, 0.f), m_fTurnSpeed, fTimeDelta);
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

