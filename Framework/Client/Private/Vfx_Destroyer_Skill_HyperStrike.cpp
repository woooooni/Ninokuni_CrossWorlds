#include "stdafx.h"
#include "Vfx_Destroyer_Skill_HyperStrike.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_HyperStrike::CVfx_Destroyer_Skill_HyperStrike(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_HyperStrike::CVfx_Destroyer_Skill_HyperStrike(const CVfx_Destroyer_Skill_HyperStrike& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_BURST;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_P_SPARKLE] = 33;
		m_pPositionOffset[TYPE_ET1_P_SPARKLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SPARKLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 33;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 33;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_SPARKLE] = 40;
		m_pPositionOffset[TYPE_ET2_D_SPARKLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_SPARKLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRES] = 40;
		m_pPositionOffset[TYPE_ET2_P_FIRES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SPRINGUP] = 40;
		m_pPositionOffset[TYPE_ET2_P_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SPRINGUP]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 40;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_D_CIRCLE] = 45;
		m_pPositionOffset[TYPE_ET3_D_CIRCLE] = _float3(0.f, 0.f, 1.f);
		m_pScaleOffset[TYPE_ET3_D_CIRCLE]    = _float3(10.f, 5.f, 10.f);
		m_pRotationOffset[TYPE_ET3_D_CIRCLE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_D_CRACK] = 45;
		m_pPositionOffset[TYPE_ET3_D_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_D_CRACK]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_SPRINGUP] = 45;
		m_pPositionOffset[TYPE_ET3_P_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_SPRINGUP]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_FIRES] = 45;
		m_pPositionOffset[TYPE_ET3_P_FIRES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_FIRES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_FIRES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_SMOKE] = 45;
		m_pPositionOffset[TYPE_ET3_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_SMOKE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 45;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_STONECRACK] = 45;
		m_pPositionOffset[TYPE_ET3_E_STONECRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_STONECRACK]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_STONECRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_STONE] = 45;
		m_pPositionOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_STONE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_CIRCLELINE] = 45;
		m_pPositionOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_HyperStrike::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SPARKLE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			m_iCount++;
		}
		
		else if (m_iCount == TYPE_ET2_D_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_SPARKLE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPRINGUP])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_D_CIRCLE], m_pScaleOffset[TYPE_ET3_D_CIRCLE], m_pRotationOffset[TYPE_ET3_D_CIRCLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_D_CRACK])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_SPRINGUP])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_FIRES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_FIRES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_SMOKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_STONECRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_STONECRACK])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_STONE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_CIRCLELINE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_HyperStrike::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_HyperStrike::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_HyperStrike* CVfx_Destroyer_Skill_HyperStrike::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_HyperStrike* pInstance = new CVfx_Destroyer_Skill_HyperStrike(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_HyperStrike");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_HyperStrike::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_HyperStrike* pInstance = new CVfx_Destroyer_Skill_HyperStrike(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_HyperStrike");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_HyperStrike::Free()
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