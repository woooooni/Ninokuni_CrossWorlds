#include "State.h"
#include "GameObject.h"
#include "StateMachine.h"
#include "GameInstance.h"



CState::CState(CStateMachine* pStateMachine)
	: m_pStateMachineCom(pStateMachine)
{	
	
}

HRESULT CState::Initialize(const list<wstring>& AnimationList)
{
	m_pOwner = m_pStateMachineCom->Get_Owner();
	if (nullptr == m_pOwner)
		return E_FAIL;

	m_pModelCom = m_pStateMachineCom->Get_Owner()->Get_Component<CModel>(L"Com_Model");
	if (nullptr == m_pModelCom)
		return E_FAIL;

	m_pRigidBodyCom = m_pStateMachineCom->Get_Owner()->Get_Component<CRigidBody>(L"Com_RigidBody");
	if (nullptr == m_pRigidBodyCom)
		return E_FAIL;

	m_pTransformCom = m_pStateMachineCom->Get_Owner()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == m_pTransformCom)
		return E_FAIL;



	m_AnimIndices.reserve(AnimationList.size());
	for (auto strAnimName : AnimationList)
	{
 		_int iAnimIndex = m_pModelCom->Find_AnimationIndex(strAnimName);
		if (-1 != iAnimIndex)
			m_AnimIndices.push_back(iAnimIndex);
		else
		{
			wstring strErrMsg = L"Find Animation Failed. : " + strAnimName + L"/ Object : " + m_pOwner->Get_PrototypeTag() + L"/ Model : " + m_pModelCom->Get_Name();
			MessageBox(nullptr, strErrMsg.c_str(), L"System Message", MB_OK);
			return E_FAIL;
		}
			
	}

	if (0 == m_AnimIndices.size())
	{
		MSG_BOX("Animation Size is 0");
		return E_FAIL;
	}
		


	return S_OK;
}


void CState::Free()
{
	__super::Free();
}
