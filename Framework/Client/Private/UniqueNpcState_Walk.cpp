#include "stdafx.h"
#include "UniqueNpcState_Walk.h"

#include "GameNpc.h"

#include "Animation.h"

CUniqueNpcState_Walk::CUniqueNpcState_Walk(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Walk::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Walk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CUniqueNpcState_Walk::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	_float4 vPos;
	_float4 vDestPos;

	XMStoreFloat4(&vPos, m_pTransformCom->Get_Position());
	XMStoreFloat4(&vDestPos, m_pNpc->Get_RoamingIndex(m_pNpc->Get_CurRoamingIndex()));


	//if (m_bIsMove)
	//{
	m_pTransformCom->LookAt_ForLandObject(m_pNpc->Get_RoamingIndex(m_pNpc->Get_CurRoamingIndex()));
	m_pTransformCom->Move(m_pTransformCom->Get_Look(), m_pNpc->Get_Stat()->fSpeed, fTimeDelta);

	if (vPos.x >= vDestPos.x - 0.1f && vPos.x <= vDestPos.x + 0.1f &&
		vPos.z >= vDestPos.z - 0.1f && vPos.z <= vDestPos.z + 0.1f)
	{
		m_pNpc->Set_CurRoamingIndex(m_pNpc->Get_CurRoamingIndex() + 1);
		if (m_pNpc->Get_CurRoamingIndex() == m_pNpc->Get_RoamingArea()->size())
			m_pNpc->Set_CurRoamingIndex(0);

		//m_bIsMove = false;
	}
}

void CUniqueNpcState_Walk::Exit_State()
{
}

CUniqueNpcState_Walk* CUniqueNpcState_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Walk* pInstance = new CUniqueNpcState_Walk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Walk");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Walk::Free()
{
	__super::Free();
}

