#include "stdafx.h"
#include "Vfx_Glanix_Skill_SwingDown.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Skill_SwingDown::CVfx_Glanix_Skill_SwingDown(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Skill_SwingDown::CVfx_Glanix_Skill_SwingDown(const CVfx_Glanix_Skill_SwingDown& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Skill_SwingDown::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_SPINBOMB;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_D_START_WARNING]    = 0;
		m_pPositionOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_START_WARNING]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_D_START_WARNING] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_E_SWING_HANDCIRCLE_RIGHT] = 0;
		m_pPositionOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_SWING_HANDCIRCLE_RIGHT] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_01] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_01] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_01] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_01] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_01] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_01] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_01] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_02] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_02] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_02] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_02] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_02] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_02] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_02] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_03] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_03] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_03] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_03] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_03] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_03] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_03] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_04] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_04] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_04] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_04] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_04] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_04] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_04] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_05] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_05] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_05] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_05] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_05] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_05] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_05] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_05] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_06] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_06] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_06] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_06] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_06] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_06] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_06] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_07] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_07] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_07] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_07] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_07] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_07] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_07] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_07] = _float3(0.f, 0.f, 0.f);
		//
		m_pFrameTriger[TYPE_P_SWING_SMOKE_08] = 0;
		m_pPositionOffset[TYPE_P_SWING_SMOKE_08] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_SMOKE_08] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_SMOKE_08] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_SWING_CIRCLES_08] = 0;
		m_pPositionOffset[TYPE_P_SWING_CIRCLES_08] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_SWING_CIRCLES_08] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_SWING_CIRCLES_08] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_D_ATTACK_WARNING] = 0;
		m_pPositionOffset[TYPE_D_ATTACK_WARNING] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_ATTACK_WARNING] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_D_ATTACK_WARNING] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_HANDCIRCLE_LEFT] = 0;
		m_pPositionOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_ATTACK_HANDCIRCLE_LEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_D_ATTACK_CRACK] = 0;
		m_pPositionOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_D_ATTACK_CRACK] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_D_ATTACK_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_SPRINGUP] = 0;
		m_pPositionOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_ATTACK_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_SMOKE] = 0;
		m_pPositionOffset[TYPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_SMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_P_ATTACK_CIRCLES] = 0;
		m_pPositionOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_P_ATTACK_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_P_ATTACK_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_E_ATTACK_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_E_ATTACK_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
    }

 	return S_OK;
}

HRESULT CVfx_Glanix_Skill_SwingDown::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Skill_SwingDown::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_START_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_START_WARNING])
		{
			m_iCount++;
		}
		//

		else if (m_iCount == TYPE_E_SWING_HANDCIRCLE_RIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_SWING_HANDCIRCLE_RIGHT])
		{
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_P_SWING_SMOKE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_01])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_01])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_02])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_02])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_03])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_03 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_03])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_04])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_04 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_04])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_05])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_05 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_05])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_06])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_06 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_06])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_07 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_07])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_07 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_07])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_P_SWING_SMOKE_08 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_SMOKE_08])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_SWING_CIRCLES_08 && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SWING_CIRCLES_08])
		{
			m_iCount++;
		}
		//

		else if (m_iCount == TYPE_D_ATTACK_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_ATTACK_WARNING])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_HANDCIRCLE_LEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_HANDCIRCLE_LEFT])
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
		else if (m_iCount == TYPE_P_ATTACK_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_SMOKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATTACK_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATTACK_CIRCLES])
		{
		    m_iCount++;
		}
		else if (m_iCount == TYPE_E_ATTACK_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_ATTACK_CIRCLELINE])
		{
		    m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Skill_SwingDown::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Skill_SwingDown::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Skill_SwingDown::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Skill_SwingDown* CVfx_Glanix_Skill_SwingDown::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Skill_SwingDown* pInstance = new CVfx_Glanix_Skill_SwingDown(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Skill_SwingDown");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Skill_SwingDown::Clone(void* pArg)
{
	CVfx_Glanix_Skill_SwingDown* pInstance = new CVfx_Glanix_Skill_SwingDown(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Skill_SwingDown");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Skill_SwingDown::Free()
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