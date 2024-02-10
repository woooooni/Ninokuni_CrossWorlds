#include "stdafx.h"
#include "Vfx_Stellia_Skill_Charge.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_Charge::CVfx_Stellia_Skill_Charge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Charge::CVfx_Stellia_Skill_Charge(const CVfx_Stellia_Skill_Charge& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Charge::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_CHARGE;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pFrameTriger[TYPE_ET1_WARNING] = 0;
	m_pPositionOffset[TYPE_ET1_WARNING] = _float3(0.f, 0.f, 0.6f);
	m_pScaleOffset[TYPE_ET1_WARNING]    = _float3(10.f, 2.f, 35.f);
	m_pRotationOffset[TYPE_ET1_WARNING] = _float3(0.f, 0.f, 0.f);

	m_pFrameTriger[TYPE_ET2_SMOKE] = 55;
	m_pPositionOffset[TYPE_ET2_SMOKE] = _float3(0.f, 0.f, -5.f);
	m_pScaleOffset[TYPE_ET2_SMOKE]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ET2_SMOKE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Charge::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Charge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_WARNING])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FourHandSwing_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_WARNING], m_pScaleOffset[TYPE_ET1_WARNING], m_pRotationOffset[TYPE_ET1_WARNING], nullptr, &m_pDecal, false);
			Safe_AddRef(m_pDecal);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_SMOKE])
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc >= 0.3f)
			{
				if (nullptr != m_pDecal)
				{
					m_pDecal->Start_AlphaDeleate();
					Safe_Release(m_pDecal);
				}

				CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
				if (pOwnerTransform != nullptr)
				{
					m_fTimeAcc = 0.f;

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Charge_Smoke"),
						pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET2_SMOKE], m_pScaleOffset[TYPE_ET2_SMOKE], m_pRotationOffset[TYPE_ET2_SMOKE]);
					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Charge_Circle"),
						pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET2_SMOKE], m_pScaleOffset[TYPE_ET2_SMOKE], m_pRotationOffset[TYPE_ET2_SMOKE]);
				}
			}

			if (m_iOwnerFrame >= 110)
				m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Stellia_Skill_Charge::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Charge::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Charge::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Charge* CVfx_Stellia_Skill_Charge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Charge* pInstance = new CVfx_Stellia_Skill_Charge(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Charge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Charge::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Charge* pInstance = new CVfx_Stellia_Skill_Charge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Charge");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Charge::Free()
{
	__super::Free();

	if (nullptr != m_pDecal)
	{
		m_pDecal->Start_AlphaDeleate();
		Safe_Release(m_pDecal);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}