#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Stand.h"
#include "Animation.h"
#include "Character_SwordMan.h"
#include "Player.h"
#include "Game_Manager.h"

CState_Animal_Stand::CState_Animal_Stand(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Stand::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Stand::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Animal_Stand::Tick_State(_float fTimeDelta)
{
	CPlayer* pObj = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pObj)
		return;

	CTransform* pTransform = pObj->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return;

	Vec4 vPlayerPos = pTransform->Get_Position();
	Vec4 vthisPos = m_pTransformCom->Get_Position();

	Vec3 vDirection = vPlayerPos - vthisPos;
	_float fDistance = vDirection.Length();

	_bool bLift = static_cast<CAnimals*>(m_pOwner)->Lifting();

	if (true == bLift)
	{
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_LIFT);
		m_iPrevAnim = CAnimals::STATE::STATE_LIFT;
	}
}

void CState_Animal_Stand::Exit_State()
{

}

CState_Animal_Stand* CState_Animal_Stand::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Stand* pInstance = new CState_Animal_Stand(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Stand");
		Safe_Release<CState_Animal_Stand*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Stand::Free()
{
	__super::Free();
}
