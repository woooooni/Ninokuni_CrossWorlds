#include "stdafx.h"
#include "Camera_Quater.h"
#include "GameInstance.h"

#include "Camera_Manager.h"


CCamera_Quater::CCamera_Quater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Quater::CCamera_Quater(const CCamera_Quater& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Quater::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Quater::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Quater::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);
}

void CCamera_Quater::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

Vec4 CCamera_Quater::Get_LookAt()
{
	return Vec4::UnitW;
}

void CCamera_Quater::Tick_Blending(const _float fDeltaTime)
{
	const Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	const Vec4 vLook = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition);

	m_pTransformCom->LookAt(vLook);
}

void CCamera_Quater::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);

}

HRESULT CCamera_Quater::Ready_Components()
{
	return S_OK;
}

CCamera_Quater* CCamera_Quater::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Quater* pInstance = new CCamera_Quater(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Quater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Quater::Clone(void* pArg)
{
	CCamera_Quater* pInstance = new CCamera_Quater(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Quater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Quater::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
