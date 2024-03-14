#include "stdafx.h"
#include "Vfx_Smoke.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"

#include "Effect.h"
#include "Particle.h"

CVfx_Smoke::CVfx_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Smoke::CVfx_Smoke(const CVfx_Smoke& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Smoke::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVfx_Smoke::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Smoke::Tick(_float fTimeDelta)
{
	// -33, 8, -30
	// ScaleStart -> 0.5, 0.5 // Scale Min, Max 1.0f -> 2.0f
	m_fTimeAcc += fTimeDelta;

	if (m_fTimeAcc > 0.5f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("TentSmoke"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(2.5f, 8.0f, -2.75f), _float3(1.0f, 1.0f, 1.f), _float3(0.f, 0.f, 0.f));

		m_fTimeAcc = 0.0f;
	}

	
}

void CVfx_Smoke::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Smoke::Render()
{
	return S_OK;
}

HRESULT CVfx_Smoke::Ready_Components()
{
	return S_OK;
}

CVfx_Smoke* CVfx_Smoke::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Smoke* pInstance = new CVfx_Smoke(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Smoke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Smoke::Clone(void* pArg)
{
	CVfx_Smoke* pInstance = new CVfx_Smoke(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Smoke");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Smoke::Free()
{
	__super::Free();
}