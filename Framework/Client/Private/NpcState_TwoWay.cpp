#include "stdafx.h"
#include "NpcState_TwoWay.h"

#include "GameNpc.h"

#include "Animation.h"

CNpcState_TwoWay::CNpcState_TwoWay(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CNpcState_TwoWay::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CNpcState_TwoWay::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CNpcState_TwoWay::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_pTransformCom->Get_Position());
	XMStoreFloat4(&vDestPos, m_pNpc->Get_RoamingIndex(m_pNpc->Get_CurRoamingIndex()));


	if (m_bIsMove)
	{
		m_pTransformCom->LookAt_ForLandObject(m_pNpc->Get_RoamingIndex(m_pNpc->Get_CurRoamingIndex()));
		m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat().fSpeed, fTimeDelta);

		if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
			vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
		{
			if (!m_bReverse)
			{
				m_pNpc->Set_CurRoamingIndex(m_pNpc->Get_CurRoamingIndex() + 1);
				if (m_pNpc->Get_CurRoamingIndex() == m_pNpc->Get_RoamingArea()->size())
				{
					m_bIsMove = false;
					m_bReverse = true;
					m_pNpc->Set_CurRoamingIndex(m_pNpc->Get_RoamingArea()->size() - 1);
					m_pModelCom->Set_Animation(m_AnimIndices[1]);
				}
			}
			else
			{
				m_pNpc->Set_CurRoamingIndex(m_pNpc->Get_CurRoamingIndex() - 1);
				if (m_pNpc->Get_CurRoamingIndex() == -1)
				{
					m_bIsMove = false;
					m_bReverse = false;
					m_pNpc->Set_CurRoamingIndex(0);
					m_pModelCom->Set_Animation(m_AnimIndices[1]);
				}
			}
		}
	}
	else
	{
		/* 0. Move / 1. Idle */
		if (__super::State_Wait(m_pModelCom->Get_Animation(m_AnimIndices[1])->Is_Loop(), m_pModelCom->Get_Animation(m_AnimIndices[1])->Get_AnimationName(), 5.f, fTimeDelta))
		{
			m_bIsMove = true;
			m_pModelCom->Set_Animation(m_AnimIndices[0]);
		}
	}
}

void CNpcState_TwoWay::Exit_State()
{
}

CNpcState_TwoWay* CNpcState_TwoWay::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CNpcState_TwoWay* pInstance = new CNpcState_TwoWay(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CNpcState_TwoWay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNpcState_TwoWay::Free()
{
	__super::Free();
}

