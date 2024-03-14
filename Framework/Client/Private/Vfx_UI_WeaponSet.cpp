#include "stdafx.h"
#include "Vfx_UI_WeaponSet.h"

#include "Particle_Manager.h"

CVfx_UI_WeaponSet::CVfx_UI_WeaponSet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_UI_WeaponSet::CVfx_UI_WeaponSet(const CVfx_UI_WeaponSet& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_UI_WeaponSet::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_UI_WeaponSet::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_UI_WeaponSet::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_WeaponSet_Main"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 1 && m_fTimeAcc > 0.1f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_WeaponSet_Twinkle_01"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_WeaponSet_Twinkle_02"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;

		Set_Dead(true);
	}
}

void CVfx_UI_WeaponSet::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_UI_WeaponSet::Render()
{
	return S_OK;
}

HRESULT CVfx_UI_WeaponSet::Ready_Components()
{
	return S_OK;
}

CVfx_UI_WeaponSet* CVfx_UI_WeaponSet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_UI_WeaponSet* pInstance = new CVfx_UI_WeaponSet(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_UI_WeaponSet");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_UI_WeaponSet::Clone(void* pArg)
{
	CVfx_UI_WeaponSet* pInstance = new CVfx_UI_WeaponSet(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_UI_WeaponSet");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_UI_WeaponSet::Free()
{
	__super::Free();
}