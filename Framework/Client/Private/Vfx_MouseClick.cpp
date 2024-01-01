#include "stdafx.h"
#include "Vfx_MouseClick.h"

#include "Particle_Manager.h"

CVfx_MouseClick::CVfx_MouseClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_MouseClick::CVfx_MouseClick(const CVfx_MouseClick& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_MouseClick::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_MouseClick::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_MouseClick::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Circle"), XMLoadFloat4x4(&m_WorldMatrix));
		m_iCount++;
	}
	else if (m_iCount == 1 && m_fTimeAcc > 0.1f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_CircleLine"), XMLoadFloat4x4(&m_WorldMatrix));
		m_iCount++;
	}
	else if (m_iCount == 2 && m_fTimeAcc > 0.25f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Leaf"), XMLoadFloat4x4(&m_WorldMatrix));
		m_iCount++;
	}
	else if (m_iCount == 3 && m_fTimeAcc > 0.35f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Twinkle"), XMLoadFloat4x4(&m_WorldMatrix));
		m_iCount++;

		Set_Dead(true);
	}
}

void CVfx_MouseClick::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_MouseClick::Render()
{
	return S_OK;
}

HRESULT CVfx_MouseClick::Ready_Components()
{
	return S_OK;
}

CVfx_MouseClick* CVfx_MouseClick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_MouseClick* pInstance = new CVfx_MouseClick(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_MouseClick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_MouseClick::Clone(void* pArg)
{
	CVfx_MouseClick* pInstance = new CVfx_MouseClick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_MouseClick");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_MouseClick::Free()
{
	__super::Free();
}