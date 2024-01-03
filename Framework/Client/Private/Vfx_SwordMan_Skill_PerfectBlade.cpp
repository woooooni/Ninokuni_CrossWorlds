#include "stdafx.h"
#include "Vfx_SwordMan_Skill_PerfectBlade.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_PerfectBlade::CVfx_SwordMan_Skill_PerfectBlade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_PerfectBlade::CVfx_SwordMan_Skill_PerfectBlade(const CVfx_SwordMan_Skill_PerfectBlade& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_PerfectBlade::Tick(_float fTimeDelta)
{
	if (m_pOwnerObject != nullptr)
	{
		CStateMachine* pMachine = m_pOwnerObject->Get_Component<CStateMachine>(L"Com_StateMachine");
		if (pMachine != nullptr)
		{
			if (pMachine->Get_CurrState() != CCharacter::CLASS_SKILL_0)
			{
				Set_Dead(true);
				return;
			}
		}

		m_fTimeAcc += fTimeDelta;
		// Decal 1
		if (m_iCount == 0)
		{
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK];
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin), XMVectorGetZ(vPosition), 1.f);

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), WorldMatrix);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}

		// TrailL 1
		else if (m_iCount == 1 && m_fTimeAcc > 0.7f) // m_pOwnerObject->Get_Component<CModel>(L"Com_Model")->Get_CurrAnimationFrame() == 2
		{
			/*
			//_float3 fPositionOffset = ;
			//_float3 fScaleOffset    = ;
			//_float3 fRotationOffset = ;

			////////////////////////////////////////////
			_matrix OffsetMatrix = XMMatrixIdentity();
			//_float4x4 WorldMatrix;
			//XMStoreFloat4x4(&WorldMatrix, OffsetMatrix);

			//// 포지션
			//WorldMatrix.m[3][0] += fPositionOffset.x;
			//WorldMatrix.m[3][1] += fPositionOffset.y;
			//WorldMatrix.m[3][2] += fPositionOffset.z;

			// 크기
			XMVECTOR vRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) * fScaleOffset.x;
			XMVECTOR vUp    = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) * fScaleOffset.y;
			XMVECTOR vLook  = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f) * fScaleOffset.z;

			// 회전
			OffsetMatrix *= XMMatrixRotationX(XMConvertToRadians(fRotationOffset.x));
			OffsetMatrix *= XMMatrixRotationY(XMConvertToRadians(fRotationOffset.y));
			OffsetMatrix *= XMMatrixRotationZ(XMConvertToRadians(fRotationOffset.z));
			OffsetMatrix.r[0] = XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), OffsetMatrix);
			OffsetMatrix.r[1] = XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), OffsetMatrix);
			OffsetMatrix.r[2] = XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), OffsetMatrix);

			//GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Ver3"), XMLoadFloat4x4(&BindWorldMatrix), &RotationMatrix);
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Ver3"), XMMatrixIdentity(), &OffsetMatrix, m_pOwnerObject);
			////////////////////////////////////////////

			// 크기 && 회전
			//XMVECTOR vRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);// *fScaleOffset.x;
			//XMVECTOR vUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);// *fScaleOffset.y;
			//XMVECTOR vLook = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);// *fScaleOffset.z;

			//_matrix RotationMatrix = XMMatrixIdentity();
			//RotationMatrix *= XMMatrixRotationX(XMConvertToRadians(fRotationOffset.x));
			//RotationMatrix *= XMMatrixRotationY(XMConvertToRadians(fRotationOffset.y));
			//RotationMatrix *= XMMatrixRotationZ(XMConvertToRadians(fRotationOffset.z));
			//vRight = XMVector3TransformNormal(vRight, RotationMatrix);
			//vUp    = XMVector3TransformNormal(vUp,    RotationMatrix);
			//vLook  = XMVector3TransformNormal(vLook,  RotationMatrix);

			////_float4x4 BindOffsetMatrix;
			//_matrix OffsetMatrix = XMMatrixIdentity();
			//OffsetMatrix.r[0] = vRight;
			//OffsetMatrix.r[1] = vUp;
			//OffsetMatrix.r[2] = vLook;
			//OffsetMatrix.r[3] = XMVectorSet(fPositionOffset.x, fPositionOffset.y, fPositionOffset.z, 1.0f);
			//XMStoreFloat4x4(&BindOffsetMatrix, OffsetMatrix);

			// 포지션
			//m_WorldMatrix.m[3][0] += m_WorldMatrix.m[0][0] * -5.f;
			//m_WorldMatrix.m[3][1] += m_WorldMatrix.m[1][0] * -2.f;
			//m_WorldMatrix.m[3][2] += m_WorldMatrix.m[2][0] * 1.f;

			//GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Ver3"), XMLoadFloat4x4(&BindWorldMatrix), &RotationMatrix);

			//CTransform* pTransform = m_pOwnerObject->Get_Component<CTransform>(L"Com_Transform");
			// pTransform->Get_WorldMatrix();
			*/

			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 20.f, 0.f)),
				nullptr//&GET_INSTANCE(CEffect_Manager)->Get_RotationMatrix(_float3(0.f, 0.f, 45.f))
			);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}
		// TrailR 2
		else if (m_iCount == 2 && m_fTimeAcc > 0.3f)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, -20.f, 0.f)),
				nullptr
			);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}
		// TrailL 3
		else if (m_iCount == 3 && m_fTimeAcc > 0.3f)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 20.f, 0.f)),
				nullptr
			);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}
		// TrailR 4
		else if (m_iCount == 4 && m_fTimeAcc > 0.3f)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, -20.f, 0.f)),
				nullptr
			);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}
		// TrailL 5
		else if (m_iCount == 5 && m_fTimeAcc > 0.3f)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 20.f, 0.f)),
				nullptr
			);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}

		// Sparkle 1
		else if (m_iCount == 6 && m_fTimeAcc > 0.25f)
		{
		    // 왼쪽 뒤
		    _matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		    _vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
		    _vector vPosition = vPositionOrigin - (WorldMatrix.r[CTransform::STATE_LOOK] * 0.5f);
		    WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 0.5f, XMVectorGetZ(vPosition), 1.f);
			WorldMatrix.r[CTransform::STATE_POSITION] -= WorldMatrix.r[CTransform::STATE_RIGHT] * 0.2f;
		    GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle"), WorldMatrix);
		    
		    m_fTimeAcc = 0.f;
		    m_iCount++;
		}

		// TrailR 6 (Special) / Sparkle_Circle
		else if (m_iCount == 7 && m_fTimeAcc > 0.05f)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, -20.f, 0.f)),
				nullptr
			);

			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK] * 0.5;
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle_Circle"), WorldMatrix);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}

		// Fire 01
		else if (m_iCount == 8 && m_fTimeAcc > 0.05f)
		{
			// 왼쪽 뒤
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK] * 0.5;
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			WorldMatrix.r[CTransform::STATE_POSITION] -= WorldMatrix.r[CTransform::STATE_RIGHT] * 0.5f;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), WorldMatrix);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}
		// Fire 1
		else if (m_iCount == 9 && m_fTimeAcc > 0.05f)
		{
			// 중심
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK];
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), WorldMatrix);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}
		// Fire 2
		else if (m_iCount == 10 && m_fTimeAcc > 0.05f)
		{
			// 오른쪽 뒤
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector  vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK] * 0.5;
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			WorldMatrix.r[CTransform::STATE_POSITION] += WorldMatrix.r[CTransform::STATE_RIGHT] * 0.5f;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), WorldMatrix);

			m_fTimeAcc = 0.f;
			m_iCount++;
		}

		// Dead
		else if (m_iCount == 11)
			Set_Dead(true);
	}
}

void CVfx_SwordMan_Skill_PerfectBlade::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_PerfectBlade* CVfx_SwordMan_Skill_PerfectBlade::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_PerfectBlade* pInstance = new CVfx_SwordMan_Skill_PerfectBlade(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_PerfectBlade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_PerfectBlade::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_PerfectBlade* pInstance = new CVfx_SwordMan_Skill_PerfectBlade(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_PerfectBlade");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_PerfectBlade::Free()
{
	__super::Free();
}