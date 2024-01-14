#include "stdafx.h"
#include "Vfx_Glanix_Skill_HandDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_HandDown::CVfx_Glanix_Skill_HandDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_HandDown::CVfx_Glanix_Skill_HandDown(const CVfx_Glanix_Skill_HandDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_HandDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_ICEWAVE;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_D_START_WARNING] = 0;
		m_pPositionOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_START_WARNING] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_P_PREACT_BIGSCIRCLE] = 0;
		m_pPositionOffset[TYPE_P_PREACT_BIGSCIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_PREACT_BIGSCIRCLE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_BIGSCIRCLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_PREACT_BIGSMOKE] = 0;
		m_pPositionOffset[TYPE_P_PREACT_BIGSMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_PREACT_BIGSMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_BIGSMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_PREACT_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_E_PREACT_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_PREACT_CIRCLELINE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_PREACT_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_PREACT_CIRCLES] = 0;
		m_pPositionOffset[TYPE_P_PREACT_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_PREACT_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_PREACT_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLELEFT] = 0;
		m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLELEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_PREACT_HANDCIRCLELEFT] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLELEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLERIGHT] = 0;
		m_pPositionOffset[TYPE_E_PREACT_HANDCIRCLERIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_PREACT_HANDCIRCLERIGHT] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_PREACT_HANDCIRCLERIGHT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_D_ATTACK_CRACK] = 0;
		m_pPositionOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_ATTACK_CRACK] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP] = 0;
		m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TTPE_P_ATTACK_SMOKE] = 0;
		m_pPositionOffset[TTPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TTPE_P_ATTACK_SMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TTPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_CIRCLES] = 0;
		m_pPositionOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	return S_OK;
}

HRESULT CVfx_Glanix_Skill_HandDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_HandDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_START_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_START_WARNING])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_PREACT_BIGSCIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_BIGSCIRCLE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_PREACT_BIGSMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_BIGSMOKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_PREACT_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_PREACT_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_PREACT_CIRCLES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_PREACT_HANDCIRCLELEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLELEFT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_PREACT_HANDCIRCLERIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_PREACT_HANDCIRCLERIGHT])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_D_ATTACK_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_ATTACK_CRACK])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP])
		{
			m_iCount++;
		}
		else if (m_iCount == TTPE_P_ATTACK_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TTPE_P_ATTACK_SMOKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATTACK_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_HandDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_HandDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_HandDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_HandDown* CVfx_Glanix_Skill_HandDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_HandDown* pInstance = new CVfx_Glanix_Skill_HandDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_HandDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_HandDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_HandDown* pInstance = new CVfx_Glanix_Skill_HandDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_HandDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_HandDown::Free()
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