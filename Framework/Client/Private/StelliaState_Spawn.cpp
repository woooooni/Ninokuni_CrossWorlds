#include "stdafx.h"
#include "StelliaState_Spawn.h"

#include "Stellia.h"

CStelliaState_Spawn::CStelliaState_Spawn(CStateMachine* pStateMachine)
	: CStelliaState_Base(pStateMachine)
{
}

HRESULT CStelliaState_Spawn::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	return S_OK;
}

void CStelliaState_Spawn::Enter_State(void* pArg)
{

	// m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.f, 10.f, 1.f));
	m_pModelCom->Set_Animation(TEXT("SKM_Stellia.ao|Stellia_Spawn"));

	//if (m_pPlayer != nullptr)
	//	m_pTransformCom->LookAt_ForLandObject(m_pPlayerTransform->Get_Position());
}

void CStelliaState_Spawn::Tick_State(_float fTimeDelta)
{
	// player가 없으면 그냥 굳어버리게.
	if (m_pPlayer != nullptr)
	{
		if (m_pModelCom->Is_Finish())
		{
			m_pStateMachineCom->Change_State(CStellia::STELLIA_SPAWNIDLE);
		}
	}
}

void CStelliaState_Spawn::Exit_State()
{
}

CStelliaState_Spawn* CStelliaState_Spawn::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CStelliaState_Spawn* pInstance = new CStelliaState_Spawn(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CStelliaState_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaState_Spawn::Free()
{
	__super::Free();
}

