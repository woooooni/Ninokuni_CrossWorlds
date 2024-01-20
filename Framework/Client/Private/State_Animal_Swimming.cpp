#include "stdafx.h"
#include "GameInstance.h"
#include "Animals.h"
#include "..\Public\State_Animal_Swimming.h"
#include "Whale.h"

CState_Animal_Swimming::CState_Animal_Swimming(CStateMachine* pMachine)
	: CState_Animals(pMachine)
{

}

HRESULT CState_Animal_Swimming::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	return S_OK;
}

void CState_Animal_Swimming::Enter_State(void* pArg)
{
	m_pModelCom->Set_Animation(m_AnimIndices[0]);
}

void CState_Animal_Swimming::Tick_State(_float fTimeDelta)
{
	CWhale* pWhale = static_cast<CWhale*>(m_pOwner);

	if (2 > pWhale->Get_RomingPoints()->size())
		return;

	_int iCurRomingPoint = pWhale->Get_CurRomingPoint();

	Vec4 vPos = m_pTransformCom->Get_Position();
	Vec4 vNextPos = pWhale->Get_RomingPointIndex(iCurRomingPoint + 1);

	if (0 == iCurRomingPoint)
		m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_BACKFILP01, &vNextPos);

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
	m_pTransformCom->Set_Scale(Vec3(0.5f, 0.5f, 0.5f));

	m_pTransformCom->Move(m_pTransformCom->Get_Look(), *pWhale->Get_Speed(), fTimeDelta);

	if (vPos.x >= vNextPos.x - 0.3f && vPos.x <= vNextPos.x + 0.3f &&
		vPos.y >= vNextPos.y - 0.3f && vPos.y <= vNextPos.y + 0.3f &&
		vPos.z >= vNextPos.z - 0.3f && vPos.z <= vNextPos.z + 0.3f)
	{
		pWhale->Set_CurRomingPoint(pWhale->Get_CurRomingPoint() + 1);
		

		if (pWhale->Get_CurRomingPoint() == pWhale->Get_RomingPoints()->size())
			pWhale->Set_CurRomingPoint(0);
	}
}

void CState_Animal_Swimming::Exit_State()
{

}

_float CState_Animal_Swimming::CalculateAngle(const Vec3& v1, const Vec3& v2)
{
	_float dotProduct = v1.Dot(v2);

	// 각도를 라디안으로 변환
	_float angleRad = ::acos(dotProduct / (v1.Length() * v2.Length()));

	// 라디안을 도(degree)로 변환하여 반환
	return ::XMConvertToDegrees(angleRad);
}

CState_Animal_Swimming* CState_Animal_Swimming::Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList)
{
	CState_Animal_Swimming* pInstance = new CState_Animal_Swimming(pStateMachine);

	if (FAILED(pInstance->Initialize(AnimationList)))
	{
		MSG_BOX("Failed Create : CState_Animal_Swimming");
		Safe_Release<CState_Animal_Swimming*>(pInstance);
	}

	return pInstance;
}

void CState_Animal_Swimming::Free()
{
	__super::Free();
}
