#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Lift.h"
#include "Animation.h"
#include "Character_SwordMan.h"

CState_Animal_Lift::CState_Animal_Lift(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Lift::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Lift::Enter_State(void* pArg)
{
	m_fNextTime = 0.0f;
	m_iCurrAnimIndex = m_AnimIndices[0];
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Animal_Lift::Tick_State(_float fTimeDelta)
{
	CGameObject* pObj = GI->Find_GameObject(LEVELID::LEVEL_TEST, LAYER_TYPE::LAYER_CHARACTER, TEXT("SwordMan"));
	if (nullptr == pObj)
		return;

	CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	Matrix mPlayerWorld = pTransform->Get_WorldFloat4x4();
	m_pTransformCom->Set_WorldMatrix(mPlayerWorld);

	m_fNextTime += fTimeDelta;
	if (m_fNextTime >= m_fChangeMotionTime)
	{
		m_fNextTime = 0.0f;
		m_iCurrAnimIndex = m_AnimIndices[1];
	}
}

void CState_Animal_Lift::Exit_State()
{
	m_fNextTime = 0.0f;
}

CState_Animal_Lift* CState_Animal_Lift::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Lift* pInstance = new CState_Animal_Lift(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Lift");
		Safe_Release<CState_Animal_Lift*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Lift::Free()
{
	__super::Free();
}
