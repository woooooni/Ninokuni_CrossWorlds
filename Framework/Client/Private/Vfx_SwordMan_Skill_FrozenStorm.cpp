#include "stdafx.h"
#include "Vfx_SwordMan_Skill_FrozenStorm.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_FrozenStorm::CVfx_SwordMan_Skill_FrozenStorm(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_FrozenStorm::CVfx_SwordMan_Skill_FrozenStorm(const CVfx_SwordMan_Skill_FrozenStorm& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_1;
	m_iMaxCount = TYPE_END;

	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	m_pFrameTriger[TYPE_D_CIRCLE]    = 0;
	m_pPositionOffset[TYPE_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_CIRCLE]    = _float3(7.f, 5.f, 7.f);
	m_pRotationOffset[TYPE_D_CIRCLE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SMOKE]    = 18;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_XSPARKLE]    = 18;
	m_pPositionOffset[TYPE_P_XSPARKLE] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_XSPARKLE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_XSPARKLE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SPARKLE_01]    = 20;
	m_pPositionOffset[TYPE_P_SPARKLE_01] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SPARKLE_01]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SPARKLE_01] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_E_TRAILBASIC]    = 20;
	m_pPositionOffset[TYPE_E_TRAILBASIC] = _float3(0.f, 0.2f, 0.f);
	m_pScaleOffset[TYPE_E_TRAILBASIC]    = _float3(4.f, 4.f, 4.f);
	m_pRotationOffset[TYPE_E_TRAILBASIC] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SNOW]    = 25;
	m_pPositionOffset[TYPE_P_SNOW] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_SNOW]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SNOW] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_E_TRAILLINE]    = 30;
	m_pPositionOffset[TYPE_E_TRAILLINE] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[TYPE_E_TRAILLINE]    = _float3(2.f, 2.f, 2.f);
	m_pRotationOffset[TYPE_E_TRAILLINE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_SPARKLE_02]    = 35;
	m_pPositionOffset[TYPE_P_SPARKLE_02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_P_SPARKLE_02]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SPARKLE_02] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_FrozenStorm::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_CIRCLE], m_pScaleOffset[TYPE_D_CIRCLE], m_pRotationOffset[TYPE_D_CIRCLE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_FrozenStorm_Smoke"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_XSPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_XSPARKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_FrozenStorm_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_XSPARKLE], m_pScaleOffset[TYPE_P_XSPARKLE], m_pRotationOffset[TYPE_P_XSPARKLE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SPARKLE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_FrozenStorm_Sparkle_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_01], m_pScaleOffset[TYPE_P_SPARKLE_01], m_pRotationOffset[TYPE_P_SPARKLE_01]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAILBASIC && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAILBASIC])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_FrozenStorm_Trail"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAILBASIC], m_pScaleOffset[TYPE_E_TRAILBASIC], m_pRotationOffset[TYPE_E_TRAILBASIC]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SNOW && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SNOW])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_FrozenStorm_Snow"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SNOW], m_pScaleOffset[TYPE_P_SNOW], m_pRotationOffset[TYPE_P_SNOW]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAILLINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAILLINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_FrozenStorm_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAILLINE], m_pScaleOffset[TYPE_E_TRAILLINE], m_pRotationOffset[TYPE_E_TRAILLINE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SPARKLE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SPARKLE_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_FrozenStorm_Sparkle_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SPARKLE_02], m_pScaleOffset[TYPE_P_SPARKLE_02], m_pRotationOffset[TYPE_P_SPARKLE_02]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_FrozenStorm::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_FrozenStorm::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_FrozenStorm* CVfx_SwordMan_Skill_FrozenStorm::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_FrozenStorm* pInstance = new CVfx_SwordMan_Skill_FrozenStorm(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_FrozenStorm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_FrozenStorm::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_FrozenStorm* pInstance = new CVfx_SwordMan_Skill_FrozenStorm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_FrozenStorm");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_FrozenStorm::Free()
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