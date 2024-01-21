#include "stdafx.h"
#include "Vfx_Engineer_Skill_BurstCall.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_BurstCall::CVfx_Engineer_Skill_BurstCall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_BurstCall::CVfx_Engineer_Skill_BurstCall(const CVfx_Engineer_Skill_BurstCall& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_BurstCall::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE]    = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_LINE] = 0;
		m_pPositionOffset[TYPE_ET1_E_LINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_LINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_LINE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_FIRETRACE] = 0;
		m_pPositionOffset[TYPE_ET2_D_FIRETRACE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_FIRETRACE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_FIRETRACE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_LIGHT] = 0;
		m_pPositionOffset[TYPE_ET2_P_LIGHT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_LIGHT] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_LIGHT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 0;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_FIRE] = 0;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_STONE] = 0;
		m_pPositionOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_STONE] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES] = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_BurstCall::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_BurstCall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_LINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_LINE])
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

		else if (m_iCount == TYPE_ET2_D_FIRETRACE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_FIRETRACE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_LIGHT])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_STONE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_BurstCall::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_BurstCall::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_BurstCall::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_BurstCall* CVfx_Engineer_Skill_BurstCall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_BurstCall* pInstance = new CVfx_Engineer_Skill_BurstCall(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_BurstCall");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_BurstCall::Clone(void* pArg)
{
	CVfx_Engineer_Skill_BurstCall* pInstance = new CVfx_Engineer_Skill_BurstCall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_BurstCall::Free()
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