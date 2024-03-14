#include "stdafx.h"
#include "Vfx_Glanix_Intro_Jump.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"

CVfx_Glanix_Intro_Jump::CVfx_Glanix_Intro_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Intro_Jump::CVfx_Glanix_Intro_Jump(const CVfx_Glanix_Intro_Jump& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Intro_Jump::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_INTRO_JUMP;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	// 점프할 때
	m_pFrameTriger[TYPE_E_CIRCLELINE]    = 48;
	m_pPositionOffset[TYPE_E_CIRCLELINE] = _float3(0.f, 0.2f, 0.f);
	m_pScaleOffset[TYPE_E_CIRCLELINE]    = _float3(3.f, 3.f, 3.f);
	m_pRotationOffset[TYPE_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Intro_Jump::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Intro_Jump::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_CIRCLELINE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Glanix_Skill_FootDown_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_CIRCLELINE], m_pScaleOffset[TYPE_E_CIRCLELINE], m_pRotationOffset[TYPE_E_CIRCLELINE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Intro_Jump::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Intro_Jump::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Intro_Jump::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Intro_Jump* CVfx_Glanix_Intro_Jump::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Intro_Jump* pInstance = new CVfx_Glanix_Intro_Jump(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Intro_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Intro_Jump::Clone(void* pArg)
{
	CVfx_Glanix_Intro_Jump* pInstance = new CVfx_Glanix_Intro_Jump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Intro_Jump");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Intro_Jump::Free()
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