#include "stdafx.h"
#include "Vfx_Destroyer_Skill_IgnorePain.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Effect.h"
#include "Particle.h"
#include "Utils.h"

CVfx_Destroyer_Skill_IgnorePain::CVfx_Destroyer_Skill_IgnorePain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_IgnorePain::CVfx_Destroyer_Skill_IgnorePain(const CVfx_Destroyer_Skill_IgnorePain& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_E_BARRIER] = 20;
		m_pPositionOffset[TYPE_ET1_E_BARRIER] = _float3(0.f, 0.45f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_BARRIER]    = _float3(2.f, 2.f, 2.f);
		m_pRotationOffset[TYPE_ET1_E_BARRIER] = _float3(0.f, 0.f, 0.f);
	}

	{ // 10초
		m_pFrameTriger[TYPE_ET2_P_SPARCKE] = 21;
		m_pPositionOffset[TYPE_ET2_P_SPARCKE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SPARCKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SPARCKE] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_EV_DISSOLVE] = 70;//Index //31 40 49 64
	m_pPositionOffset[TYPE_EV_DISSOLVE] = _float3(0.f, 0.3f, 0.f);//Color
	m_pScaleOffset[TYPE_EV_DISSOLVE]    = _float3(5.f, 0.f, 0.f);//Speed
	m_pRotationOffset[TYPE_EV_DISSOLVE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_IgnorePain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween) // 노이즈 125
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

		if (m_iCount == TYPE_ET1_E_BARRIER && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_BARRIER])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_IgnorePain_Barrier"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_BARRIER], m_pScaleOffset[TYPE_ET1_E_BARRIER], m_pRotationOffset[TYPE_ET1_E_BARRIER], m_pOwnerObject, &m_pEt1_Barrier, false);
			if (nullptr != m_pEt1_Barrier)
			{
				Safe_AddRef(m_pEt1_Barrier);
				m_pEt1_Barrier->Set_Color(m_fMainColor);
				//m_pEt1_Barrier->Set_DistortionPower(CUtils::Random_Float(0.f, 0.5f), CUtils::Random_Float(0.f, 0.5f));
			}

			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_SPARCKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPARCKE])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_IgnorePain_Cirecles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_SPARCKE], m_pScaleOffset[TYPE_ET2_P_SPARCKE], m_pRotationOffset[TYPE_ET2_P_SPARCKE], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}

		//else if (m_iCount == TYPE_END)
		//	m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_IgnorePain::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_IgnorePain::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_IgnorePain* CVfx_Destroyer_Skill_IgnorePain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_IgnorePain* pInstance = new CVfx_Destroyer_Skill_IgnorePain(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_IgnorePain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_IgnorePain::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_IgnorePain* pInstance = new CVfx_Destroyer_Skill_IgnorePain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_IgnorePain");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_IgnorePain::Free()
{
	__super::Free();

	if (nullptr != m_pEt1_Barrier)
	{
		m_pEt1_Barrier->Reserve_Dissolve(m_pFrameTriger[TYPE_EV_DISSOLVE], // Index
			_float4(max(m_fMainColor.x - 0.35f, 0.f), max(m_fMainColor.y - 0.35f, 0.f), max(m_fMainColor.z - 0.35f, 0.f), 1.f), // Color
			m_pScaleOffset[TYPE_EV_DISSOLVE].x,   // Speed
			10.f); // Total
		Safe_Release(m_pEt1_Barrier);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}