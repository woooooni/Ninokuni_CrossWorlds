#include "stdafx.h"
#include "Vfx_Witch_Skill_Rage01QuadBlackHole_Bomb.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb(const CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pPositionOffset[TYPE_P_SMALL] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SMALL]    = _float3(1.5f, 1.5f, 1.5f);
	m_pRotationOffset[TYPE_P_SMALL] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE]    = _float3(1.5f, 1.5f, 1.5f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	m_pPositionOffset[TYPE_E_ELECTLINE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_ELECTLINE]    = _float3(45.f, 45.f, 150.f);
	m_pRotationOffset[TYPE_E_ELECTLINE] = _float3(-90.f, 0.f, 0.f);

	m_pPositionOffset[TYPE_P_ELECTCIRCLE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_ELECTCIRCLE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_ELECTCIRCLE] = _float3(-90.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Tick(_float fTimeDelta)
{
	if (true == m_bDead)
		return;

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_BlackHole_Small"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMALL], m_pScaleOffset[TYPE_P_SMALL], m_pRotationOffset[TYPE_P_SMALL]);

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_BlackHole_BombSmoke"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);


	GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_LaserLine"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_ELECTLINE], m_pScaleOffset[TYPE_E_ELECTLINE], m_pRotationOffset[TYPE_E_ELECTLINE]);

	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_QuadBalckHole_Bomb_Circles"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ELECTCIRCLE], m_pScaleOffset[TYPE_P_ELECTCIRCLE], m_pRotationOffset[TYPE_P_ELECTCIRCLE]);
	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_QuadBalckHole_Bomb_Circles02"),
		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ELECTCIRCLE], m_pScaleOffset[TYPE_P_ELECTCIRCLE], m_pRotationOffset[TYPE_P_ELECTCIRCLE]);

	Set_Dead(true);
}

void CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb* CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb* pInstance = new CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Clone(void* pArg)
{
	CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb* pInstance = new CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_Rage01QuadBlackHole_Bomb::Free()
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