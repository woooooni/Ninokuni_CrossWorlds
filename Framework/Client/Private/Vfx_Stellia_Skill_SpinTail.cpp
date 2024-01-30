#include "stdafx.h"
#include "Vfx_Stellia_Skill_SpinTail.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Stellia.h"

#include "Effect.h"

CVfx_Stellia_Skill_SpinTail::CVfx_Stellia_Skill_SpinTail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Stellia_Skill_SpinTail::CVfx_Stellia_Skill_SpinTail(const CVfx_Stellia_Skill_SpinTail& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Stellia_Skill_SpinTail::Initialize_Prototype()
{
	m_bOwnerStateIndex = CStellia::STELLIA_SPINTAIL;

	m_iMaxCount = TYPE_END;
	m_pFrameTriger = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	// 0
	m_pFrameTriger[TYPE_E_TRAIL_01] = 57;
	m_pPositionOffset[TYPE_E_TRAIL_01] = _float3(0.f, 0.250, 0.f);
	m_pScaleOffset[TYPE_E_TRAIL_01] = _float3(5.f, 5.f, 5.f);
	m_pRotationOffset[TYPE_E_TRAIL_01] = _float3(0.f, 90.f, 0.f);

	m_pFrameTriger[TYPE_V_E_SPEED_01] = 110;
	m_pPositionOffset[TYPE_V_E_SPEED_01] = _float3(0.f, 0.f, 0.f); // x�� �ӵ� ����
	m_pScaleOffset[TYPE_V_E_SPEED_01] = _float3(0.f, 4.f, 0.f);  // x�� TurnSpeed ����
	m_pRotationOffset[TYPE_V_E_SPEED_01] = _float3(0.f, 1.f, 0.f); // ȸ�� ��ų �� ����

	// 1
	m_pFrameTriger[TYPE_E_TRAIL_02] = 60;
	m_pPositionOffset[TYPE_E_TRAIL_02] = _float3(0.f, 0.50, 0.f);
	m_pScaleOffset[TYPE_E_TRAIL_02] = _float3(1.f, 1.f, 1.f);
	m_pRotationOffset[TYPE_E_TRAIL_02] = _float3(0.f, 90.f, 0.f);

	m_pFrameTriger[TYPE_V_E_SPEED_02] = 110;
	m_pPositionOffset[TYPE_V_E_SPEED_02] = _float3(0.f, 0.f, 0.f); // x�� �ӵ� ����
	m_pScaleOffset[TYPE_V_E_SPEED_02] = _float3(0.f, 4.f, 0.f);  // x�� TurnSpeed ����
	m_pRotationOffset[TYPE_V_E_SPEED_02] = _float3(0.f, 1.f, 0.f); // ȸ�� ��ų �� ����


	return S_OK;
}

HRESULT CVfx_Stellia_Skill_SpinTail::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Stellia_Skill_SpinTail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		if (m_iCount == TYPE_E_TRAIL_01 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_01])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_SpinTail"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_01], m_pScaleOffset[TYPE_E_TRAIL_01], m_pRotationOffset[TYPE_E_TRAIL_01], nullptr, &m_pTail01Effect, false);
			Safe_AddRef(m_pTail01Effect);
			m_iCount++;
		}

		else if (m_iCount == TYPE_E_TRAIL_02 && m_iOwnerFrame >= m_pFrameTriger[TYPE_E_TRAIL_02])
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Stellia_Stellia_Skill_TailSmoke"),
				XMLoadFloat4x4(&m_WorldMatrix), m_pPositionOffset[TYPE_E_TRAIL_02], m_pScaleOffset[TYPE_E_TRAIL_02], m_pRotationOffset[TYPE_E_TRAIL_02], nullptr, &m_pTail01Effect, false);
			Safe_AddRef(m_pTail01Effect);
			m_iCount++;
		}

		if (m_pTail01Effect != nullptr)
		{
			CTransform* pHand01EffectTransform = m_pTail01Effect->Get_Component<CTransform>(TEXT("Com_Transform"));
			// pHand01EffectTransform->Move(pHand01EffectTransform->Get_Look(), m_pPositionOffset[TYPE_V_E_SPEED].x, fTimeDelta);
			pHand01EffectTransform->Turn(Vec3(m_pRotationOffset[TYPE_V_E_SPEED_01]), -m_pScaleOffset[TYPE_V_E_SPEED_01].y, fTimeDelta);
		}

		//if (m_pTail02Effect != nullptr)
		//{
		//	CTransform* pHand01EffectTransform = m_pTail02Effect->Get_Component<CTransform>(TEXT("Com_Transform"));
		//	// pHand01EffectTransform->Move(pHand01EffectTransform->Get_Look(), m_pPositionOffset[TYPE_V_E_SPEED].x, fTimeDelta);
		//	pHand01EffectTransform->Turn(Vec3(m_pRotationOffset[TYPE_V_E_SPEED_02]), -m_pScaleOffset[TYPE_V_E_SPEED_02].y, fTimeDelta);
		//}
	}
}

void CVfx_Stellia_Skill_SpinTail::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Stellia_Skill_SpinTail::Render()
{
	return S_OK;
}

HRESULT CVfx_Stellia_Skill_SpinTail::Ready_Components()
{
	return S_OK;
}

CVfx_Stellia_Skill_SpinTail* CVfx_Stellia_Skill_SpinTail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Stellia_Skill_SpinTail* pInstance = new CVfx_Stellia_Skill_SpinTail(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Stellia_Skill_SpinTail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Stellia_Skill_SpinTail::Clone(void* pArg)
{
	CVfx_Stellia_Skill_SpinTail* pInstance = new CVfx_Stellia_Skill_SpinTail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Stellia_Skill_SpinTail");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Stellia_Skill_SpinTail::Free()
{
	__super::Free();

	if (nullptr != m_pTail01Effect)
	{
		m_pTail01Effect->Set_Dead(true);
		Safe_Release(m_pTail01Effect);
	}

	if (!m_isCloned)
	{
		Safe_Delete_Array(m_pFrameTriger);
		Safe_Delete_Array(m_pPositionOffset);
		Safe_Delete_Array(m_pScaleOffset);
		Safe_Delete_Array(m_pRotationOffset);
	}
}