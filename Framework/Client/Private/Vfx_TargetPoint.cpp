#include "stdafx.h"
#include "Vfx_TargetPoint.h"

#include "Effect_Manager.h"
#include "Character.h"

#include "Decal.h"

CVfx_TargetPoint::CVfx_TargetPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_TargetPoint::CVfx_TargetPoint(const CVfx_TargetPoint& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_TargetPoint::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_TargetPoint::Initialize(void* pArg)
{
	if (FAILED(GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_UI_Target_Monster"),
		XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pDecalObject, false)))
		return E_FAIL;

	Safe_AddRef(m_pDecalObject);

	return S_OK;
}

void CVfx_TargetPoint::Tick(_float fTimeDelta)
{
}

void CVfx_TargetPoint::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_TargetPoint::Render()
{
	return S_OK;
}

HRESULT CVfx_TargetPoint::Ready_Components()
{
	return S_OK;
}

CVfx_TargetPoint* CVfx_TargetPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_TargetPoint* pInstance = new CVfx_TargetPoint(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_TargetPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_TargetPoint::Clone(void* pArg)
{
	CVfx_TargetPoint* pInstance = new CVfx_TargetPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_TargetPoint");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_TargetPoint::Free()
{
	__super::Free();

	if (nullptr != m_pDecalObject)
	{
		m_pDecalObject->Set_Dead(true);
		Safe_Release(m_pDecalObject);
	}
}