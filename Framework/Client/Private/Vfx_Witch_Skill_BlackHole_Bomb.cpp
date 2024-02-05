#include "stdafx.h"
#include "Vfx_Witch_Skill_BlackHole_Bomb.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Witch_Skill_BlackHole_Bomb::CVfx_Witch_Skill_BlackHole_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_BlackHole_Bomb::CVfx_Witch_Skill_BlackHole_Bomb(const CVfx_Witch_Skill_BlackHole_Bomb& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_BlackHole_Bomb::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_P_SMALL] = 0;
	m_pPositionOffset[TYPE_P_SMALL] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SMALL] = _float3(1.5f, 1.5f, 1.5f);
	m_pRotationOffset[TYPE_P_SMALL] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_P_SMOKE] = 0;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE] = _float3(1.5f, 1.5f, 1.5f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Skill_BlackHole_Bomb::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_BlackHole_Bomb::Tick(_float fTimeDelta)
{
	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_BlackHole_Small"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMALL], m_pScaleOffset[TYPE_P_SMALL], m_pRotationOffset[TYPE_P_SMALL]);

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_BlackHole_BombSmoke"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);

	Set_Dead(true);
}

void CVfx_Witch_Skill_BlackHole_Bomb::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_BlackHole_Bomb::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_BlackHole_Bomb::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_BlackHole_Bomb* CVfx_Witch_Skill_BlackHole_Bomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_BlackHole_Bomb* pInstance = new CVfx_Witch_Skill_BlackHole_Bomb(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_BlackHole_Bomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_BlackHole_Bomb::Clone(void* pArg)
{
	CVfx_Witch_Skill_BlackHole_Bomb* pInstance = new CVfx_Witch_Skill_BlackHole_Bomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_BlackHole_Bomb");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_BlackHole_Bomb::Free()
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