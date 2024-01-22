#include "stdafx.h"
#include "Vfx_Destroyer_Skill_LeafSlam.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Destroyer_Skill_LeafSlam::CVfx_Destroyer_Skill_LeafSlam(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Destroyer_Skill_LeafSlam::CVfx_Destroyer_Skill_LeafSlam(const CVfx_Destroyer_Skill_LeafSlam& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_BROWN] = 16;
		m_pPositionOffset[TYPE_ET2_D_BROWN] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_BROWN]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_BROWN] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SPARKLE] = 16;
		m_pPositionOffset[TYPE_ET2_P_SPARKLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SPARKLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_D_FIREBROWN] = 16;
		m_pPositionOffset[TYPE_ET2_D_FIREBROWN] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_FIREBROWN]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_FIREBROWN] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SPRINGUP] = 16;
		m_pPositionOffset[TYPE_ET2_P_SPRINGUP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SPRINGUP]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SPRINGUP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_D_CRACK] = 16;
		m_pPositionOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_CRACK]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE] = 16;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 16;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_STONES] = 16;
		m_pPositionOffset[TYPE_ET2_P_STONES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_STONES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_STONES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRERIGID] = 16;
		m_pPositionOffset[TYPE_ET2_P_FIRERIGID] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRERIGID]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRERIGID] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 16;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Destroyer_Skill_LeafSlam::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE], m_pOwnerObject);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_D_BROWN && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_BROWN])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPARKLE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_D_FIREBROWN && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_FIREBROWN])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SPRINGUP && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPRINGUP])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CRACK])
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
		else if (m_iCount == TYPE_ET2_P_STONES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_STONES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_FIRERIGID && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRERIGID])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Destroyer_Skill_LeafSlam::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Render()
{
	return S_OK;
}

HRESULT CVfx_Destroyer_Skill_LeafSlam::Ready_Components()
{
	return S_OK;
}

CVfx_Destroyer_Skill_LeafSlam* CVfx_Destroyer_Skill_LeafSlam::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Destroyer_Skill_LeafSlam* pInstance = new CVfx_Destroyer_Skill_LeafSlam(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Destroyer_Skill_LeafSlam");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Destroyer_Skill_LeafSlam::Clone(void* pArg)
{
	CVfx_Destroyer_Skill_LeafSlam* pInstance = new CVfx_Destroyer_Skill_LeafSlam(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Destroyer_Skill_LeafSlam");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Destroyer_Skill_LeafSlam::Free()
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