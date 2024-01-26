#include "stdafx.h"
#include "Vfx_Engineer_Skill_ExplosionShot.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_ExplosionShot::CVfx_Engineer_Skill_ExplosionShot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_ExplosionShot::CVfx_Engineer_Skill_ExplosionShot(const CVfx_Engineer_Skill_ExplosionShot& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_2;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_RECT] = 0;
		m_pPositionOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_RECT]    = _float3(4.f, 5.f, 8.f);
		m_pRotationOffset[TYPE_ET1_D_RECT] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET1_E_CIRCLELINE] = 16;
		m_pPositionOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_E_CIRCLES] = 16;
		m_pPositionOffset[TYPE_ET1_E_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_E_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_E_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_FIRE] = 16;
		m_pPositionOffset[TYPE_ET1_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_FIRE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_SMOKE] = 16;
		m_pPositionOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SMOKE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_D_CIRCLE] = 34;
		m_pPositionOffset[TYPE_ET2_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_CIRCLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_CIRCLE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_DOME] = 34;
		m_pPositionOffset[TYPE_ET2_P_DOME] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_DOME]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_DOME] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE] = 34;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 34;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 34;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 34;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_ExplosionShot::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_RECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_RECT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_FrozenStorm_Square"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_RECT], m_pScaleOffset[TYPE_ET1_D_RECT], m_pRotationOffset[TYPE_ET1_D_RECT]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET1_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLELINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_E_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_CIRCLES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_FIRE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_FIRE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SMOKE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CIRCLE])
		{

			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_DOME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_DOME])
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
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
		{
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Engineer_Skill_ExplosionShot::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_ExplosionShot* CVfx_Engineer_Skill_ExplosionShot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_ExplosionShot* pInstance = new CVfx_Engineer_Skill_ExplosionShot(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_ExplosionShot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_ExplosionShot::Clone(void* pArg)
{
	CVfx_Engineer_Skill_ExplosionShot* pInstance = new CVfx_Engineer_Skill_ExplosionShot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_ExplosionShot");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_ExplosionShot::Free()
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