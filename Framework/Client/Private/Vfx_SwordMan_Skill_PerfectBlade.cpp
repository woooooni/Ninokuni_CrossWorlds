#include "stdafx.h"
#include "Vfx_SwordMan_Skill_PerfectBlade.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"

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
 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_PerfectBlade::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), XMLoadFloat4x4(&m_WorldMatrix));
		m_iCount++;
	}
	else if (m_iCount == 1 && m_fTimeAcc > 0.1f)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Ver1"), XMLoadFloat4x4(&m_WorldMatrix));
		m_iCount++;
	}
	//else if (m_iCount == 2 && m_fTimeAcc > 0.25f) //
	//{
	//	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Ver2"), m_vPosition);
	//	m_iCount++;
	//}
	//else if (m_iCount == 3 && m_fTimeAcc > 0.35f) //
	//{
	//	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Ver3"), m_vPosition);
	//	m_iCount++;
	//
	//	Set_Dead(true);
	//}
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