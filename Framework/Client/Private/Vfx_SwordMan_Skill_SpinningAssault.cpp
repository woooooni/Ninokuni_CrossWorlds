#include "stdafx.h"
#include "Vfx_SwordMan_Skill_SpinningAssault.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_SwordMan_Skill_SpinningAssault::CVfx_SwordMan_Skill_SpinningAssault(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_SpinningAssault::CVfx_SwordMan_Skill_SpinningAssault(const CVfx_SwordMan_Skill_SpinningAssault& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_2;
	m_iMaxCount = 11;

	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0 : 데칼
	m_pFrameTriger[0]    = 0;
	m_pPositionOffset[0] = _float3(0.f, 0.f, 0.2f);
	m_pScaleOffset[0]    = _float3(4.f, 5.f, 4.f);
	m_pRotationOffset[0] = _float3(0.f, 0.f, 0.f);

	// 1 : 트레일 1
	m_pFrameTriger[1]    = 14;
	m_pPositionOffset[1] = _float3(0.f, 0.15f, 0.f);
	m_pScaleOffset[1]    = _float3(4.f, 3.f, 4.f);
	m_pRotationOffset[1] = _float3(0.f, 0.f, 0.f);

	// 2 : 트레일 2
	m_pFrameTriger[2]    = 15;
	m_pPositionOffset[2] = _float3(0.f, 0.15f, 0.f);
	m_pScaleOffset[2]    = _float3(4.2f, 3.f, 4.2f);
	m_pRotationOffset[2] = _float3(0.f, 0.f, 0.f);

	// 3. 이벤트 1
	m_pFrameTriger[3]    = 16;
	m_pPositionOffset[3] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[3]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[3] = _float3(0.f, 0.f, 0.f);

	// 4. 이벤트 2
	m_pFrameTriger[4]    = 21;
	m_pPositionOffset[4] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[4]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[4] = _float3(0.f, 0.f, 0.f);

	// 5 : 파티클 불 1
	m_pFrameTriger[5]    = 24;
	m_pPositionOffset[5] = _float3(-0.6f, 1.f, 0.f);
	m_pScaleOffset[5]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[5] = _float3(0.f, 0.f, 0.f);

	// 6 : 파티클 불 2
	m_pFrameTriger[6]    = 24;
	m_pPositionOffset[6] = _float3(-0.3f, 1.f, 0.4f);
	m_pScaleOffset[6]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[6] = _float3(0.f, 0.f, 0.f);

	// 7 : 파티클 불 3
	m_pFrameTriger[7]    = 24;
	m_pPositionOffset[7] = _float3(0.f, 1.f, 0.8f);
	m_pScaleOffset[7]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[7] = _float3(0.f, 0.f, 0.f);

	// 8 : 파티클 불 4
	m_pFrameTriger[8]    = 24;
	m_pPositionOffset[8] = _float3(0.3f, 1.f, 0.4f);
	m_pScaleOffset[8]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[8] = _float3(0.f, 0.f, 0.f);

	// 9 : 파티클 불 5
	m_pFrameTriger[9]    = 24;
	m_pPositionOffset[9] = _float3(0.6f, 1.f, 0.f);
	m_pScaleOffset[9]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[9] = _float3(0.f, 0.f, 0.f);

	// 10 : 파티클 원 1
	m_pFrameTriger[10]    = 27;
	m_pPositionOffset[10] = _float3(0.f, 0.5f, 0.f);
	m_pScaleOffset[10]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[10] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_SpinningAssault::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (-1 == m_iType)
		{
			CCharacter* pPlayer = static_cast<CCharacter*>(m_pOwnerObject);
			if (nullptr == pPlayer)
				MSG_BOX("Casting_Failde");
			else
				m_iType = pPlayer->Get_ElementalType();

			switch (m_iType)
			{
			case ELEMENTAL_TYPE::FIRE:
				m_fMainColor = _float3(0.881f, 0.263f, 0.023f);
				m_fLightColor = _float3(1.000, 0.68, 0.44);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.4f, 0.8f, 0.9f);
				m_fLightColor = _float3(0.55f, 0.85f, 1.f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.3f, 1.f, 0.5f);
				m_fLightColor = _float3(0.55f, 1.f, 0.7f);
				break;
			}
		}

		// 데칼
		if (m_iCount == 0 && m_iOwnerFrame >= m_pFrameTriger[0])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_SpinningAssault_Circle"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[0], m_pScaleOffset[0], m_pRotationOffset[0], m_pOwnerObject);
			m_iCount++;
		}

		// 이펙트
		else if (m_iCount == 1 && m_iOwnerFrame >= m_pFrameTriger[1])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SpinningAssault_Trail_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[1], m_pScaleOffset[1], m_pRotationOffset[1], m_pOwnerObject, &m_pEffect_Trail0);
			if (nullptr != m_pEffect_Trail0)
			{
				Safe_AddRef(m_pEffect_Trail0);
				m_pEffect_Trail0->Set_Color(m_fMainColor);
				m_pEffect_Trail0->Set_DistortionPower(CUtils::Random_Float(0.f, 0.25f), CUtils::Random_Float(0.f, 0.25f));
			}
			m_iCount++;
		}
		else if (m_iCount == 2 && m_iOwnerFrame >= m_pFrameTriger[2])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_SpinningAssault_Trail_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[2], m_pScaleOffset[2], m_pRotationOffset[2], m_pOwnerObject, &m_pEffect_Trail1);
			if (nullptr != m_pEffect_Trail1)
			{
				Safe_AddRef(m_pEffect_Trail1);
				m_pEffect_Trail1->Set_Color(m_fLightColor);
			}
			m_iCount++;
		}

		// 이벤트
		else if (m_iCount == 3 && m_iOwnerFrame >= m_pFrameTriger[3])
		{
			m_pEffect_Trail1->Set_UVLoop(1);
			Safe_Release(m_pEffect_Trail1);
			m_iCount++;
		}
		else if (m_iCount == 4 && m_iOwnerFrame >= m_pFrameTriger[4])
		{
			m_pEffect_Trail0->Set_UVLoop(1);
			Safe_Release(m_pEffect_Trail0);
			m_iCount++;
		}

		// 파티클
		else if (m_iCount == 5 && m_iOwnerFrame >= m_pFrameTriger[5])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SpinningAssault_Trail_Fire"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[5], m_pScaleOffset[5], m_pRotationOffset[5], m_pOwnerObject);
			m_iCount++;
		}
		else if (m_iCount == 6 && m_iOwnerFrame >= m_pFrameTriger[6])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SpinningAssault_Trail_Fire"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[6], m_pScaleOffset[6], m_pRotationOffset[6], m_pOwnerObject);
			m_iCount++;
		}
		else if (m_iCount == 7 && m_iOwnerFrame >= m_pFrameTriger[7])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SpinningAssault_Trail_Fire"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[7], m_pScaleOffset[7], m_pRotationOffset[7], m_pOwnerObject);
			m_iCount++;
		}
		else if (m_iCount == 8 && m_iOwnerFrame >= m_pFrameTriger[8])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SpinningAssault_Trail_Fire"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[8], m_pScaleOffset[8], m_pRotationOffset[8], m_pOwnerObject);
			m_iCount++;
		}
		else if (m_iCount == 9 && m_iOwnerFrame >= m_pFrameTriger[9])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SpinningAssault_Trail_Fire"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[9], m_pScaleOffset[9], m_pRotationOffset[9], m_pOwnerObject);
			m_iCount++;
		}

		else if (m_iCount == 10 && m_iOwnerFrame >= m_pFrameTriger[10])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_SpinningAssault_Trail_Sparkle_Circle"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[10], m_pScaleOffset[10], m_pRotationOffset[10], m_pOwnerObject, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fLightColor);
			}
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 11)
			m_bFinish = true;
	}
}

void CVfx_SwordMan_Skill_SpinningAssault::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_SpinningAssault* CVfx_SwordMan_Skill_SpinningAssault::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_SpinningAssault* pInstance = new CVfx_SwordMan_Skill_SpinningAssault(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_SpinningAssault::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_SpinningAssault* pInstance = new CVfx_SwordMan_Skill_SpinningAssault(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_SpinningAssault::Free()
{
	__super::Free();

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}

	if (m_pEffect_Trail0 != nullptr)
		Safe_Release(m_pEffect_Trail0);

	if (m_pEffect_Trail1 != nullptr)
		Safe_Release(m_pEffect_Trail1);
}