#include "stdafx.h"
#include "Vfx_Engineer_Skill_Destruction.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_Destruction::CVfx_Engineer_Skill_Destruction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_Destruction::CVfx_Engineer_Skill_Destruction(const CVfx_Engineer_Skill_Destruction& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_Destruction::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_BURST;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_RECT] = 0;
		m_pPositionOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_RECT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_A_CANNON_LEFT] = 0;
		m_pPositionOffset[TYPE_ET1_A_CANNON_LEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_A_CANNON_LEFT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_A_CANNON_LEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_A_CANNON_RIGHT] = 0;
		m_pPositionOffset[TYPE_ET1_A_CANNON_RIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_A_CANNON_RIGHT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_A_CANNON_RIGHT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_SMOKE] = 0;
		m_pPositionOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SMOKE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_IRON] = 0;
		m_pPositionOffset[TYPE_ET1_E_IRON] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_IRON] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_IRON] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_ENTRANCE_LEFT] = 0;
		m_pPositionOffset[TYPE_ET2_P_ENTRANCE_LEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_ENTRANCE_LEFT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_ENTRANCE_LEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_ENTRANCE_RIGHT] = 0;
		m_pPositionOffset[TYPE_ET2_P_ENTRANCE_RIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_ENTRANCE_RIGHT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_ENTRANCE_RIGHT] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_E_TRAILLINES_LEFT] = 0;
		m_pPositionOffset[TYPE_ET2_E_TRAILLINES_LEFT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_TRAILLINES_LEFT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_TRAILLINES_LEFT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_TRAILLINES_RIGHT] = 0;
		m_pPositionOffset[TYPE_ET2_E_TRAILLINES_RIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_TRAILLINES_RIGHT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_TRAILLINES_RIGHT] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_SMOKES] = 0;
		m_pPositionOffset[TYPE_ET2_P_SMOKES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE] = 0;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_E_IRON] = 0;
		m_pPositionOffset[TYPE_ET3_E_IRON] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_IRON] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_IRON] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_Destruction::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_RECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_RECT])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET1_A_CANNON_LEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_A_CANNON_LEFT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_A_CANNON_RIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_A_CANNON_RIGHT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SMOKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_IRON && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_IRON])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_ENTRANCE_LEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_ENTRANCE_LEFT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_ENTRANCE_RIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_ENTRANCE_RIGHT])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_TRAILLINES_LEFT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_TRAILLINES_LEFT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_TRAILLINES_RIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_TRAILLINES_RIGHT])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_SMOKES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_E_IRON && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_IRON])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_Destruction::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_Destruction::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_Destruction* CVfx_Engineer_Skill_Destruction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_Destruction* pInstance = new CVfx_Engineer_Skill_Destruction(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_Destruction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_Destruction::Clone(void* pArg)
{
	CVfx_Engineer_Skill_Destruction* pInstance = new CVfx_Engineer_Skill_Destruction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_Destruction");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_Destruction::Free()
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