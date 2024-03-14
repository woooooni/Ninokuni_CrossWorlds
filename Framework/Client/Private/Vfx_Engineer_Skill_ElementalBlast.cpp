#include "stdafx.h"
#include "Vfx_Engineer_Skill_ElementalBlast.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Particle.h"
#include "Decal.h"
#include "Effect.h"
#include "Utils.h"

CVfx_Engineer_Skill_ElementalBlast::CVfx_Engineer_Skill_ElementalBlast(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_ElementalBlast::CVfx_Engineer_Skill_ElementalBlast(const CVfx_Engineer_Skill_ElementalBlast& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_ElementalBlast::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_1;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_RECT] = 0;
		m_pPositionOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.3f);
		m_pScaleOffset[TYPE_ET1_D_RECT]    = _float3(2.5f, 5.f, 4.5f);
		m_pRotationOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 12;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(3.f, 18.f, 16.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(0.05f, 0.05f, 0.05f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 12;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.3f, 1.7f, 1.6f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_P_FIRE_MAIN] = 21;
		m_pPositionOffset[TYPE_ET3_P_FIRE_MAIN] = _float3(0.f, 1.f, 1.5f);
		m_pScaleOffset[TYPE_ET3_P_FIRE_MAIN]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_FIRE_MAIN] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 21;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 1.f, 1.5f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_P_SMOKE] = 21;
		m_pPositionOffset[TYPE_ET3_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_EVENT_FIRE] = 75;
	m_pPositionOffset[TYPE_EVENT_FIRE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_EVENT_FIRE]    = _float3(0.f, 0.f, 0.f);
	m_pRotationOffset[TYPE_EVENT_FIRE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ElementalBlast::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_ElementalBlast::Tick(_float fTimeDelta)
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
				m_fMainColor = _float3(1.000f, 0.670f, 0.446f);
				break;
			case ELEMENTAL_TYPE::WATER:
				m_fMainColor = _float3(0.544f, 0.964f, 0.879f);
				break;
			case ELEMENTAL_TYPE::WOOD:
				m_fMainColor = _float3(0.741f, 0.938f, 0.469f);
				break;
			}
		}

		if (m_iCount == TYPE_ET1_D_RECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_RECT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_RECT], m_pScaleOffset[TYPE_ET1_D_RECT], m_pRotationOffset[TYPE_ET1_D_RECT]);
			m_iCount++;
		}


		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			CEffect* pEffect = nullptr;
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_BurstCall_CircleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINE], m_pScaleOffset[TYPE_ET2_E_CIRCLELINE], m_pRotationOffset[TYPE_ET2_E_CIRCLELINE], nullptr, &pEffect);
			if (nullptr != pEffect)
			{
				pEffect->Set_Color(m_fMainColor);
				pEffect->Set_DistortionPower(CUtils::Random_Float(0.f, 0.25f), CUtils::Random_Float(0.f, 0.25f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			CParticle* pParticle = nullptr;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_BurstCall_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES], nullptr, &pParticle);
			if (nullptr != pParticle)
			{
				pParticle->Set_Color(m_fMainColor);
			}
			m_iCount++;
		}


		else if (m_iCount == TYPE_ET3_P_FIRE_MAIN && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_FIRE_MAIN])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_ElementalBlast_Fire_Main"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_FIRE_MAIN], m_pScaleOffset[TYPE_ET3_P_FIRE_MAIN], m_pRotationOffset[TYPE_ET3_P_FIRE_MAIN], nullptr, &m_pEt3_FireBig, false);
			Safe_AddRef(m_pEt3_FireBig);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_ElementalBlast_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES], m_pScaleOffset[TYPE_ET3_P_CIRCLES], m_pRotationOffset[TYPE_ET3_P_CIRCLES], nullptr, &m_pEt3_Circles, false);
			Safe_AddRef(m_pEt3_Circles);
			m_iCount++;
		}


		else if (m_iCount == TYPE_ET3_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_SMOKE])
		{
			//GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT(""),
			//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_SMOKE], m_pScaleOffset[TYPE_ET3_P_SMOKE], m_pRotationOffset[TYPE_ET3_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_EVENT_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_EVENT_FIRE])
		{
			if (nullptr != m_pEt3_FireBig)
			{
				m_pEt3_FireBig->Set_LoopParticle(false);
				Safe_Release(m_pEt3_FireBig);
			}

			if (nullptr != m_pEt3_Circles)
			{
				m_pEt3_Circles->Set_LoopParticle(false);
				Safe_Release(m_pEt3_Circles);
			}

			m_iCount++;
		}
	}
}

void CVfx_Engineer_Skill_ElementalBlast::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_ElementalBlast::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ElementalBlast::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_ElementalBlast* CVfx_Engineer_Skill_ElementalBlast::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_ElementalBlast* pInstance = new CVfx_Engineer_Skill_ElementalBlast(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_ElementalBlast");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_ElementalBlast::Clone(void* pArg)
{
	CVfx_Engineer_Skill_ElementalBlast* pInstance = new CVfx_Engineer_Skill_ElementalBlast(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_ElementalBlast");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_ElementalBlast::Free()
{
	__super::Free();

	if (nullptr != m_pEt3_FireBig)
	{
		m_pEt3_FireBig->Set_LoopParticle(false);
		Safe_Release(m_pEt3_FireBig);
	}

	if (nullptr != m_pEt3_Circles)
	{
		m_pEt3_Circles->Set_LoopParticle(false);
		Safe_Release(m_pEt3_Circles);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}