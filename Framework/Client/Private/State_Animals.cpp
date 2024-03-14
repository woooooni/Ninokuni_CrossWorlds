#include "stdafx.h"
#include "..\Public\State_Animals.h"
#include "Animals.h"
#include "GameInstance.h"

CState_Animals::CState_Animals(CStateMachine* pStateMachine)
	: CState(pStateMachine)
{
}

HRESULT CState_Animals::Initialize(const list<wstring>& AnimationList)
{
	if (FAILED(__super::Initialize(AnimationList)))
		return E_FAIL;

	m_pAnimal = static_cast<CAnimals*>(m_pStateMachineCom->Get_Owner());
	if (nullptr == m_pAnimal)
		return E_FAIL;


	return S_OK;
}

_bool CState_Animals::ReturnToCenter(_float fTimeDelta, Vec4& vDir)
{
	Vec4 vCenterPos = static_cast<CAnimals*>(m_pOwner)->Get_Center();

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
		return false;

	Vec4 vCurPos = pTransform->Get_Position();
	Vec4 vCenterDir = vCenterPos - vCurPos;

	_float fDistance = vCenterDir.Length();
	vCenterDir.Normalize();

	if (6.f <= fDistance)
	{
		m_pTransformCom->Set_State(CTransform::STATE::STATE_LOOK, vCenterDir);

		Vec3 vUp = Vec3(0.0f, 1.0f, 0.0f);
		Vec3 vRight;
		Vec3 vCurDir = m_pTransformCom->Get_Look();

		vRight = vUp.Cross(vCurDir);
		vRight.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_RIGHT, vRight);

		vUp = vCurDir.Cross(vRight);
		vUp.Normalize();
		m_pTransformCom->Set_State(CTransform::STATE::STATE_UP, vUp);

		vDir = vCenterDir;

		return true;
	}

	return false;
}

Vec4 CState_Animals::Get_Point_In_Bezier(const vector<Vec4>* points, const _float& fRatio)
{


	return Vec4();
}

void CState_Animals::Free()
{
	__super::Free();
}
