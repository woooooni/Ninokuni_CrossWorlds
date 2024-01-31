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
		m_pScaleOffset[TYPE_JUMP]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_JUMP] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_DOWN_TAIL] = 0;
		m_pPositionOffset[TYPE_DOWN_TAIL] = _float3(10.f, -20.f, 10.f);
		m_pScaleOffset[TYPE_DOWN_TAIL]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_DOWN_TAIL] = _float3(0.f, 0.f, 0.f);

		m_pFrameTriger[TYPE_DOWN_HEAD] = 0;
		m_pPositionOffset[TYPE_DOWN_HEAD] = _float3(-5.f, -10.f, -5.f);
		m_pScaleOffset[TYPE_DOWN_HEAD]    = _float3(1.f, 1.f, 1.f);
		m_pRotationOffset[TYPE_DOWN_HEAD] = _float3(0.f, 0.f, 0.f);
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
		_matrix matWorld = XMMatrixIdentity();
		matWorld.r[CTransform::STATE_POSITION] = vCurrentPosition;
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Whale_Up"),
			matWorld, m_pPositionOffset[TYPE_JUMP], m_pScaleOffset[TYPE_JUMP], m_pRotationOffset[TYPE_JUMP]);
		GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Whale_Down"),
			matWorld, m_pPositionOffset[TYPE_JUMP], m_pScaleOffset[TYPE_JUMP], m_pRotationOffset[TYPE_JUMP]);
		m_iCount++;
	}
	else
	{
		if (vCurrentPosition.x > -30.f)
		{
			_matrix matWorld = XMMatrixIdentity();
			matWorld.r[CTransform::STATE_POSITION] = vCurrentPosition;
			GET_INSTANCE(CParticle_Manager)->Tick_Generate_Particle_To_Matrix(&m_fAccEffect, 0.5f, fTimeDelta, TEXT("Particle_Whale_Water"), matWorld);
		}
		else
		{
			if (m_iCount == TYPE_DOWN_TAIL && vCurrentPosition.x < -90.f && vCurrentPosition.y < 10.f)
			{
				// 바다에 다시 들어갈 때 꼬리
				_matrix matWorld = XMMatrixIdentity();
				matWorld.r[CTransform::STATE_POSITION] = vCurrentPosition;
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Whale_Up_Down"), matWorld, m_pPositionOffset[TYPE_DOWN_TAIL], m_pScaleOffset[TYPE_DOWN_TAIL], m_pRotationOffset[TYPE_DOWN_TAIL]);
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Whale_Down_Down"), matWorld, m_pPositionOffset[TYPE_DOWN_TAIL], m_pScaleOffset[TYPE_DOWN_TAIL], m_pRotationOffset[TYPE_DOWN_TAIL]);
				m_iCount++;
			}

			else if (m_iCount == TYPE_DOWN_HEAD && vCurrentPosition.x < -100.f && vCurrentPosition.y < 2.f)
			{
				// 바다에 다시 들어갈 때 머리
				_matrix matWorld = XMMatrixIdentity();
				matWorld.r[CTransform::STATE_POSITION] = vCurrentPosition;
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Whale_Up"), matWorld, m_pPositionOffset[TYPE_DOWN_HEAD], m_pScaleOffset[TYPE_DOWN_HEAD], m_pRotationOffset[TYPE_DOWN_HEAD]);
				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Whale_Down"), matWorld, m_pPositionOffset[TYPE_DOWN_HEAD], m_pScaleOffset[TYPE_DOWN_HEAD], m_pRotationOffset[TYPE_DOWN_HEAD]);
				m_iCount++;
			}

			else if (m_iCount == TYPE_END || vCurrentPosition.y <= -100.f)
				Set_Dead(true);
		}
	}
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