#include "stdafx.h"
#include "NpcState_OneWay.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_OneWay::CNpcState_OneWay(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_OneWay::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_OneWay::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_OneWay::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_pTransformCom->Get_Position());
	XMStoreFloat4(&vDestPos, m_pNpc->Get_RoamingIndex(m_iCurRoamingIndex));


	//if (m_bIsMove)
	//{
		m_pTransformCom->LookAt_ForLandObject(m_pNpc->Get_RoamingIndex(m_iCurRoamingIndex));
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat().fSpeed, fTimeDelta);

		if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
			vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
		{
			m_iCurRoamingIndex += 1;
			if (m_iCurRoamingIndex == m_pNpc->Get_RoamingArea()->size())
				m_iCurRoamingIndex = 0;

			//m_bIsMove = false;
		}
	//}
	//else
	//{
	//	/* 0. Move / 1. Idle */
	//	if (__super::State_Wait(m_pModelCom->Get_Animation(m_AnimIndices[1])->Is_Loop(), m_pModelCom->Get_Animation(m_AnimIndices[1])->Get_AnimationName(), 5.f, fTimeDelta))
	//	{
	//		m_bIsMove = true;
	//		m_pModelCom->Set_Animation(m_AnimIndices[0]);
	//	}
	//}
}

void CNpcState_OneWay::Exit_State()
{
}

CNpcState_OneWay* CNpcState_OneWay::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_OneWay* pInstance = new CNpcState_OneWay(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_OneWay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_OneWay::Free()
{
	__super::Free();
}

