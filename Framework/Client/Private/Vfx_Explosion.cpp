#include "stdafx.h"
#include "Vfx_Explosion.h"

#include "Particle_Manager.h"
#include "Utils.h"

CVfx_Explosion::CVfx_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Explosion::CVfx_Explosion(const CVfx_Explosion& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Explosion::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_Explosion::Initialize(void* pArg)
{
 	return S_OK;
}

void CVfx_Explosion::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Explosion_Fire_Big"), 
		XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Explosion_Fire_Small"),
		XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

	Set_Dead(true);
}

void CVfx_Explosion::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Explosion::Render()
{
	return S_OK;
}

HRESULT CVfx_Explosion::Ready_Components()
{
	return S_OK;
}

CVfx_Explosion* CVfx_Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Explosion* pInstance = new CVfx_Explosion(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Explosion::Clone(void* pArg)
{
	CVfx_Explosion* pInstance = new CVfx_Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Explosion");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Explosion::Free()
{
	__super::Free();
}