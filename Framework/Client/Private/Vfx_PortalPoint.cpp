#include "stdafx.h"
#include "Vfx_PortalPoint.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"

#include "Effect.h"
#include "Particle.h"

CVfx_PortalPoint::CVfx_PortalPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_PortalPoint::CVfx_PortalPoint(const CVfx_PortalPoint& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_PortalPoint::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_PortalPoint::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_PortalPoint::Tick(_float fTimeDelta)
{
	if (m_iCount == 2)
		return;

	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_PortalPoint_Line"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.3f, 0.f), _float3(5.f, 0.3f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pEffectObject, false);
		Safe_AddRef(pEffectObject);
		m_iCount++;
	}

	else if (m_iCount == 1 && m_fTimeAcc > 0.2f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_PortalPoint_Circle"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.4f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pParticleObject, false);
		Safe_AddRef(pParticleObject);
		m_iCount++;
	}
}

void CVfx_PortalPoint::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_PortalPoint::Render()
{
	return S_OK;
}

HRESULT CVfx_PortalPoint::Ready_Components()
{
	return S_OK;
}

CVfx_PortalPoint* CVfx_PortalPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_PortalPoint* pInstance = new CVfx_PortalPoint(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_PortalPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_PortalPoint::Clone(void* pArg)
{
	CVfx_PortalPoint* pInstance = new CVfx_PortalPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_PortalPoint");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_PortalPoint::Free()
{
	__super::Free();

	if (nullptr != pEffectObject)
	{
		pEffectObject->Set_Dead(true);
		Safe_Release(pEffectObject);
	}

	if (nullptr != pParticleObject)
	{
		pParticleObject->Set_Dead(true);
		Safe_Release(pParticleObject);
	}
}