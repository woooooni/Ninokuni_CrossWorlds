#include "stdafx.h"
#include "Vfx_Destroyer_Skill_BattleCry.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_Destroyer_Skill_BattleCry::CVfx_Destroyer_Skill_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_BattleCry::CVfx_Destroyer_Skill_BattleCry(const CVfx_Destroyer_Skill_BattleCry& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_1;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(10.f, 3.f, 10.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	// 12 ¹øÂ½ ½ÃÀÛ
	{
		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 12;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.2f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

	// 13 Æã
	{
		m_pFrameTriger[TYPE_ET3_E_DOME] = 13;
		m_pPositionOffset[TYPE_ET3_E_DOME] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_DOME]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_E_DOME] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_D_FIRECRACK] = 13;
		m_pPositionOffset[TYPE_ET3_D_FIRECRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_D_FIRECRACK]    = _float3(10.f, 3.f, 10.f);
		m_pRotationOffset[TYPE_ET3_D_FIRECRACK] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_E_SPRINGUP] = 13;
		m_pPositionOffset[TYPE_ET3_E_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_SPRINGUP]    = _float3(2.5f, 2.5f, 2.5f);
		m_pRotationOffset[TYPE_ET3_E_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_CIRCLELINE] = 13;
		m_pPositionOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_CIRCLELINE]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_STONE] = 13;
		m_pPositionOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_STONE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 13;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_BattleCry::Tick(_float fTimeDelta)
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
				m_fMainColor = _float3(0.136f, 0.938f, 0.776f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.655f, 0.896f, 0.293f);
				break;
			}
		}

		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE], nullptr, &m_pEt1_Decal, false);
			m_pEt1_Decal->Set_AlphaSpeed(5.f);
			Safe_AddRef(m_pEt1_Decal);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BattleCry_CirecleLines"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINE], m_pScaleOffset[TYPE_ET2_E_CIRCLELINE], m_pRotationOffset[TYPE_ET2_E_CIRCLELINE], nullptr, &m_pEt2_Line, false);
			if (nullptr != m_pEt2_Line)
			{
				Safe_AddRef(m_pEt2_Line);
				m_pEt2_Line->Set_Color(m_fMainColor);
				m_pEt2_Line->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_E_DOME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_DOME])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BattleCry_Dome"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_DOME], m_pScaleOffset[TYPE_ET3_E_DOME], m_pRotationOffset[TYPE_ET3_E_DOME], nullptr, &m_pEt3_Dome, false);
			if (nullptr != m_pEt3_Dome)
			{
				Safe_AddRef(m_pEt3_Dome);
				m_pEt3_Dome->Set_Color(m_fMainColor);
				m_pEt3_Dome->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_D_FIRECRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_D_FIRECRACK])
		{
			if (nullptr != m_pEt1_Decal)
			{
				m_pEt1_Decal->Start_AlphaDeleate();
				Safe_Release(m_pEt1_Decal);
			}

			CDecal* pDecal = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_BattleCry_Crack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_D_FIRECRACK], m_pScaleOffset[TYPE_ET3_D_FIRECRACK], m_pRotationOffset[TYPE_ET3_D_FIRECRACK], nullptr, &pDecal);
			if (nullptr != pDecal)
			{
				_float3 fDecalColor = _float3(min(m_fMainColor.x + 0.2f, 1.f), min(m_fMainColor.y + 0.2f, 1.f), min(m_fMainColor.z + 0.2f, 1.f));
				pDecal->Set_Color(fDecalColor, fDecalColor);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_SPRINGUP])
		{
			if (nullptr != m_pEt2_Line)
			{
				m_pEt2_Line->Start_Dissolve(85,    // Index
					_float4(min(m_fMainColor.x + 0.3f, 1.f), min(m_fMainColor.y + 0.3f, 1.f), min(m_fMainColor.z + 0.3f, 1.f), 1.f), // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pEt2_Line);
			}

			if (nullptr != m_pEt3_Dome)
			{
				m_pEt3_Dome->Start_Dissolve(85,    // Index
					_float4(min(m_fMainColor.x + 0.3f, 1.f), min(m_fMainColor.y + 0.3f, 1.f), min(m_fMainColor.z + 0.3f, 1.f), 1.f), // Color // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pEt3_Dome);
			}

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BattleCry_SpringUp"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_SPRINGUP], m_pScaleOffset[TYPE_ET3_E_SPRINGUP], m_pRotationOffset[TYPE_ET3_E_SPRINGUP], nullptr, &m_pEt3_Spring, false);
			if (nullptr != m_pEt3_Spring)
			{
				Safe_AddRef(m_pEt3_Spring);
				m_pEt3_Spring->Set_Color(m_fMainColor);
				m_pEt3_Spring->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_CIRCLELINE])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BattleCry_CirecleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_CIRCLELINE], m_pScaleOffset[TYPE_ET3_E_CIRCLELINE], m_pRotationOffset[TYPE_ET3_E_CIRCLELINE], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_STONE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BattleCry_Stone"), 
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_STONE], m_pScaleOffset[TYPE_ET3_P_STONE], m_pRotationOffset[TYPE_ET3_P_STONE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_BattleCry_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES], m_pScaleOffset[TYPE_ET3_P_CIRCLES], m_pRotationOffset[TYPE_ET3_P_CIRCLES], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_BattleCry::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_BattleCry* CVfx_Destroyer_Skill_BattleCry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_BattleCry* pInstance = new CVfx_Destroyer_Skill_BattleCry(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_BattleCry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_BattleCry::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_BattleCry* pInstance = new CVfx_Destroyer_Skill_BattleCry(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_BattleCry");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_BattleCry::Free()
{
	__super::Free();

	if (nullptr != m_pEt1_Decal)
	{
		m_pEt1_Decal->Start_AlphaDeleate();
		Safe_Release(m_pEt1_Decal);
	}

	if (nullptr != m_pEt2_Line)
	{
		m_pEt2_Line->Start_Dissolve(85,    // Index
			_float4(1.f, 0.6f, 0.4f, 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pEt2_Line);
	}

	if (nullptr != m_pEt3_Dome)
	{
		m_pEt3_Dome->Start_Dissolve(85,    // Index
			_float4(min(m_fMainColor.x + 0.3f, 1.f), min(m_fMainColor.y + 0.3f, 1.f), min(m_fMainColor.z + 0.3f, 1.f), 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pEt3_Dome);
	}

	if (nullptr != m_pEt3_Spring)
	{
		m_pEt3_Spring->Start_Dissolve(85,  // Index
			_float4(min(m_fMainColor.x + 0.3f, 1.f), min(m_fMainColor.y + 0.3f, 1.f), min(m_fMainColor.z + 0.3f, 1.f), 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pEt3_Spring);
	}
	
	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}