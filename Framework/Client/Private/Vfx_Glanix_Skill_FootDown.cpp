#include "stdafx.h"
#include "Vfx_Glanix_Skill_FootDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_FootDown::CVfx_Glanix_Skill_FootDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_FootDown::CVfx_Glanix_Skill_FootDown(const CVfx_Glanix_Skill_FootDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_FootDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_ATTACK2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 1
	m_pFrameTriger[TYPE_D_CRACK]    = 61;
	m_pPositionOffset[TYPE_D_CRACK] = _float3(-0.15f, 0.f, 0.4f);
	m_pScaleOffset[TYPE_D_CRACK]    = _float3(8.f, 5.f, 8.f);
	m_pRotationOffset[TYPE_D_CRACK] = _float3(0.f, 0.f, 0.f);

	// 2
	m_pFrameTriger[TYPE_P_SMOKE]    = 61;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(-0.8f, 0.5f, 2.f);
	m_pScaleOffset[TYPE_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_P_CIRCLE]    = 61;
	m_pPositionOffset[TYPE_P_CIRCLE] = _float3(-0.8f, 0.5f, 2.f);
	m_pScaleOffset[TYPE_P_CIRCLE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLE] = _float3(0.f, 0.f, 0.f);

	// 4
	m_pFrameTriger[TYPE_E_CIRCLE_LINE]    = 63;
	m_pPositionOffset[TYPE_E_CIRCLE_LINE] = _float3(-0.8f, 0.2f, 2.f);
	m_pScaleOffset[TYPE_E_CIRCLE_LINE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_CIRCLE_LINE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_FootDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_FootDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_CRACK])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FootDown_Crack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_CRACK], m_pScaleOffset[TYPE_D_CRACK], m_pRotationOffset[TYPE_D_CRACK]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FootDown_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLE], m_pScaleOffset[TYPE_P_CIRCLE], m_pRotationOffset[TYPE_P_CIRCLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_CIRCLE_LINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_CIRCLE_LINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FootDown_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_CIRCLE_LINE], m_pScaleOffset[TYPE_E_CIRCLE_LINE], m_pRotationOffset[TYPE_E_CIRCLE_LINE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_FootDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_FootDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_FootDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_FootDown* CVfx_Glanix_Skill_FootDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_FootDown* pInstance = new CVfx_Glanix_Skill_FootDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_FootDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_FootDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_FootDown* pInstance = new CVfx_Glanix_Skill_FootDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_FootDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_FootDown::Free()
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