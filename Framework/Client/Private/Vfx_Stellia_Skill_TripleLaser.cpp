#include "stdafx.h"
#include "Vfx_Stellia_Skill_TripleLaser.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_TripleLaser::CVfx_Stellia_Skill_TripleLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_TripleLaser::CVfx_Stellia_Skill_TripleLaser(const CVfx_Stellia_Skill_TripleLaser& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_TRIPLELASER;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pFrameTriger[TYPE_ET1_] = 0;
	m_pPositionOffset[TYPE_ET1_] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_ET1_]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ET1_] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_TripleLaser::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_ && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Stellia_Skill_TripleLaser::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_TripleLaser* CVfx_Stellia_Skill_TripleLaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_TripleLaser* pInstance = new CVfx_Stellia_Skill_TripleLaser(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_TripleLaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_TripleLaser::Clone(void* pArg)
{
	CVfx_Stellia_Skill_TripleLaser* pInstance = new CVfx_Stellia_Skill_TripleLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_TripleLaser");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_TripleLaser::Free()
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