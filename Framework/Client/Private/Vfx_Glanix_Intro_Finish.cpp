#include "stdafx.h"
#include "Vfx_Glanix_Intro_Finish.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"
#include "Effect.h"

CVfx_Glanix_Intro_Finish::CVfx_Glanix_Intro_Finish(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Intro_Finish::CVfx_Glanix_Intro_Finish(const CVfx_Glanix_Intro_Finish& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Intro_Finish::Initialize_Prototype()
{
	m_bOwnerStateIndex = CGlanix::GLANIX_INTRO_FINISH;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pFrameTriger[TYPE_P_SMOKE]    = 15;
	m_pPositionOffset[TYPE_P_SMOKE] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_SMOKE] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_CIRCLES] = 15;
	m_pPositionOffset[TYPE_P_CIRCLES] = _float3(0.f, 1.f, 0.f);
	m_pScaleOffset[TYPE_P_CIRCLES] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_CIRCLES] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_P_ATCHI]    = 84;
	m_pPositionOffset[TYPE_P_ATCHI] = _float3(1.f, 3.f, 1.f);
	m_pScaleOffset[TYPE_P_ATCHI]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_P_ATCHI] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Glanix_Intro_Finish::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Intro_Finish::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_SMOKE])
		{
			CTransform* pTransformCom = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (pTransformCom != nullptr)
			{
				m_WorldMatrix = pTransformCom->Get_WorldFloat4x4();
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Intro_Smoke"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_SMOKE], m_pScaleOffset[TYPE_P_SMOKE], m_pRotationOffset[TYPE_P_SMOKE]);
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Intro_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_CIRCLES], m_pScaleOffset[TYPE_P_CIRCLES], m_pRotationOffset[TYPE_P_CIRCLES]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_P_ATCHI && m_iOwnerFrame >= m_pFrameTriger[TYPE_P_ATCHI])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Atchi_03"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_P_ATCHI], m_pScaleOffset[TYPE_P_ATCHI], m_pRotationOffset[TYPE_P_ATCHI]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Intro_Finish::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Intro_Finish::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Intro_Finish::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Intro_Finish* CVfx_Glanix_Intro_Finish::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Intro_Finish* pInstance = new CVfx_Glanix_Intro_Finish(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Intro_Finish");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Intro_Finish::Clone(void* pArg)
{
	CVfx_Glanix_Intro_Finish* pInstance = new CVfx_Glanix_Intro_Finish(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Intro_Finish");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Intro_Finish::Free()
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