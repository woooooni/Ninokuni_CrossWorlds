#include "stdafx.h"
#include "Vfx_Engineer_Skill_BurstCall.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_Engineer_Skill_BurstCall::CVfx_Engineer_Skill_BurstCall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_BurstCall::CVfx_Engineer_Skill_BurstCall(const CVfx_Engineer_Skill_BurstCall& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_BurstCall::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE]    = 10;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 1.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(10.f, 3.f, 10.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_LINE]    = 15;
		m_pPositionOffset[TYPE_ET1_E_LINE] = _float3(0.f, 0.f, 10.f);
		m_pScaleOffset[TYPE_ET1_E_LINE]    = _float3(1.f, 50.f, 1.f);
		m_pRotationOffset[TYPE_ET1_E_LINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 18;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(-0.7f, 7.7f, 0.5f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 18;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(-0.15f, 1.5f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	// Event : Bullet Start
	m_pFrameTriger[TYPE_EVENT_LINE] = 25;
	m_pPositionOffset[TYPE_EVENT_LINE] = _float3(2.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_EVENT_LINE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_EVENT_LINE] = _float3(0.f, 0.f, 0.f);

	{
		m_pFrameTriger[TYPE_ET2_D_CRACK] = 30;
		m_pPositionOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.8f);
		m_pScaleOffset[TYPE_ET2_D_CRACK]    = _float3(12.f, 3.f, 12.f);
		m_pRotationOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE_BIG] = 30;
		m_pPositionOffset[TYPE_ET2_P_FIRE_BIG] = _float3(0.f, 1.f, 10.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE_BIG]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE_BIG] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE_SMALL] = 30;
		m_pPositionOffset[TYPE_ET2_P_FIRE_SMALL] = _float3(0.f, 0.f, 10.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE_SMALL]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE_SMALL] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 30;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.5f, 20.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 30;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 10.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_STONE] = 30;
		m_pPositionOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 10.f);
		m_pScaleOffset[TYPE_ET2_P_STONE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 0.f);

		//
		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 30;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_BurstCall::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_BurstCall::Tick(_float fTimeDelta)
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
				m_fMainColor = _float3(1.000f, 0.342f, 0.000f);
				m_fLightColor = _float3(1.000f, 0.670f, 0.446f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.f, 1.f, 0.953f);
				m_fLightColor = _float3(0.544f, 0.964f, 0.879f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.655f, 0.896f, 0.293f);
				m_fLightColor = _float3(0.79f, 0.938f, 0.569f);
				break;
			}
		}

		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE], nullptr, &m_pEt1_Decal, false);
			Safe_AddRef(m_pEt1_Decal);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_LINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_LINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_BurstCall_Line"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_LINE], m_pScaleOffset[TYPE_ET1_E_LINE], m_pRotationOffset[TYPE_ET1_E_LINE], nullptr, &m_pEt1_Line_01, false);
			if (nullptr != m_pEt1_Line_01)
			{
				Safe_AddRef(m_pEt1_Line_01);
				m_pEt1_Line_01->Set_Color(m_fMainColor);
				//m_pEt1_Line_01->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_BurstCall_Line"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_LINE], m_pScaleOffset[TYPE_ET1_E_LINE], m_pRotationOffset[TYPE_ET1_E_LINE], nullptr, &m_pEt1_Line_02, false);
			if (nullptr != m_pEt1_Line_02)
			{
				Safe_AddRef(m_pEt1_Line_02);
				m_pEt1_Line_02->Set_Color(m_fMainColor);
				//m_pEt1_Line_02->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}

			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_BurstCall_CircleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fLightColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fLightColor);
			}
			m_iCount++;
		}
		// -------------------------------------------------------------------------------------------------------------
		else if (m_iCount == TYPE_EVENT_LINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_EVENT_LINE])
		{
			if (nullptr != m_pEt1_Line_01)
			{
				m_pEt1_Line_01->Set_UVLoop(1);
				m_pEt1_Line_01->Set_UVSpeed(m_pPositionOffset[TYPE_EVENT_LINE].x);
				Safe_Release(m_pEt1_Line_01);
			}

			if (nullptr != m_pEt1_Line_02)
			{
				m_pEt1_Line_02->Set_UVLoop(1);
				m_pEt1_Line_02->Set_UVSpeed(m_pPositionOffset[TYPE_EVENT_LINE].x);
				Safe_Release(m_pEt1_Line_02);
			}

			m_iCount++;
		}
		// -------------------------------------------------------------------------------------------------------------
		else if (m_iCount == TYPE_ET2_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CRACK])
		{
			if (nullptr != m_pEt1_Decal)
			{
				m_pEt1_Decal->Set_Dead(true);
				Safe_Release(m_pEt1_Decal);
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Engineer_Skill_BurstCall_Crack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_CRACK], m_pScaleOffset[TYPE_ET2_D_CRACK], m_pRotationOffset[TYPE_ET2_D_CRACK]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRE_BIG && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE_BIG])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Fire_Big"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRE_BIG], m_pScaleOffset[TYPE_ET2_P_FIRE_BIG], m_pRotationOffset[TYPE_ET2_P_FIRE_BIG]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRE_SMALL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE_SMALL])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Fire_Small"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRE_SMALL], m_pScaleOffset[TYPE_ET2_P_FIRE_SMALL], m_pRotationOffset[TYPE_ET2_P_FIRE_SMALL]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_BurstCall_CircleLine_Big"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINE], m_pScaleOffset[TYPE_ET2_E_CIRCLELINE], m_pRotationOffset[TYPE_ET2_E_CIRCLELINE], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fLightColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.25f), CUtils::Random_Float(0.f, 0.25f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
		    CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Circles_Rigid"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fLightColor);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_STONE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Stone"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_STONE], m_pScaleOffset[TYPE_ET2_P_STONE], m_pRotationOffset[TYPE_ET2_P_STONE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
		{
			//GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT(""),
			//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_SMOKE], m_pScaleOffset[TYPE_ET2_P_SMOKE], m_pRotationOffset[TYPE_ET2_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_BurstCall::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_BurstCall::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_BurstCall::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_BurstCall* CVfx_Engineer_Skill_BurstCall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_BurstCall* pInstance = new CVfx_Engineer_Skill_BurstCall(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_BurstCall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_BurstCall::Clone(void* pArg)
{
	CVfx_Engineer_Skill_BurstCall* pInstance = new CVfx_Engineer_Skill_BurstCall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_BurstCall::Free()
{
	__super::Free();

	if (nullptr != m_pEt1_Decal)
	{
		m_pEt1_Decal->Set_Dead(true);
		Safe_Release(m_pEt1_Decal);
	}

	if (nullptr != m_pEt1_Line_01)
	{
		m_pEt1_Line_01->Set_UVLoop(1);
		m_pEt1_Line_01->Set_UVSpeed(m_pPositionOffset[TYPE_EVENT_LINE].x);
		Safe_Release(m_pEt1_Line_01);
	}

	if (nullptr != m_pEt1_Line_02)
	{
		m_pEt1_Line_02->Set_UVLoop(1);
		m_pEt1_Line_02->Set_UVSpeed(m_pPositionOffset[TYPE_EVENT_LINE].x);
		Safe_Release(m_pEt1_Line_02);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}