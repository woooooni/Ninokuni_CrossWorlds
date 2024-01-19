#include "stdafx.h"
#include "Vfx_Glanix_Skill_Icicle.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_Icicle::CVfx_Glanix_Skill_Icicle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_Icicle::CVfx_Glanix_Skill_Icicle(const CVfx_Glanix_Skill_Icicle& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_Icicle::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	m_pFrameTriger[TYPE_ONE]    = 0;
	m_pPositionOffset[TYPE_ONE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_ONE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ONE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_TWO]    = 0;
	m_pPositionOffset[TYPE_TWO] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_TWO]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_TWO] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_Icicle::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_Icicle::Tick(_float fTimeDelta)
{
	if (m_bFinish)
	{
		Set_Dead(true);
		return;
	}

	if (m_iCount == TYPE_ONE)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Snow_Sharp_Small"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ONE], m_pScaleOffset[TYPE_ONE], m_pRotationOffset[TYPE_ONE]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_TWO)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Snow_Stone_Basic_Small"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_TWO], m_pScaleOffset[TYPE_TWO], m_pRotationOffset[TYPE_TWO]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_END)
		m_bFinish = true;
}

void CVfx_Glanix_Skill_Icicle::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_Icicle::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_Icicle::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_Icicle* CVfx_Glanix_Skill_Icicle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_Icicle* pInstance = new CVfx_Glanix_Skill_Icicle(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_Icicle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_Icicle::Clone(void* pArg)
{
	CVfx_Glanix_Skill_Icicle* pInstance = new CVfx_Glanix_Skill_Icicle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_IcePillar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_Icicle::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}