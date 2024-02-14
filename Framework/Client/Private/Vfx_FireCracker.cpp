#include "stdafx.h"
#include "Vfx_FireCracker.h"

#include "Particle_Manager.h"
#include "Utils.h"

CVfx_FireCracker::CVfx_FireCracker(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_FireCracker::CVfx_FireCracker(const CVfx_FireCracker& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_FireCracker::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_FireCracker::Initialize(void* pArg)
{
 	return S_OK;
}

void CVfx_FireCracker::Tick(_float fTimeDelta)
{
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc > m_fNextTime)
	{
		m_fTimeAcc  = 0.f;
		m_fNextTime = CUtils::Random_Float(0.3f, 0.6f);

		_int iRandomCount = CUtils::Random_Int(0, 8);
		wstring FileNameString = L"Particle_FireCracker_0" + to_wstring(iRandomCount);

		_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		WorldMatrix.r[3] = XMVectorSet(CUtils::Random_Float(-40.f, 130.f), CUtils::Random_Float(25.f, 50.f), CUtils::Random_Float(70.f, 150.f), 1.f);
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(FileNameString, WorldMatrix,
			_float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));
	}
}

void CVfx_FireCracker::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_FireCracker::Render()
{
	return S_OK;
}

HRESULT CVfx_FireCracker::Ready_Components()
{
	return S_OK;
}

CVfx_FireCracker* CVfx_FireCracker::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_FireCracker* pInstance = new CVfx_FireCracker(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_FireCracker");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_FireCracker::Clone(void* pArg)
{
	CVfx_FireCracker* pInstance = new CVfx_FireCracker(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_FireCracker");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_FireCracker::Free()
{
	__super::Free();
}