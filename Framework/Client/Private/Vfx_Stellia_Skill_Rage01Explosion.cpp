#include "stdafx.h"
#include "Vfx_Stellia_Skill_Rage01Explosion.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Stellia_Skill_Rage01Explosion::CVfx_Stellia_Skill_Rage01Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Rage01Explosion::CVfx_Stellia_Skill_Rage01Explosion(const CVfx_Stellia_Skill_Rage01Explosion& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_RAGE1LOOP_EXPLOSION;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	/* 0. Warning Decal */
	m_pFrameTriger[TYPE_D_DECAL_00] = 0;
	m_pPositionOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_DECAL_00] = _float3(50.f, 1.f, 50.f);
	m_pRotationOffset[TYPE_D_DECAL_00] = _float3(0.f, 0.f, 0.f);

	/* 1. SafeZone Decal */
	m_pFrameTriger[TYPE_D_DECAL_01] = 1;
	m_pPositionOffset[TYPE_D_DECAL_01] = _float3(0.26f, 0.f, 0.f);
	m_pScaleOffset[TYPE_D_DECAL_01] = _float3(20.f, 2.f, 19.5f);
	m_pRotationOffset[TYPE_D_DECAL_01] = _float3(0.f, -90.f, 0.f);

	/* 2. ExplosionReady Particle */
	m_pFrameTriger[TYPE_P_EXPOLSION_READY] = 5;
	m_pPositionOffset[TYPE_P_EXPOLSION_READY] = _float3(0.f, -0.5f, 0.f);
	m_pScaleOffset[TYPE_P_EXPOLSION_READY] = _float3(10.f, 1.f, 10.f);
	m_pRotationOffset[TYPE_P_EXPOLSION_READY] = _float3(0.f, -0.f, 0.f);


	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Rage01Explosion::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_D_DECAL_00 && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_DECAL_00])
		{
			m_WorldMatrix = m_pOwnerObject->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_WorldFloat4x4();

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Stellia_Rage01Crack"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_DECAL_00], m_pScaleOffset[TYPE_D_DECAL_00], m_pRotationOffset[TYPE_D_DECAL_00]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_D_DECAL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_D_DECAL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Stellia_Rage01SafeZone"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_D_DECAL_01], m_pScaleOffset[TYPE_D_DECAL_01], m_pRotationOffset[TYPE_D_DECAL_01]);
			m_iCount++;
		}

		//else if (m_iCount == TYPE_P_EXPOLSION_READY && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_EXPOLSION_READY])
		//{
		//	GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Rage01_FnishExplosion_Ready"),
		//		XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_EXPOLSION_READY], m_pScaleOffset[TYPE_P_EXPOLSION_READY], m_pRotationOffset[TYPE_P_EXPOLSION_READY]);
		//	m_iCount++;
		//}
	}
}

void CVfx_Stellia_Skill_Rage01Explosion::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Rage01Explosion::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Rage01Explosion* CVfx_Stellia_Skill_Rage01Explosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Rage01Explosion* pInstance = new CVfx_Stellia_Skill_Rage01Explosion(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Rage01Explosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Rage01Explosion::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Rage01Explosion* pInstance = new CVfx_Stellia_Skill_Rage01Explosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Rage01Explosion");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Rage01Explosion::Free()
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