#include "stdafx.h"
#include "UniqueNpcState_Walk.h"

#include "GameNpc.h"

#include "Animation.h"
#include "Ruby.h"

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

	if (m_pOwner->Get_ObjectTag() == TEXT("Ruby"))
	{
		CRuby* pRuby = static_cast<CRuby*>(m_pOwner);
		CGameObject* pPlantKiller = pRuby->Get_QuestObject();
		
		if (nullptr != pPlantKiller)
		{
			CTransform* pPlantTransform = pPlantKiller->Get_Component_Transform();
			Vec4 vDir = pPlantTransform->Get_Position() - m_pTransformCom->Get_Position();
			_float fDistance = vDir.Length();

			if (fDistance >= 1.75f)
			{
				vDir.Normalize();
				m_pTransformCom->LookAt_ForLandObject(pPlantTransform->Get_Position());
				m_pTransformCom->Move(vDir, 2.0f, fTimeDelta);
			}
			else
			{
				if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
					m_pStateMachineCom->Change_State(CGameNpc::NPC_STATE::NPC_UNIQUENPC_ATTACK);
			}
		}
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

