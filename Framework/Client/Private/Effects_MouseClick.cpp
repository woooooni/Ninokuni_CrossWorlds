#include "stdafx.h"
#include "Effects_MouseClick.h"

#include "Particle_Manager.h"

CEffects_MouseClick::CEffects_MouseClick(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_PARTICLE)
{
}

CEffects_MouseClick::CEffects_MouseClick(const CEffects_MouseClick& rhs)
	: CGameObject(rhs)
{
}

HRESULT CEffects_MouseClick::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CEffects_MouseClick::Initialize(void* pArg)
{
	if (pArg != nullptr)
		m_vPosition = *static_cast<_vector*>(pArg);

	return S_OK;
}

void CEffects_MouseClick::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Circle"), m_vPosition);
		m_iCount++;
	}
	else if (m_iCount == 1 && m_fTimeAcc > 0.1f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_CircleLine"), m_vPosition);
		m_iCount++;
	}
	else if (m_iCount == 2 && m_fTimeAcc > 0.25f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Leaf"), m_vPosition);
		m_iCount++;
	}
	else if (m_iCount == 3 && m_fTimeAcc > 0.35f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Mouse_Twinkle"), m_vPosition);
		m_iCount++;

		Set_Dead(true);
	}
}

void CEffects_MouseClick::LateTick(_float fTimeDelta)
{
}

HRESULT CEffects_MouseClick::Render()
{
	return S_OK;
}

HRESULT CEffects_MouseClick::Ready_Components()
{
	return S_OK;
}

CEffects_MouseClick* CEffects_MouseClick::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CEffects_MouseClick* pInstance = new CEffects_MouseClick(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffects_MouseClick");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffects_MouseClick::Clone(void* pArg)
{
	CEffects_MouseClick* pInstance = new CEffects_MouseClick(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffects_MouseClick");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CEffects_MouseClick::Free()
{
	__super::Free();
}