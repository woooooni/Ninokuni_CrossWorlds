#include "stdafx.h"
#include "Camera_Follow.h"

#include "GameInstance.h"

#include "Camera_Manager.h"

CCamera_Follow::CCamera_Follow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Follow::CCamera_Follow(const CCamera_Follow & rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Follow::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Follow::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Camera */
	{
		m_tLerpDist.fCurValue = Cam_Dist_Follow_Default;

		m_vLookAtOffset = Vec4{ 0.f, 0.f, 0.f, 1.f };
		m_vTargetOffset = Vec4{ 0.f, 0.f, 0.f, 1.f };

		m_vMouseSensitivity = Vec2{ 0.5f, 0.5f };
	}

	return S_OK;
}

void CCamera_Follow::Tick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::Tick(fTimeDelta);

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Calculate_Position(fTimeDelta));
	m_pTransformCom->LookAt(Calculate_Look(fTimeDelta));
}

void CCamera_Follow::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pTargetObj || nullptr == m_pLookAtObj)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Follow::Render()
{
	return S_OK;
}


HRESULT CCamera_Follow::Ready_Components()
{
	return S_OK;
}

Vec4 CCamera_Follow::Calculate_Look(_float fTimeDelta)
{
	Vec3 vRelativeLookAtOffSet = XMVector3TransformCoord(m_vLookAtOffset, m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_WorldMatrix());

	Vec4 vLookAt = Vec4(m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position()) + vRelativeLookAtOffSet;
	
	vLookAt.w = 1.f;

	return vLookAt;
}

Vec4 CCamera_Follow::Calculate_Position(_float fTimeDelta)
{
	_long	MouseMove = 0l;

	if (MouseMove = GI->Get_DIMMoveState(DIMM_X))
		m_vAngle.x += MouseMove * m_vMouseSensitivity.y * fTimeDelta * -1.f;

	if (MouseMove = GI->Get_DIMMoveState(DIMM_Y))
	{
		m_vAngle.y += MouseMove * m_vMouseSensitivity.x * fTimeDelta;

		if (m_vAngle.y <= 0.01f)
		{
			m_vAngle.y = 0.01f;
		}
		else if (3.13f < m_vAngle.y)
		{
			m_vAngle.y = 3.13f;
		}
	}

	/* 구면 좌표계(극좌표계) -> 왼손 직교 좌표계 */
	Vec4 vCamLocal;
	{
		vCamLocal.x = m_tLerpDist.fCurValue * sinf(m_vAngle.y) * cosf(m_vAngle.x);	// x = r * sin(위도 앙각) * cos(경도 방위각)
		vCamLocal.y = m_tLerpDist.fCurValue * cosf(m_vAngle.y);					// y = r * cos(위도 앙각)
		vCamLocal.z = m_tLerpDist.fCurValue * sinf(m_vAngle.y) * sinf(m_vAngle.x);	// z = r * sin(위도 앙각) * sin(경도 방위각)
		vCamLocal.w = 0.f;
	}

	/* 현재 트랜스폼 상대적 오프셋 계산 */
	//Vec3 vRelativeTargetOffSet = XMVector3TransformCoord(m_vTargetOffset, m_pTransformCom->Get_WorldMatrix());

	/* 카메라 최종 월드 위치 */
	Vec4 vCamWorld = vCamLocal + Vec4(m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position());// +vRelativeTargetOffSet;

	vCamWorld.w = 1.f;

	return vCamWorld;
}

CCamera_Follow * CCamera_Follow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Follow::Clone(void* pArg)
{
	CCamera_Follow*		pInstance = new CCamera_Follow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Follow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Follow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
