#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage02Explosion.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Stellia_Skill_Rage02Explosion::CVfx_Stellia_Skill_Rage02Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage02Explosion::CVfx_Stellia_Skill_Rage02Explosion(const CVfx_Stellia_Skill_Rage02Explosion& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage02Explosion::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_P_BOMB_X] = 0;
	m_pPositionOffset[TYPE_P_BOMB_X] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_BOMB_X] = _float3(.5f, .5f, .5f);
	m_pRotationOffset[TYPE_P_BOMB_X] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_P_BOMB_Y] = 0;
	m_pPositionOffset[TYPE_P_BOMB_Y] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_BOMB_Y] = _float3(.5f, .5f, .5f);
	m_pRotationOffset[TYPE_P_BOMB_Y] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage02Explosion::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage02Explosion::Tick(_float fTimeDelta)
{

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Rage01_Explosion_X"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_BOMB_X], m_pScaleOffset[TYPE_P_BOMB_X], m_pRotationOffset[TYPE_P_BOMB_X]);

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Rage01_Explosion_Y"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_BOMB_Y], m_pScaleOffset[TYPE_P_BOMB_Y], m_pRotationOffset[TYPE_P_BOMB_Y]);

	Set_Dead(true);
}

void CVfx_Stellia_Skill_Rage02Explosion::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage02Explosion::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage02Explosion::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage02Explosion* CVfx_Stellia_Skill_Rage02Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage02Explosion* pInstance = new CVfx_Stellia_Skill_Rage02Explosion(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage02Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage02Explosion::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage02Explosion* pInstance = new CVfx_Stellia_Skill_Rage02Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage02Explosion");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage02Explosion::Free()
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