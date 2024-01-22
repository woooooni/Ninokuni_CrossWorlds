#include "stdafx.h"
#include "Vfx_Destroyer_Skill_BattleCry.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_BattleCry::CVfx_Destroyer_Skill_BattleCry(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_BattleCry::CVfx_Destroyer_Skill_BattleCry(const CVfx_Destroyer_Skill_BattleCry& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(8.f, 5.f, 8.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_SPARCKE] = 11;
		m_pPositionOffset[TYPE_ET2_P_SPARCKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SPARCKE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SPARCKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 11;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_E_DOME] = 13;
		m_pPositionOffset[TYPE_ET3_E_DOME] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_DOME]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_DOME] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET3_D_FIRECRACK] = 13;
		m_pPositionOffset[TYPE_ET3_D_FIRECRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_D_FIRECRACK]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_D_FIRECRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_SPRINGUP] = 13;
		m_pPositionOffset[TYPE_ET3_E_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_SPRINGUP]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_E_CIRCLELINE] = 13;
		m_pPositionOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_STONE] = 13;
		m_pPositionOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_STONE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_CIRCLES] = 13;
		m_pPositionOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET3_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_BattleCry::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_SPARCKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPARCKE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_E_DOME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_DOME])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_D_FIRECRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_D_FIRECRACK])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_SPRINGUP])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_STONE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_BattleCry::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_BattleCry::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_BattleCry* CVfx_Destroyer_Skill_BattleCry::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_BattleCry* pInstance = new CVfx_Destroyer_Skill_BattleCry(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_BattleCry");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_BattleCry::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_BattleCry* pInstance = new CVfx_Destroyer_Skill_BattleCry(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_BattleCry");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_BattleCry::Free()
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