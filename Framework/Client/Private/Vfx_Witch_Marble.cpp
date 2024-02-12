#include "stdafx.h"
#include "Vfx_Witch_Marble.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

CVfx_Witch_Marble::CVfx_Witch_Marble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Marble::CVfx_Witch_Marble(const CVfx_Witch_Marble& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Marble::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pPositionOffset[TYPE_ET1_E_MARBLE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_ET1_E_MARBLE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ET1_E_MARBLE] = _float3(0.f, 0.f, 0.f);

	m_pPositionOffset[TYPE_ET1_E_MARBLE_OUTLINE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_ET1_E_MARBLE_OUTLINE]    = _float3(1.3f, 1.3f, 1.3f);
	m_pRotationOffset[TYPE_ET1_E_MARBLE_OUTLINE] = _float3(0.f, 0.f, 0.f);


	m_pPositionOffset[TYPE_ET1_P_BACK] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_ET1_P_BACK]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ET1_P_BACK] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Witch_Marble::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Marble::Tick(_float fTimeDelta)
{
	if (nullptr == m_pOwnerObject || m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	if (false == m_bCreate)
	{
		/*GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall"),
			XMLoadFloat4x4(&m_WorldMatrix), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), m_pOwnerObject, &m_pMarble);
		Safe_AddRef(m_pMarble);

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall_Flame"),
			XMLoadFloat4x4(&m_WorldMatrix), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), m_pOwnerObject);

		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_ClownWizard_DarkBall_Lighting"),
			XMLoadFloat4x4(&m_WorldMatrix), Vec3(0.f, 0.f, 0.f), Vec3(1.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), m_pOwnerObject);
		*/
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Marble"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_MARBLE], m_pScaleOffset[TYPE_ET1_E_MARBLE], m_pRotationOffset[TYPE_ET1_E_MARBLE], m_pOwnerObject, &m_pMarble);
		if (nullptr != m_pMarble)
		{
			Safe_AddRef(m_pMarble);
			m_pMarble->Set_Color(_float3(0.877, 0.4f, 1.f));
		}

		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Marble"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_MARBLE], m_pScaleOffset[TYPE_ET1_E_MARBLE], m_pRotationOffset[TYPE_ET1_E_MARBLE], m_pOwnerObject);

		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Marble_OutLine"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_MARBLE_OUTLINE], m_pScaleOffset[TYPE_ET1_E_MARBLE_OUTLINE], m_pRotationOffset[TYPE_ET1_E_MARBLE_OUTLINE], m_pOwnerObject);

		m_bCreate = true;
	}

	// 지나온 길에 흔적 파티클 생성
	if (nullptr != m_pMarble)
	{
		if (m_pMarble->Is_Dead())
		{
			Safe_Release(m_pMarble);
			return;
		}

		m_fTimeAcc += fTimeDelta;
		if (m_fTimeAcc >= 0.35f)
		{
			CTransform* pMarbleTransform = m_pMarble->Get_Component<CTransform>(L"Com_Transform");
			if (pMarbleTransform != nullptr)
			{
				m_fTimeAcc = 0.f;

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_Marble_Back"),
					pMarbleTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET1_P_BACK], m_pScaleOffset[TYPE_ET1_P_BACK], m_pRotationOffset[TYPE_ET1_P_BACK]);
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_Marble_Back_Circle"),
					pMarbleTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET1_P_BACK], m_pScaleOffset[TYPE_ET1_P_BACK], m_pRotationOffset[TYPE_ET1_P_BACK]);
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Witch_Marble_Back_Circle_02"),
					pMarbleTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET1_P_BACK], m_pScaleOffset[TYPE_ET1_P_BACK], m_pRotationOffset[TYPE_ET1_P_BACK]);
			}
		}
	}
}

void CVfx_Witch_Marble::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Marble::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Marble::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Marble* CVfx_Witch_Marble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Marble* pInstance = new CVfx_Witch_Marble(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Marble");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Marble::Clone(void* pArg)
{
	CVfx_Witch_Marble* pInstance = new CVfx_Witch_Marble(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage01Shield");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Marble::Free()
{
	__super::Free();

	if (nullptr != m_pMarble)
		Safe_Release(m_pMarble);

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}