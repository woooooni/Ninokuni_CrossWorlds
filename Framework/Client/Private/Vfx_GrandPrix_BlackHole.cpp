#include "stdafx.h"
#include "Vfx_GrandPrix_BlackHole.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"
#include "Decal.h"

CVfx_GrandPrix_BlackHole::CVfx_GrandPrix_BlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_GrandPrix_BlackHole::CVfx_GrandPrix_BlackHole(const CVfx_GrandPrix_BlackHole& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_GrandPrix_BlackHole::Initialize_Prototype()
{
	// m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_SPHERE_CREATE] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_CREATE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_CREATE] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE_CREATE] = _float3(0.f, 0.f, 0.f);

	// 1
	m_pFrameTriger[TYPE_E_SPHERE_DRAIN] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_DRAIN] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_DRAIN] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE_DRAIN] = _float3(0.f, 0.f, 0.f);

	//2 
	m_pFrameTriger[TYPE_E_SPHERE_DRAIN02] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_DRAIN02] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_DRAIN02] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE_DRAIN02] = _float3(0.f, 0.f, 0.f);

	// 3
	m_pFrameTriger[TYPE_E_SPHERE_OUTLINE] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_OUTLINE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_OUTLINE] = _float3(0.1f, 0.1f, 0.1f);
	m_pRotationOffset[TYPE_E_SPHERE_OUTLINE] = _float3(0.f, 0.f, 0.f);

	// 4
	m_pFrameTriger[TYPE_E_SPHERE_DELETE] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_DELETE] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_DELETE] = _float3(30.f, 30.f, 30.f);
	m_pRotationOffset[TYPE_E_SPHERE_DELETE] = _float3(0.f, 0.f, 0.f);

	// 5
	m_pFrameTriger[TYPE_E_SPHERE_DELETE2] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_DELETE2] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_DELETE2] = _float3(30.f, 30.f, 30.f);
	m_pRotationOffset[TYPE_E_SPHERE_DELETE2] = _float3(0.f, 0.f, 0.f);

	// 6
	m_pFrameTriger[TYPE_E_SPHERE_DELETE3] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_DELETE3] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_DELETE3] = _float3(20.f, 20.f, 20.f);
	m_pRotationOffset[TYPE_E_SPHERE_DELETE3] = _float3(0.f, 0.f, 0.f);

	// 7
	m_pFrameTriger[TYPE_E_SPHERE_DELETE4] = 0;
	m_pPositionOffset[TYPE_E_SPHERE_DELETE4] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_E_SPHERE_DELETE4] = _float3(30.f, 30.f, 30.f);
	m_pRotationOffset[TYPE_E_SPHERE_DELETE4] = _float3(0.f, 0.f, 0.f);


	return S_OK;
}

HRESULT CVfx_GrandPrix_BlackHole::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_GrandPrix_BlackHole::Tick(_float fTimeDelta)
{
	// 블랙홀 최초 생성
	if (!m_bIsCreate && m_pBlackHole == nullptr)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BlackHole_Sphere_Create"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_CREATE], m_pScaleOffset[TYPE_E_SPHERE_CREATE], m_pRotationOffset[TYPE_E_SPHERE_CREATE], nullptr, &m_pBlackHole);

		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BalckHole_Drain"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_DRAIN], m_pScaleOffset[TYPE_E_SPHERE_DRAIN], m_pRotationOffset[TYPE_E_SPHERE_DRAIN], nullptr, &m_pDrain);
		
		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BlackHole_Drain02"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_DRAIN02], m_pScaleOffset[TYPE_E_SPHERE_DRAIN02], m_pRotationOffset[TYPE_E_SPHERE_DRAIN02]);

		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BlackHole_OutLine"),
			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_OUTLINE], m_pScaleOffset[TYPE_E_SPHERE_OUTLINE], m_pRotationOffset[TYPE_E_SPHERE_OUTLINE]);

		m_bIsCreate = true;
	}
	else if (!m_bIsSmaller && m_pBlackHoleSmaller == nullptr)
	{
		// 블랙홀 이펙트가 끝났다면
		if (m_pBlackHole->Is_Dead() || m_pBlackHole->Get_DieEffect())
		{
			// 작아지는 블랙홀 생성
			if (!m_bIsSmaller && m_pBlackHoleSmaller == nullptr)
			{
				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BlackHole_Sphere_Delete"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_DELETE], m_pScaleOffset[TYPE_E_SPHERE_DELETE], m_pRotationOffset[TYPE_E_SPHERE_DELETE]);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BalckHole_Drain_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_DELETE2], m_pScaleOffset[TYPE_E_SPHERE_DELETE2], m_pRotationOffset[TYPE_E_SPHERE_DELETE2]);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BlackHole_Drain02_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_DELETE3], m_pScaleOffset[TYPE_E_SPHERE_DELETE3], m_pRotationOffset[TYPE_E_SPHERE_DELETE3]);

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_GrandPrix_BlackHole_OutLine_Smaller"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_DELETE4], m_pScaleOffset[TYPE_E_SPHERE_DELETE4], m_pRotationOffset[TYPE_E_SPHERE_DELETE4]);

				m_bIsSmaller = true;
			}
		}
	}
	//// 작아지는 이펙트가 끝났다면
	//else if (!m_bIsSmaller02 && m_pBlackHoleSmaller02 == nullptr)
	//{
	//	if (m_pBlackHoleSmaller->Is_Dead() || m_pBlackHoleSmaller->Get_DieEffect())
	//	{
	//		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Sphere_Smaller02"),
	//			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_SPHERE_SMALLER02], m_pScaleOffset[TYPE_E_SPHERE_SMALLER02], m_pRotationOffset[TYPE_E_SPHERE_SMALLER02], nullptr, &m_pBlackHoleSmaller02);
	//
	//		GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Witch_BalckHole_Drain_Smaller02"),
	//			XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_DRAND_SMALLER], m_pScaleOffset[TYPE_E_DRAND_SMALLER02], m_pRotationOffset[TYPE_E_DRAND_SMALLER02]);
	//
	//		m_bIsSmaller02 = true;
	//	}
	//}
	//
	//// 모든 블랙홀이 끝났다면 
	//if (m_pBlackHoleSmaller02 != nullptr)
	//{
	//	if (m_pBlackHoleSmaller02->Is_Dead() || m_pBlackHoleSmaller02->Get_DieEffect())
	//	{
	//		m_pWarningDecal->Set_Dead(true);
	//		Set_Dead(true);
	//	}
	//}
}

void CVfx_GrandPrix_BlackHole::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_GrandPrix_BlackHole::Render()
{
	return S_OK;
}

HRESULT CVfx_GrandPrix_BlackHole::Ready_Components()
{
	return S_OK;
}

CVfx_GrandPrix_BlackHole* CVfx_GrandPrix_BlackHole::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_GrandPrix_BlackHole* pInstance = new CVfx_GrandPrix_BlackHole(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_GrandPrix_BlackHole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_GrandPrix_BlackHole::Clone(void* pArg)
{
	CVfx_GrandPrix_BlackHole* pInstance = new CVfx_GrandPrix_BlackHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_GrandPrix_BlackHole");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_GrandPrix_BlackHole::Free()
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