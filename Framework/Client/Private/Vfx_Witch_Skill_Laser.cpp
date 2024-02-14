#include "stdafx.h"
#include "Vfx_Witch_Skill_Laser.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Decal.h"

CVfx_Witch_Skill_Laser::CVfx_Witch_Skill_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_Laser::CVfx_Witch_Skill_Laser(const CVfx_Witch_Skill_Laser& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_Laser::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_LASER] = 0;
	m_pPositionOffset[TYPE_E_LASER] = _float3(0.f, 1.5f, -50.f);
	m_pScaleOffset[TYPE_E_LASER] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_LASER] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_LASER_SMALLER] = 0;
	m_pPositionOffset[TYPE_E_LASER_SMALLER] = _float3(0.f, 0.05f, -0.050f);
	m_pScaleOffset[TYPE_E_LASER_SMALLER] = _float3(30.f, 30.f, 1000.f);
	m_pRotationOffset[TYPE_E_LASER_SMALLER] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Skill_Laser::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_Laser::Tick(_float fTimeDelta)
{
	if (!m_bIsCreate)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Rage3_Laser"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_LASER], m_pScaleOffset[TYPE_E_LASER], m_pRotationOffset[TYPE_E_LASER], nullptr, &m_pLaser);
	
		m_bIsCreate = true;
	}
	else
	{
		if (m_pLaser != nullptr)
		{
			if (m_pLaser->Is_Dead() || m_pLaser->Get_DieEffect())
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_Rage3_Laser_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_LASER_SMALLER], m_pScaleOffset[TYPE_E_LASER_SMALLER], m_pRotationOffset[TYPE_E_LASER_SMALLER]);

				Set_Dead(true);
			}
		}
	}
}

void CVfx_Witch_Skill_Laser::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_Laser::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_Laser::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_Laser* CVfx_Witch_Skill_Laser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_Laser* pInstance = new CVfx_Witch_Skill_Laser(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_Laser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_Laser::Clone(void* pArg)
{
	CVfx_Witch_Skill_Laser* pInstance = new CVfx_Witch_Skill_Laser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_Laser");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_Laser::Free()
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