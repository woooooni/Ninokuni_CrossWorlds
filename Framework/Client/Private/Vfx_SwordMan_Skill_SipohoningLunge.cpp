#include "stdafx.h"
#include "Vfx_SwordMan_Skill_SipohoningLunge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_SipohoningLunge::CVfx_SwordMan_Skill_SipohoningLunge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_SipohoningLunge::CVfx_SwordMan_Skill_SipohoningLunge(const CVfx_SwordMan_Skill_SipohoningLunge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_1;
	m_iMaxCount = 15;

	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0 : Decal_Swordman_Skill_SipohoningLunge_Circle
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.2f);
	m_pScaleOffset[0]    = _float3(10.f, 1.f, 10.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

	// 1 : Particle_Swordman_Skill_SipohoningLunge_Hand_FlashSmall
	m_pFrameTriger[1]    = 8;
	m_pPositionOffset[1] = _float3(0.25f, 1.2f, -0.5f);
	m_pScaleOffset[1]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[1] = _float3(0.f, 0.f, 0.f);

	// 2 : Effect_Swordman_Skill_SipohoningLunge_Trail_In
	m_pFrameTriger[2]    = 9;
	m_pPositionOffset[2] = _float3(0.3f, 0.1f, -0.6f);
	m_pScaleOffset[2]    = _float3(1.5f, 1.5f, 1.5f);
	m_pRotationOffset[2] = _float3(90.f, 30.f, 0.f);

	// 3 : Decal_Swordman_Skill_SipohoningLunge_Square
	m_pFrameTriger[3]    = 15;
	m_pPositionOffset[3] = _float3(0.f, 0.f, 0.25f);
	m_pScaleOffset[3]    = _float3(4.f, 1.f, 8.f);
	m_pRotationOffset[3] = _float3(0.f, 0.f, 0.f);

	// 4 : Particle_Swordman_Skill_SipohoningLunge_Hand_FlashBig
	m_pFrameTriger[4]    = 18;
	m_pPositionOffset[4] = _float3(0.f, 2.2f, 0.f);
	m_pScaleOffset[4]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[4] = _float3(0.f, 0.f, 0.f);

	// 5 : Effect_Swordman_Skill_SipohoningLunge_CirecleLine_HandSmall
	m_pFrameTriger[5]    = 18;
	m_pPositionOffset[5] = _float3(0.f, 0.f, -1.5f);
	m_pScaleOffset[5]    = _float3(1.5f, 1.5f, 1.5f);
	m_pRotationOffset[5] = _float3(90.f, 0.f, 0.f);

	// 6 : Effect_Swordman_Skill_SipohoningLunge_Attack
	m_pFrameTriger[6]    = 19;
	m_pPositionOffset[6] = _float3(-0.8f, 0.5f, 0.f);
	m_pScaleOffset[6]    = _float3(3.f, 4.f, 3.f);
	m_pRotationOffset[6] = _float3(0.f, 90.f, 0.f);

	// 7 :Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_01
	m_pFrameTriger[7]    = 19;
	m_pPositionOffset[7] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[7]    = _float3(0.5f, 0.4f, 0.5f);
	m_pRotationOffset[7] = _float3(0.f, 0.f, 0.f);

	// 8 : Particle_Swordman_Skill_SipohoningLunge_Sparkle_Circle
	m_pFrameTriger[8]    = 20;
	m_pPositionOffset[8] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[8]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[8] = _float3(0.f, 0.f, 0.f);

	// 9 : Particle_Swordman_Skill_SipohoningLunge_Rock
	m_pFrameTriger[9]    = 21;
	m_pPositionOffset[9] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[9]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[9] = _float3(0.f, 0.f, 0.f);

	// 10 : Decal_Swordman_Skill_SipohoningLunge_Crack
	m_pFrameTriger[10]    = 38;
	m_pPositionOffset[10] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[10]    = _float3(8.f, 1.f, 8.f);
	m_pRotationOffset[10] = _float3(0.f, 0.f, 0.f);

	// 11 : Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_01
	m_pFrameTriger[11]    = 41;
	m_pPositionOffset[11] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[11]    = _float3(0.5f, 0.4f, 0.5f);
	m_pRotationOffset[11] = _float3(0.f, 0.f, 0.f);

	// 12 : Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_02
	m_pFrameTriger[12]    = 43;
	m_pPositionOffset[12] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[12]    = _float3(0.5f, 0.2f, 0.5f);
	m_pRotationOffset[12] = _float3(0.f, 0.f, 0.f);

	// 13 : Particle_Swordman_Skill_SipohoningLunge_Sparkle_Circle
	m_pFrameTriger[13]    = 0;
	m_pPositionOffset[13] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[13]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[13] = _float3(0.f, 0.f, 0.f);

	// 14 : Particle_Swordman_Skill_SipohoningLunge_Rock
	m_pFrameTriger[14]    = 0;
	m_pPositionOffset[14] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[14]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[14] = _float3(0.f, 0.f, 0.f);
	 
 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_SipohoningLunge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// Decal_Swordman_Skill_SipohoningLunge_Circle
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_SipohoningLunge_Circle"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0]);
			m_iCount++;
		}
		// *Particle_Swordman_Skill_SipohoningLunge_Hand_FlashSmall
		else if (m_iCount == 1 && m_iOwnerFrame >= m_pFrameTriger[1]) 
		{
			//GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SipohoningLunge_Hand_FlashSmall"), 
			//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[1], m_pScaleOffset[1], m_pRotationOffset[1]);
			m_iCount++;
		}
		// Effect_Swordman_Skill_SipohoningLunge_Trail_In
		else if (m_iCount == 2 && m_iOwnerFrame >= m_pFrameTriger[2])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SipohoningLunge_Trail_In"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[2], m_pScaleOffset[2], m_pRotationOffset[2]);
			m_iCount++;
		}
		// Decal_Swordman_Skill_SipohoningLunge_Square
		else if (m_iCount == 3 && m_iOwnerFrame >= m_pFrameTriger[3])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_SipohoningLunge_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[3], m_pScaleOffset[3], m_pRotationOffset[3]);
			m_iCount++;
		}

		// -------------------------------------------------------------------------------------------

		// *Particle_Swordman_Skill_SipohoningLunge_Hand_FlashBig
		else if (m_iCount == 4 && m_iOwnerFrame >= m_pFrameTriger[4])
		{
			//GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SipohoningLunge_Hand_FlashBig"),
			//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[4], m_pScaleOffset[4], m_pRotationOffset[4]);
			m_iCount++;
		}
		// Effect_Swordman_Skill_SipohoningLunge_CirecleLine_HandSmall
		else if (m_iCount == 5 && m_iOwnerFrame >= m_pFrameTriger[5])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SipohoningLunge_CirecleLine_HandSmall"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[5], m_pScaleOffset[5], m_pRotationOffset[5]);
			m_iCount++;
		}
		
		// Effect_Swordman_Skill_SipohoningLunge_Attack
		else if (m_iCount == 6 && m_iOwnerFrame >= m_pFrameTriger[6])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SipohoningLunge_Attack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[6], m_pScaleOffset[6], m_pRotationOffset[6]);
			m_iCount++;
		}
		// Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_01
		else if (m_iCount == 7 && m_iOwnerFrame >= m_pFrameTriger[7]) // Why
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[7], m_pScaleOffset[7], m_pRotationOffset[7]);
			m_iCount++;
		}
		// Particle_Swordman_Skill_SipohoningLunge_Sparkle_Circle
		else if (m_iCount == 8 && m_iOwnerFrame >= m_pFrameTriger[8])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SipohoningLunge_Sparkle_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[8], m_pScaleOffset[8], m_pRotationOffset[8]);
			m_iCount++;
		}
		// Particle_Swordman_Skill_SipohoningLunge_Rock
		else if (m_iCount == 9 && m_iOwnerFrame >= m_pFrameTriger[9])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SipohoningLunge_Rock"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[9], m_pScaleOffset[9], m_pRotationOffset[9]);
			m_iCount++;
		}

		// -------------------------------------------------------------------------------------------

		// Decal_Swordman_Skill_SipohoningLunge_Crack
		else if (m_iCount == 10 && m_iOwnerFrame >= m_pFrameTriger[10])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_SipohoningLunge_Crack"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[10], m_pScaleOffset[10], m_pRotationOffset[10]);
			m_iCount++;
		}
		// Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_01
		else if (m_iCount == 11 && m_iOwnerFrame >= m_pFrameTriger[11])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[11], m_pScaleOffset[11], m_pRotationOffset[11]);
			m_iCount++;
		}
		// Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_02
		else if (m_iCount == 12 && m_iOwnerFrame >= m_pFrameTriger[12])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SipohoningLunge_CirecleLine_Attack_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[12], m_pScaleOffset[12], m_pRotationOffset[12]);
			m_iCount++;
		}

		// Particle_Swordman_Skill_SipohoningLunge_Sparkle_Circle
		else if (m_iCount == 13 && m_iOwnerFrame >= m_pFrameTriger[13])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SipohoningLunge_Sparkle_Circle"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[13], m_pScaleOffset[13], m_pRotationOffset[13]);
			m_iCount++;
		}
		// Particle_Swordman_Skill_SipohoningLunge_Rock
		else if (m_iCount == 14 && m_iOwnerFrame >= m_pFrameTriger[14])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SipohoningLunge_Rock"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[14], m_pScaleOffset[14], m_pRotationOffset[14]);
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 15)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_SipohoningLunge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SipohoningLunge::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_SipohoningLunge* CVfx_SwordMan_Skill_SipohoningLunge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_SipohoningLunge* pInstance = new CVfx_SwordMan_Skill_SipohoningLunge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_SipohoningLunge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_SipohoningLunge::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_SipohoningLunge* pInstance = new CVfx_SwordMan_Skill_SipohoningLunge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SipohoningLunge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_SipohoningLunge::Free()
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