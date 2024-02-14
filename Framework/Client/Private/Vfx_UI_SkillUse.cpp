#include "stdafx.h"
#include "Vfx_UI_SkillUse.h"

#include "Particle_Manager.h"

CVfx_UI_SkillUse::CVfx_UI_SkillUse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_UI_SkillUse::CVfx_UI_SkillUse(const CVfx_UI_SkillUse& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_UI_SkillUse::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_UI_SkillUse::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_UI_SkillUse::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_SkillUse_Main"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 1 && m_fTimeAcc > 0.1f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_SkillUse_CircleLine"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 2 && m_fTimeAcc > 0.25f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_SkillUse_Circle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 3 && m_fTimeAcc > 0.35f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_SkillUse_Twinkle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;

		Set_Dead(true);
	}
}

void CVfx_UI_SkillUse::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_UI_SkillUse::Render()
{
	return S_OK;
}

HRESULT CVfx_UI_SkillUse::Ready_Components()
{
	return S_OK;
}

CVfx_UI_SkillUse* CVfx_UI_SkillUse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_UI_SkillUse* pInstance = new CVfx_UI_SkillUse(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_UI_SkillUse");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_UI_SkillUse::Clone(void* pArg)
{
	CVfx_UI_SkillUse* pInstance = new CVfx_UI_SkillUse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_UI_SkillUse");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_UI_SkillUse::Free()
{
	__super::Free();
}