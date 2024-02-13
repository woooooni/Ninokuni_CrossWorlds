#include "stdafx.h"
#include "Vfx_Witch_Marble_Hit.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

CVfx_Witch_Marble_Hit::CVfx_Witch_Marble_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Marble_Hit::CVfx_Witch_Marble_Hit(const CVfx_Witch_Marble_Hit& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Marble_Hit::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pPositionOffset[TYPE_E_LINE] = _float3(0.f, -0.2f, 0.f);
	m_pScaleOffset[TYPE_E_LINE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_LINE] = _float3(0.f, 0.f, 0.f);

	m_pPositionOffset[TYPE_P_CIRCLE] = _float3(0.f, -0.2f, 0.f);
	m_pScaleOffset[TYPE_P_CIRCLE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Witch_Marble_Hit::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Marble_Hit::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	CEffect* pEffect = nullptr;
	GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Marble_Hit"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_LINE], m_pScaleOffset[TYPE_E_LINE], m_pRotationOffset[TYPE_E_LINE], nullptr, &pEffect);
	if (nullptr != pEffect)
	{
		pEffect->Reserve_Dissolve(11,    // Index
			_float4(1.f, 1.f, 1.f, 1.f), // Color
			10.f,  // Speed
			10.f); // Total
	}

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_Marble_Hit_Circle"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLE], m_pScaleOffset[TYPE_P_CIRCLE], m_pRotationOffset[TYPE_P_CIRCLE]);
	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_Marble_Hit_Circle02"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLE], m_pScaleOffset[TYPE_P_CIRCLE], m_pRotationOffset[TYPE_P_CIRCLE]);

	Set_Dead(true);
}

void CVfx_Witch_Marble_Hit::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Marble_Hit::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Marble_Hit::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Marble_Hit* CVfx_Witch_Marble_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Marble_Hit* pInstance = new CVfx_Witch_Marble_Hit(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Marble_Hit");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Marble_Hit::Clone(void* pArg)
{
	CVfx_Witch_Marble_Hit* pInstance = new CVfx_Witch_Marble_Hit(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Marble_Hit");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Marble_Hit::Free()
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