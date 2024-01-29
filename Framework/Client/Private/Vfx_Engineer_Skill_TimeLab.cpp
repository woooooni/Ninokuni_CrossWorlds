#include "stdafx.h"
#include "Vfx_Engineer_Skill_TimeLab.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"
#include "Effect.h"

CVfx_Engineer_Skill_TimeLab::CVfx_Engineer_Skill_TimeLab(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Engineer_Skill_TimeLab::CVfx_Engineer_Skill_TimeLab(const CVfx_Engineer_Skill_TimeLab& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Engineer_Skill_TimeLab::Initialize_Prototype()
{
	m_bOwnerStateIndex = CCharacter::SKILL_SPECIAL_1;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_ET1_D_CIRCLE] = 0;
		m_pPositionOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_D_CIRCLE]    = _float3(10.f, 2.f, 10.f);
		m_pRotationOffset[TYPE_ET1_D_CIRCLE] = _float3(0.f, 0.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET1_E_TIME_BIG_FRAME] = 12;
		m_pPositionOffset[TYPE_ET1_E_TIME_BIG_FRAME] = _float3(0.f, 0.3f, 0.1f);
		m_pScaleOffset[TYPE_ET1_E_TIME_BIG_FRAME]    = _float3(4.5f, 4.5f, 4.5f);
		m_pRotationOffset[TYPE_ET1_E_TIME_BIG_FRAME] = _float3(0.f, 180.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET1_P_LIGHT] = 12;
		m_pPositionOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 1.2f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_LIGHT]    = _float3(0.5f, 0.5f, 0.5f);
		m_pRotationOffset[TYPE_ET1_P_LIGHT] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET1_P_CIRCLES] = 12;
		m_pPositionOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET1_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET1_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}


	{
		m_pFrameTriger[TYPE_ET2_E_TIME_SMALL_FRAME] = 28;
		m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_FRAME] = _float3(0.f, 0.8f, 0.1f);
		m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_FRAME]    = _float3(2.5f, 2.5f, 2.5f);
		m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_FRAME] = _float3(0.f, 180.f, 0.f);


		m_pFrameTriger[TYPE_ET2_E_TIME_SMALL_GEAR01] = 28;
		m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_GEAR01] = _float3(-0.12f, 1.12f, 0.15f);
		m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_GEAR01]    = _float3(2.f, 2.f, 2.f);
		m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_GEAR01] = _float3(0.f, 180.f, 0.f);

		m_pFrameTriger[TYPE_ET2_E_TIME_SMALL_GEAR02] = 28;
		m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_GEAR02] = _float3(0.11f, 0.85f, 0.15f);
		m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_GEAR02]    = _float3(2.f, 2.f, 2.f);
		m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_GEAR02] = _float3(0.f, 180.f, 0.f);
	}

	{
		m_pFrameTriger[TYPE_ET2_P_LIGHTLINE] = 28;
		m_pPositionOffset[TYPE_ET2_P_LIGHTLINE] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_LIGHTLINE]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_LIGHTLINE] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_ET2_P_CIRCLES] = 28;
		m_pPositionOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 2.f, 0.f);
		m_pScaleOffset[TYPE_ET2_P_CIRCLES]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_ET2_P_CIRCLES] = _float3(0.f, 0.f, 0.f);
	}

	m_pFrameTriger[TYPE_ET3_EVENT_DELETE] = 40;
	m_pPositionOffset[TYPE_ET3_EVENT_DELETE] = _float3(1.f, 0.5f, 0.1f); // BigGear Color
	m_pScaleOffset[TYPE_ET3_EVENT_DELETE]    = _float3(1.f, 0.6f, 0.3f); // SmallGear Color
	m_pRotationOffset[TYPE_ET3_EVENT_DELETE] = _float3(0.f, 0.f, 0.f);

 	return S_OK;
}

HRESULT CVfx_Engineer_Skill_TimeLab::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Engineer_Skill_TimeLab::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_ET1_D_CIRCLE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_D_CIRCLE])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_D_CIRCLE], m_pScaleOffset[TYPE_ET1_D_CIRCLE], m_pRotationOffset[TYPE_ET1_D_CIRCLE]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET1_E_TIME_BIG_FRAME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_E_TIME_BIG_FRAME])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_BigGear_Frame"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_TIME_BIG_FRAME], m_pScaleOffset[TYPE_ET1_E_TIME_BIG_FRAME], m_pRotationOffset[TYPE_ET1_E_TIME_BIG_FRAME], nullptr, &m_pBigGear_Frame, false);
			Safe_AddRef(m_pBigGear_Frame);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_BigGear_Minutehand"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_TIME_BIG_FRAME], m_pScaleOffset[TYPE_ET1_E_TIME_BIG_FRAME], m_pRotationOffset[TYPE_ET1_E_TIME_BIG_FRAME], nullptr, &m_pBigGear_Minut, false);
			Safe_AddRef(m_pBigGear_Minut);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_BigGear_Secondhand"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_E_TIME_BIG_FRAME], m_pScaleOffset[TYPE_ET1_E_TIME_BIG_FRAME], m_pRotationOffset[TYPE_ET1_E_TIME_BIG_FRAME], nullptr, &m_pBigGear_Second, false);
			Safe_AddRef(m_pBigGear_Second);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_LIGHT && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_LIGHT])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_CircleLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_LIGHT], m_pScaleOffset[TYPE_ET1_P_LIGHT], m_pRotationOffset[TYPE_ET1_P_LIGHT]);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET1_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET1_P_CIRCLES])
		{
			if (nullptr != m_pBigGear_Frame)
			{
				m_pBigGear_Frame->Start_Dissolve(73, // Index
					_float4(m_pPositionOffset[TYPE_ET3_EVENT_DELETE].x, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].y, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pBigGear_Frame);
			}

			if (nullptr != m_pBigGear_Minut)
			{
				m_pBigGear_Minut->Start_Dissolve(73, // Index
					_float4(m_pPositionOffset[TYPE_ET3_EVENT_DELETE].x, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].y, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pBigGear_Minut);
			}

			if (nullptr != m_pBigGear_Second)
			{
				m_pBigGear_Second->Start_Dissolve(73, // Index
					_float4(m_pPositionOffset[TYPE_ET3_EVENT_DELETE].x, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].y, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
					5.f,   // Speed
					10.f); // Total
				Safe_Release(m_pBigGear_Second);
			}

			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_TimeLab_Circles_Start"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET1_P_CIRCLES], m_pScaleOffset[TYPE_ET1_P_CIRCLES], m_pRotationOffset[TYPE_ET1_P_CIRCLES]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET2_E_TIME_SMALL_FRAME && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_E_TIME_SMALL_FRAME])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_Frame"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_FRAME], nullptr, &m_pSmallGear_Frame, false);
			Safe_AddRef(m_pSmallGear_Frame);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_FrameLine"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_FRAME], nullptr, &m_pSmallGear_FrameLine, false);
			Safe_AddRef(m_pSmallGear_FrameLine);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_GearIn"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_FRAME], nullptr, &m_pSmallGear_GearIn, false);
			Safe_AddRef(m_pSmallGear_GearIn);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_Minutehand"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_FRAME], nullptr, &m_pSmallGear_Minut, false);
			Safe_AddRef(m_pSmallGear_Minut);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_Secondhand"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_FRAME], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_FRAME], nullptr, &m_pSmallGear_Second, false);
			Safe_AddRef(m_pSmallGear_Second);
			m_iCount++;


			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_Gear_01"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_GEAR01], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_GEAR01], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_GEAR01], nullptr, &m_pSmallGear_Gear01, false);
			Safe_AddRef(m_pSmallGear_Gear01);
			m_iCount++;

			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Engineer_Skill_TimeLab_SmallGear_Gear_02"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_E_TIME_SMALL_GEAR02], m_pScaleOffset[TYPE_ET2_E_TIME_SMALL_GEAR02], m_pRotationOffset[TYPE_ET2_E_TIME_SMALL_GEAR02], nullptr, &m_pSmallGear_Gear02, false);
			Safe_AddRef(m_pSmallGear_Gear02);
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_LIGHTLINE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_LIGHTLINE])
		{
			m_iCount++;
		}
		else if (m_iCount == TYPE_ET2_P_CIRCLES && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET2_P_CIRCLES])
		{
		    GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Engineer_Skill_TimeLab_Circles_End"),
			    XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_ET2_P_CIRCLES], m_pScaleOffset[TYPE_ET2_P_CIRCLES], m_pRotationOffset[TYPE_ET2_P_CIRCLES]);
			m_iCount++;
		}

		else if (m_iCount == TYPE_ET3_EVENT_DELETE && m_iOwnerFrame >= m_pFrameTriger[TYPE_ET3_EVENT_DELETE])
		{
		    if (nullptr != m_pSmallGear_Frame)
		    {
		    	m_pSmallGear_Frame->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_Frame);
		    }
		    
		    if (nullptr != m_pSmallGear_FrameLine)
		    {
		    	m_pSmallGear_FrameLine->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_FrameLine);
		    }
		    
		    if (nullptr != m_pSmallGear_GearIn)
		    {
		    	m_pSmallGear_GearIn->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_GearIn);
		    }
		    
		    if (nullptr != m_pSmallGear_Minut)
		    {
		    	m_pSmallGear_Minut->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_Minut);
		    }
		    
		    if (nullptr != m_pSmallGear_Second)
		    {
		    	m_pSmallGear_Second->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_Second);
		    }
		    
		    if (nullptr != m_pSmallGear_Gear01)
		    {
		    	m_pSmallGear_Gear01->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_Gear01);
		    }
		    
		    if (nullptr != m_pSmallGear_Gear02)
		    {
		    	m_pSmallGear_Gear02->Start_Dissolve(73, // Index
		    		_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
		    		5.f,   // Speed
		    		10.f); // Total
		    	Safe_Release(m_pSmallGear_Gear02);
		    }
		    
		    m_iCount++;
		}
		else if (m_iCount == TYPE_END)
		{
		    m_bFinish = true;
        }
	}
}

void CVfx_Engineer_Skill_TimeLab::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Engineer_Skill_TimeLab::Render()
{
	return S_OK;
}

HRESULT CVfx_Engineer_Skill_TimeLab::Ready_Components()
{
	return S_OK;
}

CVfx_Engineer_Skill_TimeLab* CVfx_Engineer_Skill_TimeLab::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Engineer_Skill_TimeLab* pInstance = new CVfx_Engineer_Skill_TimeLab(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Engineer_Skill_TimeLab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Engineer_Skill_TimeLab::Clone(void* pArg)
{
	CVfx_Engineer_Skill_TimeLab* pInstance = new CVfx_Engineer_Skill_TimeLab(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Engineer_Skill_TimeLab");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Engineer_Skill_TimeLab::Free()
{
	__super::Free();

	if (nullptr != m_pBigGear_Frame)
	{
		m_pBigGear_Frame->Start_Dissolve(73, // Index
			_float4(m_pPositionOffset[TYPE_ET3_EVENT_DELETE].x, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].y, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pBigGear_Frame);
	}

	if (nullptr != m_pBigGear_Minut)
	{
		m_pBigGear_Minut->Start_Dissolve(73, // Index
			_float4(m_pPositionOffset[TYPE_ET3_EVENT_DELETE].x, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].y, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pBigGear_Minut);
	}

	if (nullptr != m_pBigGear_Second)
	{
		m_pBigGear_Second->Start_Dissolve(73, // Index
			_float4(m_pPositionOffset[TYPE_ET3_EVENT_DELETE].x, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].y, m_pPositionOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pBigGear_Second);
	}

	// ------------------------------------------------------
	if (nullptr != m_pSmallGear_Frame)
	{
		m_pSmallGear_Frame->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_Frame);
	}

	if (nullptr != m_pSmallGear_FrameLine)
	{
		m_pSmallGear_FrameLine->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_FrameLine);
	}

	if (nullptr != m_pSmallGear_GearIn)
	{
		m_pSmallGear_GearIn->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_GearIn);
	}

	if (nullptr != m_pSmallGear_Minut)
	{
		m_pSmallGear_Minut->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_Minut);
	}

	if (nullptr != m_pSmallGear_Second)
	{
		m_pSmallGear_Second->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_Second);
	}

	if (nullptr != m_pSmallGear_Gear01)
	{
		m_pSmallGear_Gear01->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_Gear01);
	}

	if (nullptr != m_pSmallGear_Gear02)
	{
		m_pSmallGear_Gear02->Start_Dissolve(73, // Index
			_float4(m_pScaleOffset[TYPE_ET3_EVENT_DELETE].x, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].y, m_pScaleOffset[TYPE_ET3_EVENT_DELETE].z, 1.f),     // Color
			5.f,   // Speed
			10.f); // Total
		Safe_Release(m_pSmallGear_Gear02);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}