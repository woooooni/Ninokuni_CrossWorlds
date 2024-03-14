#include "stdafx.h"
#include "Vfx_Stellia_Skill_TripleLaser.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_TripleLaser::CVfx_Stellia_Skill_TripleLaser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_TripleLaser::CVfx_Stellia_Skill_TripleLaser(const CVfx_Stellia_Skill_TripleLaser& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_TRIPLELASER;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pFrameTriger[TYPE_ET1_D_WARNING_CIRCLE] = 0;
	m_pPositionOffset[TYPE_ET1_D_WARNING_CIRCLE] = _float3(0.f, 0.f, 0.8f);
	m_pScaleOffset[TYPE_ET1_D_WARNING_CIRCLE]    = _float3(15.f, 3.f, 15.f);
	m_pRotationOffset[TYPE_ET1_D_WARNING_CIRCLE] = _float3(0.f, 0.f, 0.f);

	{
		m_pFrameTriger[TYPE_ET2_E_MARBLE_MU] = 13;
		m_pPositionOffset[TYPE_ET2_E_MARBLE_MU] = _float3(0.f, 1.2f, 0.2f);
		m_pScaleOffset[TYPE_ET2_E_MARBLE_MU]    = _float3(7.f, 7.f, 7.f);
		m_pRotationOffset[TYPE_ET2_E_MARBLE_MU] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_MARBLE_LINE_MU] = 18;
		m_pPositionOffset[TYPE_ET2_E_MARBLE_LINE_MU] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_MARBLE_LINE_MU]    = _float3(1.2f, 1.2f, 1.2f);
		m_pRotationOffset[TYPE_ET2_E_MARBLE_LINE_MU] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_E_MARBLE_LD] = 38;
		m_pPositionOffset[TYPE_ET2_E_MARBLE_LD] = _float3(-0.6f, 0.6f, 0.2f);
		m_pScaleOffset[TYPE_ET2_E_MARBLE_LD]    = _float3(7.f, 7.f, 7.f);
		m_pRotationOffset[TYPE_ET2_E_MARBLE_LD] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_MARBLE_LINE_LD] = 43;
		m_pPositionOffset[TYPE_ET2_E_MARBLE_LINE_LD] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_MARBLE_LINE_LD]    = _float3(1.2f, 1.2f, 1.2f);
		m_pRotationOffset[TYPE_ET2_E_MARBLE_LINE_LD] = _float3(0.f, 0.f, 0.f);


		m_pFrameTriger[TYPE_ET2_E_MARBLE_RD] = 59;
		m_pPositionOffset[TYPE_ET2_E_MARBLE_RD] = _float3(0.6f, 0.6f, 0.2f);
		m_pScaleOffset[TYPE_ET2_E_MARBLE_RD]    = _float3(7.f, 7.f, 7.f);
		m_pRotationOffset[TYPE_ET2_E_MARBLE_RD] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_MARBLEE_LINE_RD] = 64;
		m_pPositionOffset[TYPE_ET2_E_MARBLEE_LINE_RD] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_MARBLEE_LINE_RD]    = _float3(1.2f, 1.2f, 1.2f);
		m_pRotationOffset[TYPE_ET2_E_MARBLEE_LINE_RD] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET3_E_ELECT] = 90;
		m_pPositionOffset[TYPE_ET3_E_ELECT] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET3_E_ELECT]    = _float3(15.f, 15.f, 50.f);
		m_pRotationOffset[TYPE_ET3_E_ELECT] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_EVENT_DELETE] = 100;

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_TripleLaser::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_WARNING_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_WARNING_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_JumpDown_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_WARNING_CIRCLE], m_pScaleOffset[TYPE_ET1_D_WARNING_CIRCLE], m_pRotationOffset[TYPE_ET1_D_WARNING_CIRCLE], nullptr, &m_pWarningDecal, false);
			if (nullptr != m_pWarningDecal)
			{
				Safe_AddRef(m_pWarningDecal);
				CTransform* pTransform = m_pWarningDecal->Get_Component<CTransform>(TEXT("Com_Transform"));
				m_vTargetPos = pTransform->Get_Position();
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_MARBLE_MU && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MARBLE_MU])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_Marble"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MARBLE_MU], m_pScaleOffset[TYPE_ET2_E_MARBLE_MU], m_pRotationOffset[TYPE_ET2_E_MARBLE_MU], nullptr, &m_pMarble_MU, false);
			if (nullptr != m_pMarble_MU)
			{
				Safe_AddRef(m_pMarble_MU);
				m_pMarble_MU->Set_ScaleMinDelete(false);
				m_pMarble_MU->Set_ScaleMinSize(_float3(4.f, 4.f, 4.f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_MARBLE_LINE_MU && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MARBLE_LINE_MU])
		{
		    if (nullptr != m_pMarble_MU)
		    {
				CTransform* pTransform = m_pMarble_MU->Get_Component<CTransform>(TEXT("Com_Transform"));
				if (nullptr != pTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_Smoke_In"),
						pTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET2_E_MARBLE_LINE_MU], m_pScaleOffset[TYPE_ET2_E_MARBLE_LINE_MU], m_pRotationOffset[TYPE_ET2_E_MARBLE_LINE_MU], nullptr, &m_pMarble_Line_MU, false);
					Safe_AddRef(m_pMarble_Line_MU);

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_TripleLaser_Electricity"),
						pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pElect_MU, false);
					Safe_AddRef(m_pElect_MU);
				}
		    }
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_MARBLE_LD && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MARBLE_LD])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_Marble"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MARBLE_LD], m_pScaleOffset[TYPE_ET2_E_MARBLE_LD], m_pRotationOffset[TYPE_ET2_E_MARBLE_LD], nullptr, &m_pMarble_LD, false);
			if (nullptr != m_pMarble_LD)
			{
				Safe_AddRef(m_pMarble_LD);
				m_pMarble_LD->Set_ScaleMinDelete(false);
				m_pMarble_LD->Set_ScaleMinSize(_float3(4.f, 4.f, 4.f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_MARBLE_LINE_LD && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MARBLE_LINE_LD])
		{
			if (nullptr != m_pMarble_LD)
			{
				CTransform* pTransform = m_pMarble_LD->Get_Component<CTransform>(TEXT("Com_Transform"));
				if (nullptr != pTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_Smoke_In"),
						pTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET2_E_MARBLE_LINE_LD], m_pScaleOffset[TYPE_ET2_E_MARBLE_LINE_LD], m_pRotationOffset[TYPE_ET2_E_MARBLE_LINE_LD], nullptr, &m_pMarble_Line_LD, false);
					Safe_AddRef(m_pMarble_Line_LD);

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_TripleLaser_Electricity"),
						pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pElect_LD, false);
					Safe_AddRef(m_pElect_LD);
				}
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_MARBLE_RD && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MARBLE_RD])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_Marble"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MARBLE_RD], m_pScaleOffset[TYPE_ET2_E_MARBLE_RD], m_pRotationOffset[TYPE_ET2_E_MARBLE_RD], nullptr, &m_pMarble_RD, false);
			if (nullptr != m_pMarble_RD)
			{
				Safe_AddRef(m_pMarble_RD);
				m_pMarble_RD->Set_ScaleMinDelete(false);
				m_pMarble_RD->Set_ScaleMinSize(_float3(4.f, 4.f, 4.f));
			}
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_MARBLEE_LINE_RD && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MARBLEE_LINE_RD])
		{
			if (nullptr != m_pMarble_RD)
			{
				CTransform* pTransform = m_pMarble_RD->Get_Component<CTransform>(TEXT("Com_Transform"));
				if (nullptr != pTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_Smoke_In"),
						pTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET2_E_MARBLEE_LINE_RD], m_pScaleOffset[TYPE_ET2_E_MARBLEE_LINE_RD], m_pRotationOffset[TYPE_ET2_E_MARBLEE_LINE_RD], nullptr, &m_pMarble_Line_RD, false);
					Safe_AddRef(m_pMarble_Line_RD);

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_TripleLaser_Electricity"),
						pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pElect_RD, false);
					Safe_AddRef(m_pElect_RD);
				}
			}
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_E_ELECT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_E_ELECT])
		{
			if (nullptr != m_pMarble_MU)
			{
				CTransform* pTransform = m_pMarble_MU->Get_Component<CTransform>(TEXT("Com_Transform"));
				if (nullptr != pTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_LaserLine"),
						pTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET3_E_ELECT], m_pScaleOffset[TYPE_ET3_E_ELECT], m_pRotationOffset[TYPE_ET3_E_ELECT], nullptr, &m_pElectLine_MU, false);
					if (nullptr != m_pElectLine_MU)
					{
						Safe_AddRef(m_pElectLine_MU);
						CTransform* pTransform = m_pElectLine_MU->Get_Component<CTransform>(TEXT("Com_Transform"));
						if (nullptr != pTransform)
							pTransform->LookAt(m_vTargetPos);
					}

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_TripleLaser_Circles"),
						pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f,1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pCircles_MU, false);
					if (nullptr != m_pCircles_MU)
					{
						Safe_AddRef(m_pCircles_MU);
						CTransform* pTransform = m_pCircles_MU->Get_Component<CTransform>(TEXT("Com_Transform"));
						if (nullptr != pTransform)
							pTransform->LookAt(m_vTargetPos);
					}
				}
				
				m_pMarble_MU->Set_ScaleMinDelete(true);
				m_pMarble_MU->Set_ScaleMinSize(_float3(0.1f, 0.1f, 0.1f));
				Safe_Release(m_pMarble_MU);
			}
			if (nullptr != m_pMarble_LD)
			{
				CTransform* pTransform = m_pMarble_LD->Get_Component<CTransform>(TEXT("Com_Transform"));
				if (nullptr != pTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_LaserLine"),
						pTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET3_E_ELECT], m_pScaleOffset[TYPE_ET3_E_ELECT], m_pRotationOffset[TYPE_ET3_E_ELECT], nullptr, &m_pElectLine_LD, false);
					if (nullptr != m_pElectLine_LD)
					{
						Safe_AddRef(m_pElectLine_LD);
						CTransform* pTransform = m_pElectLine_LD->Get_Component<CTransform>(TEXT("Com_Transform"));
						if (nullptr != pTransform)
							pTransform->LookAt(m_vTargetPos);
					}

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_TripleLaser_Circles"),
						pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pCircles_LD, false);
					if (nullptr != m_pCircles_LD)
					{
						Safe_AddRef(m_pCircles_LD);
						CTransform* pTransform = m_pCircles_LD->Get_Component<CTransform>(TEXT("Com_Transform"));
						if (nullptr != pTransform)
							pTransform->LookAt(m_vTargetPos);
					}
				}

				m_pMarble_LD->Set_ScaleMinDelete(true);
				m_pMarble_LD->Set_ScaleMinSize(_float3(0.1f, 0.1f, 0.1f));
				Safe_Release(m_pMarble_LD);
			}
			if (nullptr != m_pMarble_RD)
			{
				CTransform* pTransform = m_pMarble_RD->Get_Component<CTransform>(TEXT("Com_Transform"));
				if (nullptr != pTransform)
				{
					GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_TripleLaser_LaserLine"),
						pTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_ET3_E_ELECT], m_pScaleOffset[TYPE_ET3_E_ELECT], m_pRotationOffset[TYPE_ET3_E_ELECT], nullptr, &m_pElectLine_RD, false);
					if (nullptr != m_pElectLine_RD)
					{
						Safe_AddRef(m_pElectLine_RD);
						CTransform* pTransform = m_pElectLine_RD->Get_Component<CTransform>(TEXT("Com_Transform"));
						if (nullptr != pTransform)
							pTransform->LookAt(m_vTargetPos);
					}

					GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_TripleLaser_Circles"),
						pTransform->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pCircles_RD, false);
					if (nullptr != m_pCircles_RD)
					{
						Safe_AddRef(m_pCircles_RD);
						CTransform* pTransform = m_pCircles_RD->Get_Component<CTransform>(TEXT("Com_Transform"));
						if (nullptr != pTransform)
							pTransform->LookAt(m_vTargetPos);
					}
				}

				m_pMarble_RD->Set_ScaleMinDelete(true);
				m_pMarble_RD->Set_ScaleMinSize(_float3(0.1f, 0.1f, 0.1f));
				Safe_Release(m_pMarble_RD);
			}

			if (nullptr != m_pMarble_Line_MU)
			{
				m_pMarble_Line_MU->Set_UVLoop(1);
				Safe_Release(m_pMarble_Line_MU);
			}
			if (nullptr != m_pMarble_Line_LD)
			{
				m_pMarble_Line_LD->Set_UVLoop(1);
				Safe_Release(m_pMarble_Line_LD);
			}
			if (nullptr != m_pMarble_Line_RD)
			{
				m_pMarble_Line_RD->Set_UVLoop(1);
				Safe_Release(m_pMarble_Line_RD);
			}

			if (nullptr != m_pElect_MU)
			{
				m_pElect_MU->Set_Dead(true);
				Safe_Release(m_pElect_MU);
			}
			if (nullptr != m_pElect_LD)
			{
				m_pElect_LD->Set_Dead(true);
				Safe_Release(m_pElect_LD);
			}
			if (nullptr != m_pElect_RD)
			{
				m_pElect_RD->Set_Dead(true);
				Safe_Release(m_pElect_RD);
			}

			m_iCount++;
		}

		else if (m_iCount == TYPE_EVENT_DELETE && m_iOwnerFrame >= m_pFrameTriger[TYPE_EVENT_DELETE])
		{
		    if (nullptr != m_pCircles_MU)
		    {
		    	m_pCircles_MU->Set_LoopParticle(false);
		    	Safe_Release(m_pCircles_MU);
		    }
		    if (nullptr != m_pCircles_LD)
		    {
		    	m_pCircles_LD->Set_LoopParticle(false);
		    	Safe_Release(m_pCircles_LD);
		    }
		    if (nullptr != m_pCircles_RD)
		    {
		    	m_pCircles_RD->Set_LoopParticle(false);
		    	Safe_Release(m_pCircles_RD);
		    }
		}
	}
}

void CVfx_Stellia_Skill_TripleLaser::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_TripleLaser::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_TripleLaser* CVfx_Stellia_Skill_TripleLaser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_TripleLaser* pInstance = new CVfx_Stellia_Skill_TripleLaser(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_TripleLaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_TripleLaser::Clone(void* pArg)
{
	CVfx_Stellia_Skill_TripleLaser* pInstance = new CVfx_Stellia_Skill_TripleLaser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_TripleLaser");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_TripleLaser::Free()
{
	__super::Free();

	if (nullptr != m_pWarningDecal)
	{
		m_pWarningDecal->Start_AlphaDeleate();
		Safe_Release(m_pWarningDecal);
	}

	if (nullptr != m_pMarble_MU)
	{
		m_pMarble_MU->Set_ScaleMinDelete(true);
		m_pMarble_MU->Set_ScaleMinSize(_float3(0.1f, 0.1f, 0.1f));
		Safe_Release(m_pMarble_MU);
	}
	if (nullptr != m_pMarble_LD)
	{
		m_pMarble_LD->Set_ScaleMinDelete(true);
		m_pMarble_LD->Set_ScaleMinSize(_float3(0.1f, 0.1f, 0.1f));
		Safe_Release(m_pMarble_LD);
	}
	if (nullptr != m_pMarble_RD)
	{
		m_pMarble_RD->Set_ScaleMinDelete(true);
		m_pMarble_RD->Set_ScaleMinSize(_float3(0.1f, 0.1f, 0.1f));
		Safe_Release(m_pMarble_RD);
	}

	if (nullptr != m_pMarble_Line_MU)
	{
		m_pMarble_Line_MU->Set_UVLoop(1);
		Safe_Release(m_pMarble_Line_MU);
	}
	if (nullptr != m_pMarble_Line_LD)
	{
		m_pMarble_Line_LD->Set_UVLoop(1);
		Safe_Release(m_pMarble_Line_LD);
	}
	if (nullptr != m_pMarble_Line_RD)
	{
		m_pMarble_Line_RD->Set_UVLoop(1);
		Safe_Release(m_pMarble_Line_RD);
	}

	if (nullptr != m_pElect_MU)
	{
		m_pElect_MU->Set_Dead(true);
		Safe_Release(m_pElect_MU);
	}
	if (nullptr != m_pElect_LD)
	{
		m_pElect_LD->Set_Dead(true);
		Safe_Release(m_pElect_LD);
	}
	if (nullptr != m_pElect_RD)
	{
		m_pElect_RD->Set_Dead(true);
		Safe_Release(m_pElect_RD);
	}

	if (nullptr != m_pElectLine_MU)
	{
		m_pElectLine_MU->Set_Dead(true);
		Safe_Release(m_pElectLine_MU);
	}
	if (nullptr != m_pElectLine_LD)
	{
		m_pElectLine_LD->Set_Dead(true);
		Safe_Release(m_pElectLine_LD);
	}
	if (nullptr != m_pElectLine_RD)
	{
		m_pElectLine_RD->Set_Dead(true);
		Safe_Release(m_pElectLine_RD);
	}

	if (nullptr != m_pCircles_MU)
	{
		m_pCircles_MU->Set_LoopParticle(false);
		Safe_Release(m_pCircles_MU);
	}
	if (nullptr != m_pCircles_LD)
	{
		m_pCircles_LD->Set_LoopParticle(false);
		Safe_Release(m_pCircles_LD);
	}
	if (nullptr != m_pCircles_RD)
	{
		m_pCircles_RD->Set_LoopParticle(false);
		Safe_Release(m_pCircles_RD);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}