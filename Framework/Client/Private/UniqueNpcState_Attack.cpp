#include "stdafx.h"
#include "UniqueNpcState_Attack.h"

#include "GameNpc.h"

#include "Animation.h"
#include "Ruby.h"

CUniqueNpcState_Attack::CUniqueNpcState_Attack(CStateMachine* pStateMachine)
	: CNpcState_Base(pStateMachine)
{
}

HRESULT CUniqueNpcState_Attack::Initialize(const list<wstring>& AnimationList)
{
	__super::Initialize(AnimationList);

	m_iCurrAnimIndex = m_AnimIndices[0];

	return S_OK;
}

void CUniqueNpcState_Attack::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_iCurrAnimIndex);
}

void CUniqueNpcState_Attack::Tick_State(_float fTimeDelta)
{
	__super::Tick_State(fTimeDelta);

	//if (m_pOwner->Get_ObjectTag() == TEXT("Ruby"))
	//{
	//	CRuby* pRuby = static_cast<CRuby*>(m_pOwner);
	//	CGameObject* pPlantKiller = pRuby->Get_QuestObject();

	//	if (nullptr != pPlantKiller)
	//	{
	//		CTransform* pPlantTransform = pPlantKiller->Get_Component_Transform();
	//		Vec4 vDir = pPlantTransform->Get_Position() - m_pTransformCom->Get_Position();
	//		_float fDistance = vDir.Length();

	//		if (fDistance >= 2.5f)
	//		{
	//			vDir.Normalize();
	//			m_pTransformCom->LookAt_ForLandObject(pPlantTransform->Get_Position());
	//			m_pTransformCom->Move(vDir, 2.0f, fTimeDelta);
	//		}
	//	}
	//}

}

void CUniqueNpcState_Attack::Exit_State()
{
}

CUniqueNpcState_Attack* CUniqueNpcState_Attack::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CUniqueNpcState_Attack* pInstance = new CUniqueNpcState_Attack(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Fail Create : CUniqueNpcState_Attack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUniqueNpcState_Attack::Free()
{
	__super::Free();
}

