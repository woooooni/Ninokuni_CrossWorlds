#include "stdafx.h"
#include "Vfx_Glanix_Intro_Roar.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Glanix.h"

CVfx_Glanix_Intro_Roar::CVfx_Glanix_Intro_Roar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Glanix_Intro_Roar::CVfx_Glanix_Intro_Roar(const CVfx_Glanix_Intro_Roar& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Glanix_Intro_Roar::Initialize_Prototype()
{
	//m_bOwnerStateIndex = CGlanix::GLANIX_INTRO_ROAR;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];
	
	// 26 ¿Þ
	{
		m_pFrameTriger[TYPE_ET1_P_SPARKLE]    = 26;
		m_pPositionOffset[TYPE_ET1_P_SPARKLE] = _float3(-0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET1_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_SMOKE]    = 26;
		m_pPositionOffset[TYPE_ET1_P_SMOKE] = _float3(-0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET1_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 34 ¿À
	{
		m_pFrameTriger[TYPE_ET2_P_SPARKLE]    = 34;
		m_pPositionOffset[TYPE_ET2_P_SPARKLE] = _float3(0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET2_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_SMOKE]    = 34;
		m_pPositionOffset[TYPE_ET2_P_SMOKE] = _float3(0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET2_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 39 ¿Þ
	{
		m_pFrameTriger[TYPE_ET3_P_SPARKLE]    = 39;
		m_pPositionOffset[TYPE_ET3_P_SPARKLE] = _float3(-0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET3_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET3_P_SMOKE]    = 39;
		m_pPositionOffset[TYPE_ET3_P_SMOKE] = _float3(-0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET3_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET3_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 44 ¿À
	{
		m_pFrameTriger[TYPE_ET4_P_SPARKLE]    = 44;
		m_pPositionOffset[TYPE_ET4_P_SPARKLE] = _float3(0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET4_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET4_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET4_P_SMOKE]    = 44;
		m_pPositionOffset[TYPE_ET4_P_SMOKE] = _float3(0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET4_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET4_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 48 ¿Þ
	{
		m_pFrameTriger[TYPE_ET5_P_SPARKLE]    = 48;
		m_pPositionOffset[TYPE_ET5_P_SPARKLE] = _float3(-0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET5_P_SPARKLE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET5_P_SPARKLE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET5_P_SMOKE]    = 48;
		m_pPositionOffset[TYPE_ET5_P_SMOKE] = _float3(-0.5f, 3.f, 1.f);
		m_pScaleOffset[TYPE_ET5_P_SMOKE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET5_P_SMOKE] = _float3(0.f, 0.f, 0.f);
	}

	// 70 Æ÷È¿ ½ÃÀÛ
	{
		m_pFrameTriger[TYPE_ET6_E_ROAR]    = 70;
		m_pPositionOffset[TYPE_ET6_E_ROAR] = _float3(0.f, 25.f, 0.f);
		m_pScaleOffset[TYPE_ET6_E_ROAR]    = _float3(0.2f, 0.2f, 0.2f);
		m_pRotationOffset[TYPE_ET6_E_ROAR] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Glanix_Intro_Roar::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Glanix_Intro_Roar::Tick(_float fTimeDelta)
{
	if (m_bFinish || m_pOwnerObject == nullptr || m_pOwnerObject->Is_ReserveDead() || m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	CModel* pModel = m_pOwnerObject->Get_Component<CModel>(L"Com_Model");
	if (pModel == nullptr)
	{
		Set_Dead(true);
		return;
	}

	if (m_bOwnerTween) // false == TweenFinish
		m_bOwnerTween = pModel->Is_Tween();
	else
		m_iOwnerFrame = pModel->Get_CurrAnimationFrame();

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SPARKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_SPARKLE], m_pScaleOffset[TYPE_ET1_P_SPARKLE], m_pRotationOffset[TYPE_ET1_P_SPARKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_SMOKE], m_pScaleOffset[TYPE_ET1_P_SMOKE], m_pRotationOffset[TYPE_ET1_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SPARKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_SPARKLE], m_pScaleOffset[TYPE_ET2_P_SPARKLE], m_pRotationOffset[TYPE_ET2_P_SPARKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_SMOKE], m_pScaleOffset[TYPE_ET2_P_SMOKE], m_pRotationOffset[TYPE_ET2_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_SPARKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_SPARKLE], m_pScaleOffset[TYPE_ET3_P_SPARKLE], m_pRotationOffset[TYPE_ET3_P_SPARKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET3_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET3_P_SMOKE], m_pScaleOffset[TYPE_ET3_P_SMOKE], m_pRotationOffset[TYPE_ET3_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET4_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET4_P_SPARKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET4_P_SPARKLE], m_pScaleOffset[TYPE_ET4_P_SPARKLE], m_pRotationOffset[TYPE_ET4_P_SPARKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET4_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET4_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET4_P_SMOKE], m_pScaleOffset[TYPE_ET4_P_SMOKE], m_pRotationOffset[TYPE_ET4_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET5_P_SPARKLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET5_P_SPARKLE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Sparkle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET5_P_SPARKLE], m_pScaleOffset[TYPE_ET5_P_SPARKLE], m_pRotationOffset[TYPE_ET5_P_SPARKLE]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET5_P_SMOKE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET5_P_SMOKE])
		{
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Glanix_Skill_FourHandSwing_Trail_Smoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET5_P_SMOKE], m_pScaleOffset[TYPE_ET5_P_SMOKE], m_pRotationOffset[TYPE_ET5_P_SMOKE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET6_E_ROAR && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET6_E_ROAR])
		{
			GET_INSTANCE(CEffect_Manager)->Tick_Generate_Effect(&fTimeAcc, 0.225f, fTimeDelta, TEXT("Effect_Glanix_Roar_TrailLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET6_E_ROAR], m_pScaleOffset[TYPE_ET6_E_ROAR], m_pRotationOffset[TYPE_ET6_E_ROAR]);

			if(m_iOwnerFrame >= m_pFrameTriger[TYPE_ET6_E_ROAR] + 25)
				m_iCount++;
		}

		else if (m_iCount == TYPE_END)
			m_bFinish = true;
	}
}

void CVfx_Glanix_Intro_Roar::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Glanix_Intro_Roar::Render()
{
	return S_OK;
}

HRESULT CVfx_Glanix_Intro_Roar::Ready_Components()
{
	return S_OK;
}

CVfx_Glanix_Intro_Roar* CVfx_Glanix_Intro_Roar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Glanix_Intro_Roar* pInstance = new CVfx_Glanix_Intro_Roar(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Glanix_Intro_Roar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Glanix_Intro_Roar::Clone(void* pArg)
{
	CVfx_Glanix_Intro_Roar* pInstance = new CVfx_Glanix_Intro_Roar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Glanix_Intro_Roar");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Glanix_Intro_Roar::Free()
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