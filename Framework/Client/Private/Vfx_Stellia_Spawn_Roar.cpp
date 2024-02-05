#include "stdafx.h"
#include "Vfx_Stellia_Spawn_Roar.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Spawn_Roar::CVfx_Stellia_Spawn_Roar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Spawn_Roar::CVfx_Stellia_Spawn_Roar(const CVfx_Stellia_Spawn_Roar& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Spawn_Roar::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_SPAWN;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	{
		m_pFrameTriger[TYPE_ET1_P_SMOKE] = 31;
		m_pPositionOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.7f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 32;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.5f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_ROAR] = 47;
		m_pPositionOffset[TYPE_ET2_E_ROAR] = _float3(30.f, 0.f, -10.f);
		m_pScaleOffset[TYPE_ET2_E_ROAR]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_ET2_E_ROAR] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Stellia_Spawn_Roar::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Spawn_Roar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SMOKE])
		{
			CTransform* pCTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			if (pCTransform != nullptr)
				m_WorldMatrix = pCTransform->Get_WorldFloat4x4();
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_SMOKE], m_pScaleOffset[TYPE_ET1_P_SMOKE], m_pRotationOffset[TYPE_ET1_P_SMOKE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES]);
			
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Spawn_Circle_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES]);

			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_ROAR && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_ROAR])
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc >= 0.225f) // 47 65 95 117 131 166 176 /0.5 0.0 0.0 0.0
			{
				CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
				if (pOwnerModel != nullptr)
				{
					m_fTimeAcc = 0.f;

					Matrix TongueMatrix = pOwnerModel->Get_SocketLocalMatrix(2) * m_WorldMatrix; // 뼈 행렬 x 플레이어 월드 행렬 = 뼈 월드 행렬
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Roar_TrailLine_01"), TongueMatrix, m_pPositionOffset[TYPE_ET2_E_ROAR], m_pScaleOffset[TYPE_ET2_E_ROAR], m_pRotationOffset[TYPE_ET2_E_ROAR]);
				}				
			}

			if(m_iOwnerFrame >= 71)
				m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Stellia_Spawn_Roar::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Spawn_Roar::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Spawn_Roar::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Spawn_Roar* CVfx_Stellia_Spawn_Roar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Spawn_Roar* pInstance = new CVfx_Stellia_Spawn_Roar(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Spawn_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Spawn_Roar::Clone(void* pArg)
{
	CVfx_Stellia_Spawn_Roar* pInstance = new CVfx_Stellia_Spawn_Roar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Spawn_Roar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Spawn_Roar::Free()
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