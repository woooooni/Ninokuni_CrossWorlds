#include "stdafx.h"
#include "Vfx_SwordMan_Skill_PerfectBlade.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_PerfectBlade::CVfx_SwordMan_Skill_PerfectBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_PerfectBlade::CVfx_SwordMan_Skill_PerfectBlade(const CVfx_SwordMan_Skill_PerfectBlade& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_PerfectBlade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// Decal 1
		if (m_iCount == 0)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.2f), _float3(4.f, 1.f, 4.f), _float3(0.f, 0.f, 0.f));
			m_iCount++;
		}

		// TrailL 1
		else if (m_iCount == 1 && m_iOwnerFrame >= 15)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.2f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, 45.f));
			m_iCount++;
		}
		// TrailR 2
		else if (m_iCount == 2 && m_iOwnerFrame >= 22)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.2f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, -45.f));
			m_iCount++;
		}
		// TrailL 3
		else if (m_iCount == 3 && m_iOwnerFrame >= 28)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.2f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, 45.f));
			m_iCount++;
		}
		// TrailR 4
		else if (m_iCount == 4 && m_iOwnerFrame >= 34)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.2f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, -45.f));
			m_iCount++;
		}
		// TrailL 5
		else if (m_iCount == 5 && m_iOwnerFrame >= 39)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.2f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, 45.f));
			m_iCount++;
		}

		// Sparkle 1
		else if (m_iCount == 6 && m_iOwnerFrame >= 45)
		{
			// 왼쪽 뒤
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(-0.1f, 0.f, -0.1f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			m_iCount++;
		}

		// TrailR 6 (Special) / Sparkle_Circle
		else if (m_iCount == 7 && m_iOwnerFrame >= 47)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.2f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, -45.f));

			// 앞쪽 위
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle_Circle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.1f, 0.05f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			m_iCount++;
		}

		// Fire 1
		else if (m_iCount == 8 && m_iOwnerFrame >= 48)
		{
			// 왼쪽 뒤
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), XMLoadFloat4x4(&m_WorldMatrix), _float3(-0.1f, 0.f, -0.1f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			m_iCount++;
		}
		// Fire 2
		else if (m_iCount == 9 && m_iOwnerFrame >= 49)
		{
			// 중심
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.1f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			m_iCount++;
		}
		// Fire 3
		else if (m_iCount == 10 && m_iOwnerFrame >= 50)
		{
			// 오른쪽 뒤
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.1f, 0.f, -0.1f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 11)
		    m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_PerfectBlade::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_PerfectBlade* CVfx_SwordMan_Skill_PerfectBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_PerfectBlade* pInstance = new CVfx_SwordMan_Skill_PerfectBlade(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_PerfectBlade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_PerfectBlade::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_PerfectBlade* pInstance = new CVfx_SwordMan_Skill_PerfectBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_PerfectBlade");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_PerfectBlade::Free()
{
	__super::Free();
}