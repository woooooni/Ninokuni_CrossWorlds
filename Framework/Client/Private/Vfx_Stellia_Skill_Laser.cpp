#include "stdafx.h"
#include "Vfx_Stellia_Skill_Laser.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Decal.h"
#include "Effect.h"
#include "Particle.h"

#include "Stellia.h"

CVfx_Stellia_Skill_Laser::CVfx_Stellia_Skill_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_Laser::CVfx_Stellia_Skill_Laser(const CVfx_Stellia_Skill_Laser& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_Laser::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_LASER;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	m_pFrameTriger[TYPE_ET1_D_WARNING] = 0;
	m_pPositionOffset[TYPE_ET1_D_WARNING] = _float3(0.f, 0.f, 0.6f);
	m_pScaleOffset[TYPE_ET1_D_WARNING]    = _float3(7.f, 2.f, 42.f);
	m_pRotationOffset[TYPE_ET1_D_WARNING] = _float3(0.f, 0.f, 0.f);

	{
		m_pFrameTriger[TYPE_ET2_E_MAINLINE_01] = 33;
		m_pPositionOffset[TYPE_ET2_E_MAINLINE_01] = _float3(0.f, 1.f, -1.5f);
		m_pScaleOffset[TYPE_ET2_E_MAINLINE_01]    = _float3(8.f, 30.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_MAINLINE_01] = _float3(90.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_MAINLINE_02] = 33;
		m_pPositionOffset[TYPE_ET2_E_MAINLINE_02] = _float3(0.2f, 1.f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_MAINLINE_02]    = _float3(8.f, 30.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_MAINLINE_02] = _float3(0.f, 90.f, 90.f);
	}

	{
		// ©Л
		m_pFrameTriger[TYPE_ET2_E_LINEEFFECT_01] = 33;
		m_pPositionOffset[TYPE_ET2_E_LINEEFFECT_01] = _float3(-0.5f, 1.5f, -1.7f);
		m_pScaleOffset[TYPE_ET2_E_LINEEFFECT_01]    = _float3(4.f, 8.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_LINEEFFECT_01] = _float3(90.f, 20.f, 0.f);

		// аб
		m_pFrameTriger[TYPE_ET2_E_LINEEFFECT_02] = 33;
		m_pPositionOffset[TYPE_ET2_E_LINEEFFECT_02] = _float3(-0.5f, 1.7f, 1.5f);
		m_pScaleOffset[TYPE_ET2_E_LINEEFFECT_02]    = _float3(4.f, 8.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_LINEEFFECT_02] = _float3(-90.f, 180.f, -20.f);

		// ╩С
		m_pFrameTriger[TYPE_ET2_E_LINEEFFECT_03] = 33;
		m_pPositionOffset[TYPE_ET2_E_LINEEFFECT_03] = _float3(0.f, 1.7f, 0.f);
		m_pScaleOffset[TYPE_ET2_E_LINEEFFECT_03]    = _float3(4.f, 8.f, 1.f);
		m_pRotationOffset[TYPE_ET2_E_LINEEFFECT_03] = _float3(0.f, -90.f, -75.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 33;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 1.8f, 7.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] = 33;
		m_pPositionOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 4.f, 12.f);
		m_pScaleOffset[TYPE_ET2_E_CIRCLELINE]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET2_E_CIRCLELINE] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_ET3_V_FINISH] = 100;
	m_pPositionOffset[TYPE_ET3_V_FINISH] = _float3(0.f, 0.f, 0.f);
	m_pScaleOffset[TYPE_ET3_V_FINISH]    = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_ET3_V_FINISH] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Laser::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_Laser::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_WARNING && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_WARNING])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Glanix_Skill_FourHandSwing_Warning"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_WARNING], m_pScaleOffset[TYPE_ET1_D_WARNING], m_pRotationOffset[TYPE_ET1_D_WARNING]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_MAINLINE_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_MAINLINE_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MAINLINE_01], m_pScaleOffset[TYPE_ET2_E_MAINLINE_01], m_pRotationOffset[TYPE_ET2_E_MAINLINE_01], false, &m_pMainLine_01, false);
			Safe_AddRef(m_pMainLine_01);

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MAINLINE_01], m_pScaleOffset[TYPE_ET2_E_MAINLINE_01], m_pRotationOffset[TYPE_ET2_E_MAINLINE_01], false, &m_pMainLine_02, false);
			Safe_AddRef(m_pMainLine_02);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MAINLINE_02], m_pScaleOffset[TYPE_ET2_E_MAINLINE_02], m_pRotationOffset[TYPE_ET2_E_MAINLINE_02], false, &m_pMainLine_03, false);
			Safe_AddRef(m_pMainLine_03);

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_MAINLINE_02], m_pScaleOffset[TYPE_ET2_E_MAINLINE_02], m_pRotationOffset[TYPE_ET2_E_MAINLINE_02], false, &m_pMainLine_04, false);
			Safe_AddRef(m_pMainLine_04);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_E_LINEEFFECT_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_LINEEFFECT_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect_Add"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_LINEEFFECT_01], m_pScaleOffset[TYPE_ET2_E_LINEEFFECT_01], m_pRotationOffset[TYPE_ET2_E_LINEEFFECT_01], nullptr, &m_pMainLineEffect_01, false);
			Safe_AddRef(m_pMainLineEffect_01);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect_Add"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_LINEEFFECT_02], m_pScaleOffset[TYPE_ET2_E_LINEEFFECT_02], m_pRotationOffset[TYPE_ET2_E_LINEEFFECT_02], nullptr, &m_pMainLineEffect_02, false);
			Safe_AddRef(m_pMainLineEffect_02);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_LaserLineEffect_Add"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_LINEEFFECT_03], m_pScaleOffset[TYPE_ET2_E_LINEEFFECT_03], m_pRotationOffset[TYPE_ET2_E_LINEEFFECT_03], nullptr, &m_pMainLineEffect_03, false);
			Safe_AddRef(m_pMainLineEffect_03);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Laser_Circles"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES], nullptr, &m_pCircleParticle_01, false);
			Safe_AddRef(m_pCircleParticle_01);

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Stellia_Skill_Laser_Circles_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES], nullptr, &m_pCircleParticle_02, false);
			Safe_AddRef(m_pCircleParticle_02);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_CIRCLELINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE])
		{
			m_fTimeAcc += fTimeDelta;
			if (m_fTimeAcc >= 0.2f)
			{
				m_fTimeAcc = 0.f;

				GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_CircleLineEffect"),
					XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_CIRCLELINE], m_pScaleOffset[TYPE_ET2_E_CIRCLELINE], m_pRotationOffset[TYPE_ET2_E_CIRCLELINE]);
			}

			if (m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_CIRCLELINE] + 20)
				m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_V_FINISH && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_V_FINISH])
		{
			if (nullptr != m_pMainLine_01)
			{
				m_pMainLine_01->Set_AnimationLoop(false);
				m_pMainLine_01->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLine_01);
			}
			if (nullptr != m_pMainLine_02)
			{
				m_pMainLine_02->Set_AnimationLoop(false);
				m_pMainLine_02->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLine_02);
			}
			if (nullptr != m_pMainLine_03)
			{
				m_pMainLine_03->Set_AnimationLoop(false);
				m_pMainLine_03->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLine_03);
			}
			if (nullptr != m_pMainLine_04)
			{
				m_pMainLine_04->Set_AnimationLoop(false);
				m_pMainLine_04->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLine_04);
			}

			if (nullptr != m_pMainLineEffect_01)
			{
				m_pMainLineEffect_01->Set_AnimationLoop(false);
				m_pMainLineEffect_01->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLineEffect_01);
			}
			if (nullptr != m_pMainLineEffect_02)
			{
				m_pMainLineEffect_02->Set_AnimationLoop(false);
				m_pMainLineEffect_02->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLineEffect_02);
			}
			if (nullptr != m_pMainLineEffect_03)
			{
				m_pMainLineEffect_03->Set_AnimationLoop(false);
				m_pMainLineEffect_03->Set_AnimationFinishDelete(true);
				Safe_Release(m_pMainLineEffect_03);
			}

			if (nullptr != m_pCircleParticle_01)
			{
				m_pCircleParticle_01->Set_LoopParticle(false);
				Safe_Release(m_pCircleParticle_01);
			}
			if (nullptr != m_pCircleParticle_02)
			{
				m_pCircleParticle_02->Set_LoopParticle(false);
				Safe_Release(m_pCircleParticle_02);
			}

			m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Stellia_Skill_Laser::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_Laser::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_Laser::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_Laser* CVfx_Stellia_Skill_Laser::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_Laser* pInstance = new CVfx_Stellia_Skill_Laser(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_Laser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_Laser::Clone(void* pArg)
{
	CVfx_Stellia_Skill_Laser* pInstance = new CVfx_Stellia_Skill_Laser(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_Laser");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_Laser::Free()
{
	__super::Free();

	if (nullptr != m_pMainLine_01)
	{
		m_pMainLine_01->Set_AnimationLoop(false);
		m_pMainLine_01->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLine_01);
	}
	if (nullptr != m_pMainLine_02)
	{
		m_pMainLine_02->Set_AnimationLoop(false);
		m_pMainLine_02->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLine_02);
	}
	if (nullptr != m_pMainLine_03)
	{
		m_pMainLine_03->Set_AnimationLoop(false);
		m_pMainLine_03->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLine_03);
	}
	if (nullptr != m_pMainLine_04)
	{
		m_pMainLine_04->Set_AnimationLoop(false);
		m_pMainLine_04->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLine_04);
	}

	if (nullptr != m_pMainLineEffect_01)
	{
		m_pMainLineEffect_01->Set_AnimationLoop(false);
		m_pMainLineEffect_01->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLineEffect_01);
	}
	if (nullptr != m_pMainLineEffect_02)
	{
		m_pMainLineEffect_02->Set_AnimationLoop(false);
		m_pMainLineEffect_02->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLineEffect_02);
	}
	if (nullptr != m_pMainLineEffect_03)
	{
		m_pMainLineEffect_03->Set_AnimationLoop(false);
		m_pMainLineEffect_03->Set_AnimationFinishDelete(true);
		Safe_Release(m_pMainLineEffect_03);
	}

	if (nullptr != m_pCircleParticle_01)
	{
		m_pCircleParticle_01->Set_LoopParticle(false);
		Safe_Release(m_pCircleParticle_01);
	}
	if (nullptr != m_pCircleParticle_02)
	{
		m_pCircleParticle_02->Set_LoopParticle(false);
		Safe_Release(m_pCircleParticle_02);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}