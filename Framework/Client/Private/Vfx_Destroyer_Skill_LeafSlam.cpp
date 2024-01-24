#include "stdafx.h"
#include "Vfx_Destroyer_Skill_LeafSlam.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Decal.h"
#include "Effect.h"

CVfx_Destroyer_Skill_LeafSlam::CVfx_Destroyer_Skill_LeafSlam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_LeafSlam::CVfx_Destroyer_Skill_LeafSlam(const CVfx_Destroyer_Skill_LeafSlam& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.5f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(4.f, 4.f, 4.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_CRACK] = 0;
		m_pPositionOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.35f);
		m_pScaleOffset[TYPE_ET2_D_CRACK]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_SPRINGUP] = 1;
		m_pPositionOffset[TYPE_ET2_E_SPRINGUP] = _float3(0.f, 0.f, 1.7f);
		m_pScaleOffset[TYPE_ET2_E_SPRINGUP]    = _float3(1.f, 2.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_SPRINGUP] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_STONES] = 5;
		m_pPositionOffset[TYPE_ET2_P_STONES] = _float3(0.f, 0.f, 1.5f);
		m_pScaleOffset[TYPE_ET2_P_STONES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_STONES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 6;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 1.5f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 7;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 2.f, 17.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(0.1f, 0.1f, 0.1f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_LeafSlam::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE], m_pOwnerObject, &m_pEt1_Decal, false);
			Safe_AddRef(m_pEt1_Decal);
			m_iCount++;
		}

		CModel* pModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
		if (nullptr != pModel)
		{
			if (pModel->Get_CurrAnimation() == pModel->Get_Animation("SKM_Destroyer_Merge.ao|Destroyer_SkillMegaCrushFinish"))
			{
				if (m_iCount == TYPE_ET2_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CRACK])
				{
					if (nullptr != m_pEt1_Decal)
					{
						m_pEt1_Decal->Set_Dead(true);
						Safe_Release(m_pEt1_Decal);
					}

					CTransform* pTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
					if (nullptr != pTransform)
						m_WorldMatrix = pTransform->Get_WorldFloat4x4();
					GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Destroyer_Skill_LeafSlam_Crack"),
						XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_CRACK], m_pScaleOffset[TYPE_ET2_D_CRACK], m_pRotationOffset[TYPE_ET2_D_CRACK]);
					m_iCount++;
				}
				else if (m_iCount == TYPE_ET2_E_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_SPRINGUP])
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_BattleCry_SpringUp"),
						XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_SPRINGUP], m_pScaleOffset[TYPE_ET2_E_SPRINGUP], m_pRotationOffset[TYPE_ET2_E_SPRINGUP], nullptr, &m_pEt2_SpringUp, false);
					Safe_AddRef(m_pEt2_SpringUp);
					m_iCount++;
				}

				else if (m_iCount == TYPE_ET2_P_STONES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_STONES])
				{
					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_LeafSlam_Stone"),
						XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_STONES], m_pScaleOffset[TYPE_ET2_P_STONES], m_pRotationOffset[TYPE_ET2_P_STONES]);
					m_iCount++;
				}
				else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
				{
					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_Skill_LeafSlam_Circles"),
						XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES]);
					m_iCount++;
				}
				
				else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
				{
					CEffect* pCircleLine = nullptr;
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Destroyer_Skill_LeafSlam_CirecleLine"),
						XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINE], m_pScaleOffset[TYPE_ET2_E_CIRCLELINE], m_pRotationOffset[TYPE_ET2_E_CIRCLELINE], nullptr, &pCircleLine);
					if (nullptr != pCircleLine)
					{
						pCircleLine->Reserve_Dissolve(3, // Index
							_float4(1.f, 0.4f, 0.f, 1.f),  // Color
							20.f,  // Speed
							10.f); // Total
					}
					m_iCount++;
				}

				else if (m_iCount == TYPE_END)
					m_bFinish = true;
			}
		}
	}
}

void CVfx_Destroyer_Skill_LeafSlam::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_LeafSlam* CVfx_Destroyer_Skill_LeafSlam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_LeafSlam* pInstance = new CVfx_Destroyer_Skill_LeafSlam(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_LeafSlam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_LeafSlam::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_LeafSlam* pInstance = new CVfx_Destroyer_Skill_LeafSlam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_LeafSlam");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_LeafSlam::Free()
{
	__super::Free();

	if (nullptr != m_pEt1_Decal)
	{
		m_pEt1_Decal->Set_Dead(true);
		Safe_Release(m_pEt1_Decal);
	}

	if (nullptr != m_pEt2_SpringUp)
	{
		m_pEt2_SpringUp->Start_Dissolve(85,  // Index
			_float4(1.f, 0.6f, 0.4f, 1.f), // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pEt2_SpringUp);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}