#include "stdafx.h"
#include "Vfx_Engineer_Skill_HealingTree.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Effect.h"
#include "Particle.h"

CVfx_Engineer_Skill_HealingTree::CVfx_Engineer_Skill_HealingTree(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_HealingTree::CVfx_Engineer_Skill_HealingTree(const CVfx_Engineer_Skill_HealingTree& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_HealingTree::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];


	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(10.f, 3.f, 10.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_LIGHT] = 10;
		m_pPositionOffset[TYPE_ET2_P_LIGHT] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_LIGHT]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_LIGHT] = _float3(0.f, 0.f, 0.f);
	}

	// -----------------------------------------------------------------
	_uint iStartFrame = 22;
	{
		m_pFrameTriger[TYPE_ET3_E_AURA_01] = iStartFrame;
		m_pPositionOffset[TYPE_ET3_E_AURA_01] = _float3(0.f, 0.f, -0.7f);
		m_pScaleOffset[TYPE_ET3_E_AURA_01]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET3_E_AURA_01] = _float3(90.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CROSSSGLITTER_01] = iStartFrame;
		m_pPositionOffset[TYPE_ET3_P_CROSSSGLITTER_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CROSSSGLITTER_01]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CROSSSGLITTER_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES_01] = iStartFrame;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES_01]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES_01] = _float3(0.f, 0.f, 0.f);
	}

	return S_OK;
}

HRESULT CVfx_Engineer_Skill_HealingTree::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_HealingTree::Tick(_float fTimeDelta)
{
	if (m_bFinish || m_pOwnerObject == nullptr || m_pOwnerObject->Is_ReserveDead() || m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	CStateMachine* pMachine = m_pOwnerObject->Get_Component<CStateMachine>(L"Com_StateMachine");
	CModel* pModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");

	if (pMachine == nullptr || pModel == nullptr)
	{
		Set_Dead(true);
		return;
	}

	if (m_bOwnerTween) // false == TweenFinish
		m_bOwnerTween = pModel->Is_Tween();
	else
		m_iOwnerFrame = pModel->Get_CurrAnimationFrame();

	if (!m_bOwnerTween)
	{
		if (m_iCount >= TYPE_END)
			m_bFinish = true;

		else if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_LIGHT])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_HealingTree_ZeroCircles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_LIGHT], m_pScaleOffset[TYPE_ET2_P_LIGHT], m_pRotationOffset[TYPE_ET2_P_LIGHT]);
			m_iCount++;
		}

		// 1
		else if (m_iCount == TYPE_ET3_E_AURA_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_AURA_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_HealingTree_Aura"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_AURA_01], m_pScaleOffset[TYPE_ET3_E_AURA_01], m_pRotationOffset[TYPE_ET3_E_AURA_01], m_pOwnerObject);
			m_iCount++;

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_SkilEngineEngineer_Skill_HealingTree_Cross"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CROSSSGLITTER_01], m_pScaleOffset[TYPE_ET3_P_CROSSSGLITTER_01], m_pRotationOffset[TYPE_ET3_P_CROSSSGLITTER_01], m_pOwnerObject);
			m_iCount++;

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_SkilEngineEngineer_Skill_HealingTree_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES_01], m_pScaleOffset[TYPE_ET3_P_CIRCLES_01], m_pRotationOffset[TYPE_ET3_P_CIRCLES_01], m_pOwnerObject);
			m_iCount++;
		}

		// 2 ~ 6
		else if (m_iCount > TYPE_ET3_P_CIRCLES_01)
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc >= 1.f)
			{
				m_fTimeAcc = 0.f;

				CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr != pOwnerTransform)
					m_WorldMatrix = pOwnerTransform->Get_WorldFloat4x4();

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_HealingTree_Aura"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_E_AURA_01], m_pScaleOffset[TYPE_ET3_E_AURA_01], m_pRotationOffset[TYPE_ET3_E_AURA_01], m_pOwnerObject);

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_SkilEngineEngineer_Skill_HealingTree_Cross"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CROSSSGLITTER_01], m_pScaleOffset[TYPE_ET3_P_CROSSSGLITTER_01], m_pRotationOffset[TYPE_ET3_P_CROSSSGLITTER_01], m_pOwnerObject);

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Destroyer_SkilEngineEngineer_Skill_HealingTree_Circles"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_CIRCLES_01], m_pScaleOffset[TYPE_ET3_P_CIRCLES_01], m_pRotationOffset[TYPE_ET3_P_CIRCLES_01], m_pOwnerObject);

				m_iCount++;
			}
		}
	}
}

void CVfx_Engineer_Skill_HealingTree::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_HealingTree::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_HealingTree::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_HealingTree* CVfx_Engineer_Skill_HealingTree::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_HealingTree* pInstance = new CVfx_Engineer_Skill_HealingTree(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_HealingTree");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_HealingTree::Clone(void* pArg)
{
	CVfx_Engineer_Skill_HealingTree* pInstance = new CVfx_Engineer_Skill_HealingTree(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_HealingTree");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_HealingTree::Free()
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