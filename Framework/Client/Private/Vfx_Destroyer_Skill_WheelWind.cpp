#include "stdafx.h"
#include "Vfx_Destroyer_Skill_WheelWind.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Utils.h"
#include "Decal.h"
#include "Effect.h"
#include "Model.h"

CVfx_Destroyer_Skill_WheelWind::CVfx_Destroyer_Skill_WheelWind(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_WheelWind::CVfx_Destroyer_Skill_WheelWind(const CVfx_Destroyer_Skill_WheelWind& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_WheelWind::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE]    = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	// 돌기 시작
	{
		m_pFrameTriger[TYPE_ET2_E_TORNADO]    = 13;
		m_pPositionOffset[TYPE_ET2_E_TORNADO] = _float3(0.f, 0.1f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_TORNADO]    = _float3(8.f, 5.f, 8.f);
		m_pRotationOffset[TYPE_ET2_E_TORNADO] = _float3(0.f, 0.f, 0.f);
	}


	m_pFrameTriger[TYPE_ET3_P_FIRE]    = 0;
	m_pPositionOffset[TYPE_ET3_P_FIRE] = _float3(-2.5f, 1.f, -2.5f); // Min
	m_pScaleOffset[TYPE_ET3_P_FIRE]    = _float3(2.5f, 2.f, 2.5f);   // Max
	m_pRotationOffset[TYPE_ET3_P_FIRE] = _float3(0.1f, 0.2f, 0.f); // MinMax

	m_pFrameTriger[TYPE_ET3_P_CIRCLES]    = 15; // EventFrame
	m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(2.f, 3.f, 0.f); // Speed
	m_pScaleOffset[TYPE_ET3_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_WheelWind::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_WheelWind::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE], nullptr, &m_pEt1_Decal, false);
			m_pEt1_Decal->Set_LifeTime(6.f);
			Safe_AddRef(m_pEt1_Decal);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_TORNADO && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_TORNADO])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_WheelWind_Trail"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TORNADO], m_pScaleOffset[TYPE_ET2_E_TORNADO], m_pRotationOffset[TYPE_ET2_E_TORNADO], nullptr, &m_pEt2_Trail, false);
			Safe_AddRef(m_pEt2_Trail);
			m_iCount++;

			fNextTime = CUtils::Random_Float(0.5f, 1.f);
		}
		
		else if (m_iCount == TYPE_ET3_P_FIRE)
		{
			if (nullptr != m_pEt2_Trail)
			{
				CModel* pModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
				if (nullptr != pModel)
				{
					if (pModel->Get_CurrAnimation() == pModel->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_SkillWhirlwindfinish"))
					{
						if (m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
						{
							m_pEt2_Trail->Set_UVFlow(1, _float2(1, 0), _float2(m_pPositionOffset[TYPE_ET3_P_CIRCLES].x, m_pPositionOffset[TYPE_ET3_P_CIRCLES].y));
							Safe_Release(m_pEt2_Trail);
						}
					}
					else
					{
						m_fTimeAcc += fTimeDelta;
						if (fNextTime <= m_fTimeAcc)
						{
							m_fTimeAcc = 0.f;

							fNextTime = CUtils::Random_Float(m_pRotationOffset[TYPE_ET3_P_FIRE].x, m_pRotationOffset[TYPE_ET3_P_FIRE].y);

							_float3 fRandomPosition = _float3(CUtils::Random_Float(m_pPositionOffset[TYPE_ET3_P_FIRE].x, m_pScaleOffset[TYPE_ET3_P_FIRE].x),
								CUtils::Random_Float(m_pPositionOffset[TYPE_ET3_P_FIRE].y, m_pScaleOffset[TYPE_ET3_P_FIRE].y),
								CUtils::Random_Float(m_pPositionOffset[TYPE_ET3_P_FIRE].z, m_pScaleOffset[TYPE_ET3_P_FIRE].z));

							GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_WheelWind_Fire"), XMLoadFloat4x4(&m_WorldMatrix), fRandomPosition, _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
							GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_WheelWind_Circle"), XMLoadFloat4x4(&m_WorldMatrix), fRandomPosition, _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
						}
					}
				}
			}
		}
	}
}

void CVfx_Destroyer_Skill_WheelWind::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_WheelWind::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_WheelWind::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_WheelWind* CVfx_Destroyer_Skill_WheelWind::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_WheelWind* pInstance = new CVfx_Destroyer_Skill_WheelWind(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_WheelWind");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_WheelWind::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_WheelWind* pInstance = new CVfx_Destroyer_Skill_WheelWind(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_WheelWind");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_WheelWind::Free()
{
	__super::Free();

	if (nullptr != m_pEt1_Decal)
	{
		m_pEt1_Decal->Start_AlphaDeleate();
		Safe_Release(m_pEt1_Decal);
	}

	if (nullptr != m_pEt2_Trail)
	{
		m_pEt2_Trail->Set_UVFlow(1, _float2(1, 0), _float2(2.f, 3.f));
		Safe_Release(m_pEt2_Trail);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}