#include "stdafx.h"
#include "Vfx_Destroyer_Skill_FrengeCharge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_Destroyer_Skill_FrengeCharge::CVfx_Destroyer_Skill_FrengeCharge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_FrengeCharge::CVfx_Destroyer_Skill_FrengeCharge(const CVfx_Destroyer_Skill_FrengeCharge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_E_TRAIL] = 5;
		m_pPositionOffset[TYPE_ET1_E_TRAIL] = _float3(0.f, 2.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_TRAIL]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET1_E_TRAIL] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 5;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_ELECT] = 15;
		m_pPositionOffset[TYPE_ET2_P_ELECT] = _float3(0.f, 2.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_ELECT]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_ELECT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_E_CIRCLELINE] = 27;
		m_pPositionOffset[TYPE_ET3_E_CIRCLELINE] = _float3(-0.5f, 2.6f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_CIRCLELINE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES_01] = 27;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES_01] = _float3(-0.5f, 2.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES_01]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES_02] = 27;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES_02] = _float3(-0.5f, 2.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES_02]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES_02] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_FrengeCharge::Tick(_float fTimeDelta)
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
				m_fMainColor = _float3(0.979f, 0.589f, 0.325f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.293f, 0.896f, 0.774f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.655f, 0.896f, 0.293f);
				break;
			}
		}

		if (m_iCount == TYPE_ET1_E_TRAIL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_TRAIL])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_FrengeCharge_Trail"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_TRAIL], m_pScaleOffset[TYPE_ET1_E_TRAIL], m_pRotationOffset[TYPE_ET1_E_TRAIL], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));

			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_FrengeCharge_Circles_01_Twinkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_ELECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_ELECT])
		{
			_int iRandomCount = CUtils::Random_Int(1, 9);
			wstring FileNameString = L"Particle_Destroyer_Skill_FrengeCharge_ThunderLine_0" + to_wstring(iRandomCount);

			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(FileNameString,
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_ELECT], m_pScaleOffset[TYPE_ET2_P_ELECT], m_pRotationOffset[TYPE_ET2_P_ELECT], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(_float3(
					min(m_fMainColor.x + CUtils::Random_Float(0.f, 0.2f), 1.f),
					min(m_fMainColor.y + CUtils::Random_Float(0.f, 0.2f), 1.f),
					min(m_fMainColor.z + CUtils::Random_Float(0.f, 0.2f), 1.f)));
			}
			m_iCount++;
		}
		// ------------------------------------------------------------------------------------------------------------
		else if (m_iCount == TYPE_ET3_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_CIRCLELINE])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_FrengeCharge_CirecleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_CIRCLELINE], m_pScaleOffset[TYPE_ET3_E_CIRCLELINE], m_pRotationOffset[TYPE_ET3_E_CIRCLELINE], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));

			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES_01])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_FrengeCharge_Circles_01_Rigid"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES_01], m_pScaleOffset[TYPE_ET3_P_CIRCLES_01], m_pRotationOffset[TYPE_ET3_P_CIRCLES_01]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES_02])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_FrengeCharge_Circles_02_Rigid"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES_02], m_pScaleOffset[TYPE_ET3_P_CIRCLES_02], m_pRotationOffset[TYPE_ET3_P_CIRCLES_02]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_FrengeCharge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_FrengeCharge::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_FrengeCharge* CVfx_Destroyer_Skill_FrengeCharge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_FrengeCharge* pInstance = new CVfx_Destroyer_Skill_FrengeCharge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_FrengeCharge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_FrengeCharge::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_FrengeCharge* pInstance = new CVfx_Destroyer_Skill_FrengeCharge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_FrengeCharge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_FrengeCharge::Free()
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