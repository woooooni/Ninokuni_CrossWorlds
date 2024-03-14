#include "stdafx.h"
#include "Vfx_Engineer_Skill_Destruction_Boom.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"

CVfx_Engineer_Skill_Destruction_Boom::CVfx_Engineer_Skill_Destruction_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_Destruction_Boom::CVfx_Engineer_Skill_Destruction_Boom(const CVfx_Engineer_Skill_Destruction_Boom& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_Destruction_Boom::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// Æã
	{
		m_pFrameTriger[TYPE_ET2_D_CRACK] = 0;
		m_pPositionOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_D_CRACK] = _float3(10.f, 3.f, 10.f);
		m_pRotationOffset[TYPE_ET2_D_CRACK] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE_BIG] = 0;
		m_pPositionOffset[TYPE_ET2_P_FIRE_BIG] = _float3(0.f, 1.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE_BIG] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE_BIG] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_FIRE_SMALL] = 0;
		m_pPositionOffset[TYPE_ET2_P_FIRE_SMALL] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_FIRE_SMALL] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_FIRE_SMALL] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 0;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 0;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_STONE] = 0;
		m_pPositionOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_STONE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_STONE] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_P_SMOKE] = 0;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE] = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction_Boom::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_Destruction_Boom::Tick(_float fTimeDelta)
{
	if (m_iCount == TYPE_ET2_D_CRACK && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_D_CRACK])
	{
		m_WorldMatrix = m_pOwnerObject->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldFloat4x4();
		GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Engineer_Skill_Destruction_Boom_Crack"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_D_CRACK], m_pScaleOffset[TYPE_ET2_D_CRACK], m_pRotationOffset[TYPE_ET2_D_CRACK]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_ET2_P_FIRE_BIG && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE_BIG])
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_Destruction_Boom_Fire_Big"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRE_BIG], m_pScaleOffset[TYPE_ET2_P_FIRE_BIG], m_pRotationOffset[TYPE_ET2_P_FIRE_BIG]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_ET2_P_FIRE_SMALL && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_FIRE_SMALL])
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_ExplosionShot_Boom_Fire_Small"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_FIRE_SMALL], m_pScaleOffset[TYPE_ET2_P_FIRE_SMALL], m_pRotationOffset[TYPE_ET2_P_FIRE_SMALL]);
		m_iCount++;
	}

	else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_ExplosionShot_Boom_CircleLine_Big"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINE], m_pScaleOffset[TYPE_ET2_E_CIRCLELINE], m_pRotationOffset[TYPE_ET2_E_CIRCLELINE]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_ExplosionShot_Boom_Circles"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_ET2_P_STONE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_STONE])
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_ExplosionShot_Boom_Stone"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_STONE], m_pScaleOffset[TYPE_ET2_P_STONE], m_pRotationOffset[TYPE_ET2_P_STONE]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
	{
		//GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT(""),
		//	XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_SMOKE], m_pScaleOffset[TYPE_ET2_P_SMOKE], m_pRotationOffset[TYPE_ET2_P_SMOKE]);
		m_iCount++;
	}

	else if (m_iCount == TYPE_END)
		Set_Dead(true);
}

void CVfx_Engineer_Skill_Destruction_Boom::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_Destruction_Boom::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_Destruction_Boom::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_Destruction_Boom* CVfx_Engineer_Skill_Destruction_Boom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_Destruction_Boom* pInstance = new CVfx_Engineer_Skill_Destruction_Boom(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_Destruction_Boom");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_Destruction_Boom::Clone(void* pArg)
{
	CVfx_Engineer_Skill_Destruction_Boom* pInstance = new CVfx_Engineer_Skill_Destruction_Boom(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_Destruction_Boom");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_Destruction_Boom::Free()
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