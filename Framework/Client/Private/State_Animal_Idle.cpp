#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Idle.h"
#include "Animation.h"
#include "Character_SwordMan.h"

CState_Animal_Idle::CState_Animal_Idle(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Idle::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Idle::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Animal_Idle::Tick_State(_float fTimeDelta)
{
	CGameObject* pObj = GI->Find_GameObject(LEVELID::LEVEL_TEST, LAYER_TYPE::LAYER_CHARACTER, TEXT("SwordMan"));
	if (nullptr == pObj)
		return;

	CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	Vec4 vPlayerPos = pTransform->Get_Position();
	Vec4 vthisPos = m_pTransformCom->Get_Position();

	Vec3 vDirection = vPlayerPos - vthisPos;
	_float fDistance = vDirection.Length();

	_bool bLift = static_cast<CAnimals*>(m_pOwner)->Lifting();

	if (true == bLift)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_LIFT);
	else if (fDistance <= 2.0f && false == m_bExit)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_RUN);
	else if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
	{
		_int iRandomVal = GI->RandomInt(0, 1);

		switch (iRandomVal)
		{
		case 0:
			m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);
			break;
		case 1:
			m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_WALK);
			break;
		}
	}

}

void CState_Animal_Idle::Exit_State()
{
	if (true == m_bExit)
	{
		m_bExit = false;
		return;
	}

	m_bExit = true;
}

CState_Animal_Idle* CState_Animal_Idle::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Idle* pInstance = new CState_Animal_Idle(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Idle");
		Safe_Release<CState_Animal_Idle*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Idle::Free()
{
	__super::Free();
}
