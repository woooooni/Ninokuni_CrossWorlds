#include "stdafx.h"
#include "Vfx_SwordMan_Skill_SipohoningLunge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_SipohoningLunge::CVfx_SwordMan_Skill_SipohoningLunge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_SipohoningLunge::CVfx_SwordMan_Skill_SipohoningLunge(const CVfx_SwordMan_Skill_SipohoningLunge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_SipohoningLunge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 
		if (m_iCount == 0)
		{

			m_iCount++;
		}

		// 
		else if (m_iCount == 1 && m_iOwnerFrame >= 15)
		{

			m_iCount++;
		}
	}
}

void CVfx_SwordMan_Skill_SipohoningLunge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_SipohoningLunge* CVfx_SwordMan_Skill_SipohoningLunge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_SipohoningLunge* pInstance = new CVfx_SwordMan_Skill_SipohoningLunge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_SipohoningLunge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_SipohoningLunge::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_SipohoningLunge* pInstance = new CVfx_SwordMan_Skill_SipohoningLunge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SipohoningLunge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_SipohoningLunge::Free()
{
	__super::Free();
}