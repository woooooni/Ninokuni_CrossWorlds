#include "stdafx.h"
#include "Vfx_UI_MouseClick.h"

#include "Particle_Manager.h"

CVfx_UI_MouseClick::CVfx_UI_MouseClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_UI_MouseClick::CVfx_UI_MouseClick(const CVfx_UI_MouseClick& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_UI_MouseClick::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_UI_MouseClick::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_UI_MouseClick::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Circle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 1 && m_fTimeAcc > 0.1f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_CircleLine"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 2 && m_fTimeAcc > 0.25f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Leaf"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;
	}
	else if (m_iCount == 3 && m_fTimeAcc > 0.35f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Twinkle"), XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
		m_iCount++;

		Set_Dead(true);
	}
}

void CVfx_UI_MouseClick::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_UI_MouseClick::Render()
{
	return S_OK;
}

HRESULT CVfx_UI_MouseClick::Ready_Components()
{
	return S_OK;
}

CVfx_UI_MouseClick* CVfx_UI_MouseClick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_UI_MouseClick* pInstance = new CVfx_UI_MouseClick(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_UI_MouseClick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_UI_MouseClick::Clone(void* pArg)
{
	CVfx_UI_MouseClick* pInstance = new CVfx_UI_MouseClick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_UI_MouseClick");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_UI_MouseClick::Free()
{
	__super::Free();
}