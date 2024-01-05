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
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_0;

 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_PerfectBlade::Initialize(void* pArg)
{
	return S_OK;
}

void CVfx_SwordMan_Skill_PerfectBlade::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// Decal 1
		if (m_iCount == 0)
		{
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK];
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin), XMVectorGetZ(vPosition), 1.f);

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), WorldMatrix);

			m_iCount++;
		}

		// TrailL 1
		else if (m_iCount == 1 && m_iOwnerFrame >= 15) // m_pOwnerObject->Get_Component<CModel>(L"Com_Model")->Get_CurrAnimationFrame() == 2
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect(TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				XMLoadFloat4x4(&m_WorldMatrix),
				::XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f),
				_float3(5.f, 5.f, 5.f),
				_float3(0.f, 20.f, 0.f), 
				m_pOwnerObject);
			
			m_iCount++;
		}
		// TrailR 2
		else if (m_iCount == 2 && m_iOwnerFrame >= 22)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, -20.f, 0.f)),
				nullptr
			);

			m_iCount++;
		}
		// TrailL 3
		else if (m_iCount == 3 && m_iOwnerFrame >= 28)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 20.f, 0.f)),
				nullptr
			);

			m_iCount++;
		}
		// TrailR 4
		else if (m_iCount == 4 && m_iOwnerFrame >= 34)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Right"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, -20.f, 0.f)),
				nullptr
			);

			m_iCount++;
		}
		// TrailL 5
		else if (m_iCount == 5 && m_iOwnerFrame >= 39)
		{
			GET_INSTANCE(CEffect_Manager)->Generate_Effect
			(
				TEXT("Effect_Swordman_Skill_Perfectblade_Trail_Basic_Left"),
				GET_INSTANCE(CEffect_Manager)->Get_WorldMatrixEffect(XMLoadFloat4x4(&m_WorldMatrix), _float3(0.f, 1.f, 0.f), _float3(5.f, 5.f, 5.f), _float3(0.f, 20.f, 0.f)),
				nullptr
			);

			m_iCount++;
		}

		// Sparkle 1
		else if (m_iCount == 6 && m_iOwnerFrame >= 45)
		{
			// 哭率 第
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin - (WorldMatrix.r[CTransform::STATE_LOOK] * 0.5f);
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 0.5f, XMVectorGetZ(vPosition), 1.f);
			WorldMatrix.r[CTransform::STATE_POSITION] -= WorldMatrix.r[CTransform::STATE_RIGHT] * 0.2f;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Sparkle"), WorldMatrix);

			m_iCount++;
		}

		// TrailR 6 (Special) / Sparkle_Circle
		else if (m_iCount == 7 && m_iOwnerFrame >= 47)
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

			m_iCount++;
		}

		// Fire 1
		else if (m_iCount == 8 && m_iOwnerFrame >= 48)
		{
			// 哭率 第
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin - WorldMatrix.r[CTransform::STATE_LOOK] * 0.2f;
			vPosition += WorldMatrix.r[CTransform::STATE_LOOK] * 0.5f;
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			WorldMatrix.r[CTransform::STATE_POSITION] -= WorldMatrix.r[CTransform::STATE_RIGHT] * 0.5f;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), WorldMatrix);

			m_iCount++;
		}
		// Fire 2
		else if (m_iCount == 9 && m_iOwnerFrame >= 49)
		{
			// 吝缴
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK] * 0.5f;
			vPosition += WorldMatrix.r[CTransform::STATE_LOOK] * 0.5f;
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), WorldMatrix);

			m_iCount++;
		}
		// Fire 3
		else if (m_iCount == 10 && m_iOwnerFrame >= 50)
		{
			// 坷弗率 第
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector  vPosition = vPositionOrigin - WorldMatrix.r[CTransform::STATE_LOOK] * 0.2;
			vPosition += WorldMatrix.r[CTransform::STATE_LOOK] * 0.5f;
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin) + 1.f, XMVectorGetZ(vPosition), 1.f);
			WorldMatrix.r[CTransform::STATE_POSITION] += WorldMatrix.r[CTransform::STATE_RIGHT] * 0.5f;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Swordman_Skill_Perfectblade_Trail_Fire"), WorldMatrix);

			m_iCount++;
		}

		// Dead
		else if (m_iCount == 11)
		    m_bFinish = true;
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