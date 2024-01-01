#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Walk.h"
#include "Player.h"
#include "Game_Manager.h"

CState_Animal_Walk::CState_Animal_Walk(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Walk::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Walk::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Animal_Walk::Tick_State(_float fTimeDelta)
{
	// TODO
	// 걷다가 -> 가까우면 RUN
	// 아니면 무조건 IDLE

	// 잡히면 바로 플레이어 WorldPos에서 Lift.
	CPlayer* pObj = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pObj)
		return;

	CTransform* pTransform = pObj->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

#pragma region RUN
	Vec4 vPlayerPos = pTransform->Get_Position();
	Vec4 vthisPos = m_pTransformCom->Get_Position();

	Vec3 vDirection = vPlayerPos - vthisPos;
	_float fDistance = vDirection.Length();
#pragma endregion RUN 

#pragma region WALK
	if (true == m_bIsWalk)
	{
		m_bIsWalk = false;

		m_fRandom[0] = GI->RandomFloat(-5.0f, 5.0f);
		m_fRandom[1] = GI->RandomFloat(-5.0f, 5.0f);

		Vec4 vRandomPos = Vec4(m_fRandom[0], vthisPos.y, m_fRandom[1], 1.0f);
		Vec4 vCurPos = m_pTransformCom->Get_Position();
		
		m_vRandomDir = vRandomPos - vCurPos;
		m_vRandomDir.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, m_vRandomDir);

		Vec3 vUp = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 vRight;
		Vec3 vCurDir = m_pTransformCom->Get_Look();

		vRight = vUp.Cross(vCurDir);
		vRight.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, vRight);

		vUp = vCurDir.Cross(vRight);
		vUp.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, vUp);
	}

	m_pTransformCom->Move(m_vRandomDir, 1.0f, fTimeDelta);
#pragma endregion WALK
	_bool bLift = static_cast<CAnimals*>(m_pOwner)->Lifting();

	if (true == bLift)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_LIFT);
	else if (fDistance <= 2.0f && false == m_bExit)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_RUN);
	else if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);
}

void CState_Animal_Walk::Exit_State()
{
	m_bIsWalk = true;
	m_fRandom[0] = m_fRandom[1] = 0.0f;
	m_vRandomDir = Vec4::Zero;
	m_fRandomDist = 0.0f;

	if (true == m_bExit)
	{
		m_bExit = false;
		return;
	}

	m_bExit = true;
}

CState_Animal_Walk* CState_Animal_Walk::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Walk* pInstance = new CState_Animal_Walk(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Walk");
		Safe_Release<CState_Animal_Walk*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Walk::Free()
{
	__super::Free();
}
