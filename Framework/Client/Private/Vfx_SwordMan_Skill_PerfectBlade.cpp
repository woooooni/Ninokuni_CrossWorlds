#include "stdafx.h"
#include "Vfx_SwordMan_Skill_PerfectBlade.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_PerfectBlade::CVfx_SwordMan_Skill_PerfectBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_PerfectBlade::CVfx_SwordMan_Skill_PerfectBlade(const CVfx_SwordMan_Skill_PerfectBlade& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;
	m_iMaxCount = 12;

	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0 : Decal 1
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.2f);
	m_pScaleOffset[0]    = _float3(5.f, 1.f, 5.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

	// 1 : TrailL 1
	m_pFrameTriger[1]    = 15;
	m_pPositionOffset[1] = _float3(0.0f, 0.28f, 0.f);
	m_pScaleOffset[1]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[1] = _float3(0.f, 35.f, 0.f);

	// 2 : TrailL 2
	m_pFrameTriger[2]    = 22;
	m_pPositionOffset[2] = _float3(0.05f, 0.3f, 0.f);
	m_pScaleOffset[2]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[2] = _float3(0.f, -25.f, 25.f);

	// 3 : TrailL 3
	m_pFrameTriger[3]    = 28;
	m_pPositionOffset[3] = _float3(0.25f, 0.4f, 0.f);
	m_pScaleOffset[3]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[3] = _float3(0.f, 25.f, 45.f);

	// 4 : TrailL 4
	m_pFrameTriger[4]    = 34;
	m_pPositionOffset[4] = _float3(-0.25f, 0.4f, 0.f);
	m_pScaleOffset[4]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[4] = _float3(0.f, -25.f, -45.f);

	// 5 : TrailL 5
	m_pFrameTriger[5]    = 39;
	m_pPositionOffset[5] = _float3(-0.1f, 0.28f, 0.f);
	m_pScaleOffset[5]    = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[5] = _float3(0.f, 25.f, -35.f);

	// 6 : Sparkle 1
	m_pFrameTriger[6]    = 45;
	m_pPositionOffset[6] = _float3(-0.2f, 0.5f, -0.1f);
	m_pScaleOffset[6]    = _float3(4.f, 1.f, 4.f);
	m_pRotationOffset[6] = _float3(0.f, 0.f, 0.f);

	// 7 : Special_TrailR 6
	m_pFrameTriger[7]    = 47;
	m_pPositionOffset[7] = _float3(0.1f, 0.25f, 0.f);
	m_pScaleOffset[7]    = _float3(6.f, 6.f, 6.f);
	m_pRotationOffset[7] = _float3(0.f, -25.f, 20.f);

	// 8 : Sparkle_Circle
	m_pFrameTriger[8]    = 47;
	m_pPositionOffset[8] = _float3(0.f, 0.2f, 0.05f);
	m_pScaleOffset[8]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[8] = _float3(0.f, 0.f, 0.f);

	// 9 : Fire 1
	m_pFrameTriger[9]    = 48;
	m_pPositionOffset[9] = _float3(-1.f, 0.5f, 2.f);
	m_pScaleOffset[9]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[9] = _float3(0.f, 0.f, 0.f);

	// 10 : Fire 2
	m_pFrameTriger[10]    = 49;
	m_pPositionOffset[10] = _float3(0.f, 1.f, 3.f);
	m_pScaleOffset[10]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[10] = _float3(0.f, 0.f, 0.f);

	// 11 : Fire 3
	m_pFrameTriger[11]    = 50;
	m_pPositionOffset[11] = _float3(1.f, 1.5f, 2.f);
	m_pScaleOffset[11]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[11] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_PerfectBlade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// Decal 1
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}

		// TrailL 1
		else if (m_iCount == 1 && m_iOwnerFrame >= m_pFrameTriger[1])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				XMLoadFloat4x4(&m_WorldMatrix),
				::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
				_float3(5.f, 5.f, 5.f),
				_float3(0.f, 20.f, 0.f), 
				m_pOwnerObject);
			
			m_iCount++;
		}
		// TrailR 2
		else if (m_iCount == 2 && m_iOwnerFrame >= m_pFrameTriger[2])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[2], m_pScaleOffset[2], m_pRotationOffset[2]);
			m_iCount++;
		}
		// TrailL 3
		else if (m_iCount == 3 && m_iOwnerFrame >= m_pFrameTriger[3])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[3], m_pScaleOffset[3], m_pRotationOffset[3]);
			m_iCount++;
		}
		// TrailR 4
		else if (m_iCount == 4 && m_iOwnerFrame >= m_pFrameTriger[4])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[4], m_pScaleOffset[4], m_pRotationOffset[4]);
			m_iCount++;
		}
		// TrailL 5
		else if (m_iCount == 5 && m_iOwnerFrame >= m_pFrameTriger[5])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[5], m_pScaleOffset[5], m_pRotationOffset[5]);
			m_iCount++;
		}

		// Sparkle 1
		else if (m_iCount == 6 && m_iOwnerFrame >= m_pFrameTriger[6])
		{
			// 왼쪽 뒤
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[6], m_pScaleOffset[6], m_pRotationOffset[6]);
			m_iCount++;
		}

		// Special_TrailR 6
		else if (m_iCount == 7 && m_iOwnerFrame >= m_pFrameTriger[7])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[7], m_pScaleOffset[7], m_pRotationOffset[7]);
			m_iCount++;

		}
		// Sparkle_Circle
		else if (m_iCount == 8 && m_iOwnerFrame >= m_pFrameTriger[8])
		{
			// 앞쪽 위
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle_Circle"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[8], m_pScaleOffset[8], m_pRotationOffset[8]);
			m_iCount++;
		}

		// Fire 1
		else if (m_iCount == 9 && m_iOwnerFrame >= m_pFrameTriger[9])
		{
			// 왼쪽 뒤
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[9], m_pScaleOffset[9], m_pRotationOffset[9]);
			m_iCount++;
		}
		// Fire 2
		else if (m_iCount == 10 && m_iOwnerFrame >= m_pFrameTriger[10])
		{
			// 중심
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[10], m_pScaleOffset[10], m_pRotationOffset[10]);
			m_iCount++;
		}
		// Fire 3
		else if (m_iCount == 11 && m_iOwnerFrame >= m_pFrameTriger[11])
		{
			// 오른쪽 뒤
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[11], m_pScaleOffset[11], m_pRotationOffset[11]);
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 12)
		    m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_PerfectBlade::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_PerfectBlade* CVfx_SwordMan_Skill_PerfectBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_PerfectBlade* pInstance = new CVfx_SwordMan_Skill_PerfectBlade(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_PerfectBlade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_PerfectBlade::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_PerfectBlade* pInstance = new CVfx_SwordMan_Skill_PerfectBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_PerfectBlade");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_PerfectBlade::Free()
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