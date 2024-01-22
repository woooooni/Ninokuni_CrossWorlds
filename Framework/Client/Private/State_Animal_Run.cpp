#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Run.h"
#include "Player.h"
#include "Game_Manager.h"

CState_Animal_Run::CState_Animal_Run(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Run::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Run::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Animal_Run::Tick_State(_float fTimeDelta)
{
	CPlayer* pObj = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pObj)
		return;

	CTransform* pTransform = pObj->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	if (true == m_bIsRun)
	{
		m_vDirection = pTransform->Get_State(CTransform::STATE::STATE_LOOK);
		m_vDirection.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, m_vDirection);

		Vec3 vUp = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 vRight;
		Vec3 vCurDir = m_pTransformCom->Get_Look();

		vRight = vUp.Cross(vCurDir);
		vRight.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, vRight);

		vUp = vCurDir.Cross(vRight);
		vUp.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, vUp);

		m_bIsRun = false;
	}

	CAnimals* pAnimal = static_cast<CAnimals*>(m_pOwner);
	m_pTransformCom->Move(m_vDirection, *pAnimal->Get_Speed() , fTimeDelta);

	_bool bLift = static_cast<CAnimals*>(m_pOwner)->Lifting();

	if (true == bLift)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_LIFT);
	else if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);
}

void CState_Animal_Run::Exit_State()
{
	m_bIsRun = true;
	m_vDirection = Vec4::Zero;
}

CState_Animal_Run* CState_Animal_Run::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Run* pInstance = new CState_Animal_Run(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Run");
		Safe_Release<CState_Animal_Run*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Run::Free()
{
	__super::Free();
}
