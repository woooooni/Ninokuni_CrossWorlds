#include "stdafx.h"
#include "Vfx_Glanix_Skill_SwingDownDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_SwingDownDown::CVfx_Glanix_Skill_SwingDownDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_SwingDownDown::CVfx_Glanix_Skill_SwingDownDown(const CVfx_Glanix_Skill_SwingDownDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_SwingDownDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_SPINBOMBBOMB;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_D_START_WARNING]    = 0;
	m_pPositionOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_START_WARNING]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_SwingDownDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_SwingDownDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_START_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_START_WARNING])
		{
			m_iCount++;
		}

		//else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_SwingDownDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_SwingDownDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_SwingDownDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_SwingDownDown* CVfx_Glanix_Skill_SwingDownDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_SwingDownDown* pInstance = new CVfx_Glanix_Skill_SwingDownDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_SwingDownDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_SwingDownDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_SwingDownDown* pInstance = new CVfx_Glanix_Skill_SwingDownDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_SwingDownDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_SwingDownDown::Free()
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