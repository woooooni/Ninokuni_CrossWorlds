#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_BackFlip_01.h"
#include "Whale.h"

CState_BackFlip_01::CState_BackFlip_01(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_BackFlip_01::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_BackFlip_01::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);

	// 다음 Index로 미리 LookAt
	Vec4 vNextPos = *static_cast<Vec4*>(pArg);

	Vec4 vPos = m_pTransformCom->Get_Position();
	Vec4 vMoveDir = vNextPos - vPos;
	vMoveDir.Normalize();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, vMoveDir);

	Vec3 vLook = m_pTransformCom->Get_Look();
	Vec3 vUp = Vec3(0.0f, 1.0f, 0.0f);
	Vec3 vRight;
	vRight = vUp.Cross(vLook);
	vRight.Normalize();
	m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, vRight);

	vUp = vLook.Cross(vRight);
	vUp.Normalize();
	m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, vUp);
	m_pTransformCom->Set_Scale(Vec3(0.7f, 0.7f, 0.7f));
	
	m_fAcc = 0.0f;
}

void CState_BackFlip_01::Tick_State(_float fTimeDelta)
{
	CWhale* pWhale = static_cast<CWhale*>(m_pOwner);

	if (0 == pWhale->Get_RomingPoints()->size())
		return;

	_int iCurRomingPoint = pWhale->Get_CurRomingPoint(); 

	Vec3 vStartPos = pWhale->Get_RomingPointIndex(iCurRomingPoint);
	Vec3 vEndPos = pWhale->Get_RomingPointIndex(iCurRomingPoint + 1);
	Vec4 vPos = Vec3(0.0f,0.0f,0.0f); // 현재 위치

	m_pTransformCom->ParabolicFormula(vStartPos, vEndPos, fTimeDelta, 60.0f, 4.5f);
	m_pTransformCom->Rotation_Acc(::XMVector3Normalize(m_pTransformCom->Get_Look()), ::XMConvertToRadians(fTimeDelta * 35.0f));
}

void CState_BackFlip_01::Exit_State()
{
	m_pTransformCom->ResetTime();
}

CState_BackFlip_01* CState_BackFlip_01::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_BackFlip_01* pInstance = new CState_BackFlip_01(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_BackFlip_01");
		Safe_Release<CState_BackFlip_01*>(pInstance);
	}

	return pInstance;
}

void CState_BackFlip_01::Free()
{
	__super::Free();
}
