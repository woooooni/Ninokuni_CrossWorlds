#include "stdafx.h"
#include "Vfx_Engineer_Skill_ExplosionShot_Boom.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_Engineer_Skill_ExplosionShot_Boom::CVfx_Engineer_Skill_ExplosionShot_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_ExplosionShot_Boom::CVfx_Engineer_Skill_ExplosionShot_Boom(const CVfx_Engineer_Skill_ExplosionShot_Boom& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Boom::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// Æã
	{
		// Çª¸¥ ¿ø µ¥Ä®
		m_pFrameTriger[TYPE_ET2_D_CIRCLE] = 34;
		m_pPositionOffset[TYPE_ET2_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_CIRCLE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_D_CIRCLE] = _float3(0.f, 0.f, 0.f);

		// ºÓÀº ¿ø ÀÌÆåÆ®
		m_pFrameTriger[TYPE_ET2_P_DOME] = 34;
		m_pPositionOffset[TYPE_ET2_P_DOME] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_DOME]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_DOME] = _float3(0.f, 0.f, 0.f);

		// Æø¹ß ºÒ²É ÆÄÆ¼Å¬
		m_pFrameTriger[TYPE_ET2_P_FIRE] = 34;
		m_pPositionOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE] = _float3(0.f, 0.f, 0.f);

		// È¸¿À¸® ÀÌÆåÆ®
		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 34;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);

		// ÀÛÀº ¹ÝÂ¦ÀÌ ÆÄÆ¼Å¬
		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 34;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		// ¿¬±â ÆÄÆ¼Å¬
		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 34;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE]    = _float3(5.f, 5.f, 5.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Boom::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_ExplosionShot_Boom::Tick(_float fTimeDelta)
{

}

void CVfx_Engineer_Skill_ExplosionShot_Boom::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Boom::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_ExplosionShot_Boom::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_ExplosionShot_Boom* CVfx_Engineer_Skill_ExplosionShot_Boom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_ExplosionShot_Boom* pInstance = new CVfx_Engineer_Skill_ExplosionShot_Boom(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_ExplosionShot_Boom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_ExplosionShot_Boom::Clone(void* pArg)
{
	CVfx_Engineer_Skill_ExplosionShot_Boom* pInstance = new CVfx_Engineer_Skill_ExplosionShot_Boom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_ExplosionShot_Boom");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_ExplosionShot_Boom::Free()
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