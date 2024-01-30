#include "stdafx.h"
#include "Vfx_Whale_Jump.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"

CVfx_Whale_Jump::CVfx_Whale_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_Whale_Jump::CVfx_Whale_Jump(const CVfx_Whale_Jump& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_Whale_Jump::Initialize_Prototype()
{
	m_iMaxCount = TYPE_END;
	m_pFrameTriger    = new _int[m_iMaxCount];
	m_pPositionOffset = new _float3[m_iMaxCount];
	m_pScaleOffset    = new _float3[m_iMaxCount];
	m_pRotationOffset = new _float3[m_iMaxCount];

	{
		m_pFrameTriger[TYPE_JUMP] = 0;
		m_pPositionOffset[TYPE_JUMP] = _float3(0.f, 0.f, 0.f);
		m_pScaleOffset[TYPE_JUMP]    = _float3(8.f, 3.f, 8.f);
		m_pRotationOffset[TYPE_JUMP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_DOWN] = 0;
		m_pPositionOffset[TYPE_DOWN] = _float3(0.f, 1.f, 0.f);
		m_pScaleOffset[TYPE_DOWN]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_DOWN] = _float3(0.f, 0.f, 0.f);
	}

 	return S_OK;
}

HRESULT CVfx_Whale_Jump::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_Whale_Jump::Tick(_float fTimeDelta)
{
	if (nullptr == m_pOwnerObject || true == m_pOwnerObject->Is_ReserveDead() || true == m_pOwnerObject->Is_Dead())
	{
		Set_Dead(true);
		return;
	}

	CTransform* pOwnerTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
	if(nullptr == pOwnerTransform)
	{
		Set_Dead(true);
		return;
	}

	// 시작 포지션 : 40.9144 / -83.917 / 43.367 
	Vec3 vCurrentPosition = pOwnerTransform->Get_Position();
	if (m_iCount == TYPE_JUMP && vCurrentPosition.y > -10.f)
	{
		// 얼굴 내밀었을 때
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT(""),
			pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_JUMP], m_pScaleOffset[TYPE_JUMP], m_pRotationOffset[TYPE_JUMP]);
		m_iCount++;
	}
	else if (m_iCount == TYPE_DOWN && vCurrentPosition.x < -100.f && vCurrentPosition.y < 30.f)
	{
		// 바다에 다시 들어갈 때
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT(""),
			pOwnerTransform->Get_WorldMatrix(), m_pPositionOffset[TYPE_DOWN], m_pScaleOffset[TYPE_DOWN], m_pRotationOffset[TYPE_DOWN]);
		m_iCount++;
	}

	else if (m_iCount == TYPE_END || vCurrentPosition.y <= -100.f)
		Set_Dead(true);
}

void CVfx_Whale_Jump::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_Whale_Jump::Render()
{
	return S_OK;
}

HRESULT CVfx_Whale_Jump::Ready_Components()
{
	return S_OK;
}

CVfx_Whale_Jump* CVfx_Whale_Jump::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_Whale_Jump* pInstance = new CVfx_Whale_Jump(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_Whale_Jump");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_Whale_Jump::Clone(void* pArg)
{
	CVfx_Whale_Jump* pInstance = new CVfx_Whale_Jump(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_Whale_Jump");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_Whale_Jump::Free()
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