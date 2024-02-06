#include "stdafx.h"
#include "Vfx_Stellia_Skill_Roar.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_Roar::CVfx_Stellia_Skill_Roar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Roar::CVfx_Stellia_Skill_Roar(const CVfx_Stellia_Skill_Roar& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Roar::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pFrameTriger[TYPE_ET1_E_ROAR] = 57;
	m_pPositionOffset[TYPE_ET1_E_ROAR] = _float3(35.f, 0.f, -15.f);
	m_pScaleOffset[TYPE_ET1_E_ROAR]    = _float3(0.2f, 0.2f, 0.2f);
	m_pRotationOffset[TYPE_ET1_E_ROAR] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Roar::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Roar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_E_ROAR && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_ROAR])
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc >= 0.225f)
			{
				CModel* pOwnerModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
				if (pOwnerModel != nullptr)
				{
					m_fTimeAcc = 0.f;

					Matrix TongueMatrix = pOwnerModel->Get_SocketLocalMatrix(2) * m_WorldMatrix;
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Roar_TrailLine_01"), TongueMatrix, m_pPositionOffset[TYPE_ET1_E_ROAR], m_pScaleOffset[TYPE_ET1_E_ROAR], m_pRotationOffset[TYPE_ET1_E_ROAR]);
				}
			}

			if (m_iOwnerFrame >= m_iFinishFrame)
				m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Stellia_Skill_Roar::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Roar::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Roar::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Roar* CVfx_Stellia_Skill_Roar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Roar* pInstance = new CVfx_Stellia_Skill_Roar(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Roar::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Roar* pInstance = new CVfx_Stellia_Skill_Roar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Roar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Roar::Free()
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