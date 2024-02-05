#include "stdafx.h"
#include "Vfx_Witch_Skill_Laser_Warning.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Decal.h"

CVfx_Witch_Skill_Laser_Warning::CVfx_Witch_Skill_Laser_Warning(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Witch_Skill_Laser_Warning::CVfx_Witch_Skill_Laser_Warning(const CVfx_Witch_Skill_Laser_Warning& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Witch_Skill_Laser_Warning::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_D_WARNING] = 0;
	m_pPositionOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_WARNING] = _float3(3.f, 0.5f, 100.f);
	m_pRotationOffset[TYPE_D_WARNING] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_LASERLINE] = 0;
	m_pPositionOffset[TYPE_E_LASERLINE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_LASERLINE] = _float3(3.f, 5.f, 100.f);
	m_pRotationOffset[TYPE_E_LASERLINE] = _float3(0.f, 0.f, 0.f);

	return S_OK;
}

HRESULT CVfx_Witch_Skill_Laser_Warning::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Witch_Skill_Laser_Warning::Tick(_float fTimeDelta)
{
	// 레이저 경고 생성
	if (!m_bIsCreate && m_pWarningDecal == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Stellia_Rage03LaserDecal"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_WARNING], m_pScaleOffset[TYPE_D_WARNING], m_pRotationOffset[TYPE_D_WARNING], nullptr, &m_pWarningDecal);

		//GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain"),
		//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_LASERLINE], m_pScaleOffset[TYPE_E_LASERLINE], m_pRotationOffset[TYPE_E_LASERLINE]);

		m_bIsCreate = true;
	}

	// 라이프 타임이 끝나면 
	else if (m_pWarningDecal != nullptr)
	{
		if (m_pWarningDecal->Is_Dead() || m_pWarningDecal->Get_DecalDead())
		{
			Set_Dead(true);
		}
	}
}

void CVfx_Witch_Skill_Laser_Warning::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Witch_Skill_Laser_Warning::Render()
{
	return S_OK;
}

HRESULT CVfx_Witch_Skill_Laser_Warning::Ready_Components()
{
	return S_OK;
}

CVfx_Witch_Skill_Laser_Warning* CVfx_Witch_Skill_Laser_Warning::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Witch_Skill_Laser_Warning* pInstance = new CVfx_Witch_Skill_Laser_Warning(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Witch_Skill_Laser_Warning");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Witch_Skill_Laser_Warning::Clone(void* pArg)
{
	CVfx_Witch_Skill_Laser_Warning* pInstance = new CVfx_Witch_Skill_Laser_Warning(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Witch_Skill_Laser_Warning");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Witch_Skill_Laser_Warning::Free()
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