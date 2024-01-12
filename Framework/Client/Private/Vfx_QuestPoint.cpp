#include "stdafx.h"
#include "Vfx_QuestPoint.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

#include "Decal.h"
#include "Particle.h"

CVfx_QuestPoint::CVfx_QuestPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_QuestPoint::CVfx_QuestPoint(const CVfx_QuestPoint& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_QuestPoint::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_QuestPoint::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_QuestPoint::Tick(_float fTimeDelta)
{
	if (m_iCount == 4)
		return;

	m_fTimeAcc += fTimeDelta;
	if (m_iCount == 0 && m_fTimeAcc > 0.f)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_QuestPoint"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 0.f, 0.f), nullptr, &pDecalObject, false);
		Safe_AddRef(pDecalObject);
		m_iCount++;
	}

	else if (m_iCount == 1 && m_fTimeAcc > 0.2f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_QuestPoint_Leaf"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.5f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pLeafObject, false);
		Safe_AddRef(pLeafObject);
		m_iCount++;
	}

	else if (m_iCount == 2 && m_fTimeAcc > 0.4f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_QuestPoint_Circle_01"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pCircle01Object, false);
		Safe_AddRef(pCircle01Object);
		m_iCount++;
	}

	else if (m_iCount == 3 && m_fTimeAcc > 0.6f)
	{
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_QuestPoint_Circle_02"),
			XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &pCircle02Object, false);
		Safe_AddRef(pCircle02Object);
		m_iCount++;
	}
}

void CVfx_QuestPoint::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_QuestPoint::Render()
{
	return S_OK;
}

HRESULT CVfx_QuestPoint::Ready_Components()
{
	return S_OK;
}

CVfx_QuestPoint* CVfx_QuestPoint::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_QuestPoint* pInstance = new CVfx_QuestPoint(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_QuestPoint");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_QuestPoint::Clone(void* pArg)
{
	CVfx_QuestPoint* pInstance = new CVfx_QuestPoint(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_QuestPoint");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_QuestPoint::Free()
{
	__super::Free();

	if (nullptr != pDecalObject)
	{
		pDecalObject->Set_Dead(true);
		Safe_Release(pDecalObject);
	}

	if (nullptr != pLeafObject)
	{
		pLeafObject->Set_Dead(true);
		Safe_Release(pLeafObject);
	}

	if (nullptr != pCircle01Object)
	{
		pCircle01Object->Set_Dead(true);
		Safe_Release(pCircle01Object);
	}

	if (nullptr != pCircle02Object)
	{
		pCircle02Object->Set_Dead(true);
		Safe_Release(pCircle02Object);
	}
}