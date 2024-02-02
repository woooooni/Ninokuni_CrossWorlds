#include "stdafx.h"
#include "Vfx_Engineer_Skill_FlashHeal.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Particle.h"
#include "Decal.h"
#include "Effect.h"
#include "Utils.h"

CVfx_Engineer_Skill_FlashHeal::CVfx_Engineer_Skill_FlashHeal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_FlashHeal::CVfx_Engineer_Skill_FlashHeal(const CVfx_Engineer_Skill_FlashHeal& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE] = _float3(10.f, 3.f, 10.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 10;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 7.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.05f, 0.05f, 0.05f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_DOME] = 16;
		m_pPositionOffset[TYPE_ET2_E_DOME] = _float3(0.f, 1.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_DOME]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET2_E_DOME] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_DOMELINE] = 16;
		m_pPositionOffset[TYPE_ET2_E_DOMELINE] = _float3(0.f, 1.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_DOMELINE]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET2_E_DOMELINE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_E_CROSS] = 26;
		m_pPositionOffset[TYPE_ET2_E_CROSS] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CROSS]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CROSS] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_TWINKLE] = 26;
		m_pPositionOffset[TYPE_ET2_P_TWINKLE] = _float3(0.f, 2.5f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_TWINKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_TWINKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 40;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_ET3_EVENT_DELETE] = 45;

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_FlashHeal::Tick(_float fTimeDelta)
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
				m_fMainColor = _float3(1.000f, 0.944f, 0.435f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.394f, 0.896f, 1.000f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.549f, 1.000f, 0.629f);
				break;
			}
		}

		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_FlashHeal_CircleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_CIRCLELINE], m_pScaleOffset[TYPE_ET1_E_CIRCLELINE], m_pRotationOffset[TYPE_ET1_E_CIRCLELINE], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.2f), CUtils::Random_Float(0.f, 0.2f));
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_DOME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_DOME])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_FlashHeal_Dome"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_DOME], m_pScaleOffset[TYPE_ET2_E_DOME], m_pRotationOffset[TYPE_ET2_E_DOME], nullptr, &m_pDome, false);
			if (nullptr != m_pDome)
			{
				Safe_AddRef(m_pDome);
				m_pDome->Set_Color(m_fMainColor);
				m_pDome->Set_DistortionPower(CUtils::Random_Float(0.f, 0.2f), CUtils::Random_Float(0.f, 0.2f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_DOMELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_DOMELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_FlahHeal_DomeLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_DOMELINE], m_pScaleOffset[TYPE_ET2_E_DOMELINE], m_pRotationOffset[TYPE_ET2_E_DOMELINE], nullptr, &m_pDomeLine, false);
			if (nullptr != m_pDomeLine)
			{
				Safe_AddRef(m_pDomeLine);
				m_pDome->Set_DistortionPower(CUtils::Random_Float(0.f, 0.2f), CUtils::Random_Float(0.f, 0.2f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_CROSS && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CROSS])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_FlashHeal_Cross"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CROSS], m_pScaleOffset[TYPE_ET2_E_CROSS], m_pRotationOffset[TYPE_ET2_E_CROSS], nullptr, &m_pCross, false);
			if (nullptr != m_pCross)
			{
				Safe_AddRef(m_pCross);
				m_pCross->Set_Color(m_fMainColor);
				m_pCross->Set_DistortionPower(0.f, 0.f);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_TWINKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_TWINKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_FlashHeal_Twinkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_TWINKLE], m_pScaleOffset[TYPE_ET2_P_TWINKLE], m_pRotationOffset[TYPE_ET2_P_TWINKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_FlashHeal_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], _float3(m_pRotationOffset[TYPE_ET2_P_CIRCLES].x, m_pRotationOffset[TYPE_ET2_P_CIRCLES].y, m_pRotationOffset[TYPE_ET2_P_CIRCLES].z), nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_EVENT_DELETE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_EVENT_DELETE])
		{
			_float3 fEventColor = _float3(max(m_fMainColor.x - 0.1f, 0.f), max(m_fMainColor.y - 0.1f, 0.f), max(m_fMainColor.z - 0.1f, 0.f));

			if (nullptr != m_pDome)
			{
				m_pDome->Start_Dissolve(90,        // Index
					_float4(fEventColor.x, fEventColor.y, fEventColor.z, 1.f),//_float4(1.f, 0.8f, 0.4f, 1.f), // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pDome);
			}

			if (nullptr != m_pDomeLine)
			{
				m_pDomeLine->Start_Dissolve(90,    // Index
					_float4(fEventColor.x, fEventColor.y, fEventColor.z, 1.f),//_float4(1.f, 0.8f, 0.4f, 1.f), // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pDomeLine);
			}

			if (nullptr != m_pCross)
			{
				m_pCross->Start_Dissolve(73,       // Index
					_float4(fEventColor.x, fEventColor.y, fEventColor.z, 1.f),//_float4(1.f, 0.7f, 0.2f, 1.f), // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pCross);
			}

			m_iCount++;
		}
	}
}

void CVfx_Engineer_Skill_FlashHeal::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_FlashHeal::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_FlashHeal* CVfx_Engineer_Skill_FlashHeal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_FlashHeal* pInstance = new CVfx_Engineer_Skill_FlashHeal(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_FlashHeal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_FlashHeal::Clone(void* pArg)
{
	CVfx_Engineer_Skill_FlashHeal* pInstance = new CVfx_Engineer_Skill_FlashHeal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_FlashHeal");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_FlashHeal::Free()
{
	__super::Free();

	if (nullptr != m_pDome)
	{
		m_pDome->Start_Dissolve(90,        // Index
			_float4(1.f, 0.8f, 0.4f, 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pDome);
	}

	if (nullptr != m_pDomeLine)
	{
		m_pDomeLine->Start_Dissolve(90,    // Index
			_float4(1.f, 0.8f, 0.4f, 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pDomeLine);
	}

	if (nullptr != m_pCross)
	{
		m_pCross->Start_Dissolve(73,       // Index
			_float4(1.f, 0.7f, 0.2f, 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pCross);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}