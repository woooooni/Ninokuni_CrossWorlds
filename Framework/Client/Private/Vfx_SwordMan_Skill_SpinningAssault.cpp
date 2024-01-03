#include "stdafx.h"
#include "Vfx_SwordMan_Skill_SpinningAssault.h"

#include "Particle_Manager.h"
#include "Effect_Manager.h"
#include "Character.h"

CVfx_SwordMan_Skill_SpinningAssault::CVfx_SwordMan_Skill_SpinningAssault(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVfx(pDevice, pContext, strObjectTag)
{
}

CVfx_SwordMan_Skill_SpinningAssault::CVfx_SwordMan_Skill_SpinningAssault(const CVfx_SwordMan_Skill_SpinningAssault& rhs)
	: CVfx(rhs)
{
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Initialize_Prototype()
{
 	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Initialize(void* pArg)
{
	m_bOwnerStateIndex = CCharacter::CLASS_SKILL_2;

	return S_OK;
}

void CVfx_SwordMan_Skill_SpinningAssault::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bOwnerTween)
	{
		// 
		if (m_iCount == 0)
		{
			_matrix WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);

			_vector vPositionOrigin = WorldMatrix.r[CTransform::STATE_POSITION];
			_vector vPosition = vPositionOrigin + WorldMatrix.r[CTransform::STATE_LOOK];
			WorldMatrix.r[CTransform::STATE_POSITION] = XMVectorSet(XMVectorGetX(vPosition), XMVectorGetY(vPositionOrigin), XMVectorGetZ(vPosition), 1.f);

			GET_INSTANCE(CEffect_Manager)->Generate_Decal(TEXT("Decal_Swordman_Skill_Perfectblade_Circle"), WorldMatrix, nullptr, m_pOwnerObject);

			m_iCount++;
		}

		// 
		else if (m_iCount == 1 && m_iOwnerFrame >= 15)
		{

			m_iCount++;
		}
	}
}

void CVfx_SwordMan_Skill_SpinningAssault::LateTick(_float fTimeDelta)
{
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Render()
{
	return S_OK;
}

HRESULT CVfx_SwordMan_Skill_SpinningAssault::Ready_Components()
{
	return S_OK;
}

CVfx_SwordMan_Skill_SpinningAssault* CVfx_SwordMan_Skill_SpinningAssault::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CVfx_SwordMan_Skill_SpinningAssault* pInstance = new CVfx_SwordMan_Skill_SpinningAssault(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CVfx_SwordMan_Skill_SpinningAssault::Clone(void* pArg)
{
	CVfx_SwordMan_Skill_SpinningAssault* pInstance = new CVfx_SwordMan_Skill_SpinningAssault(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVfx_SwordMan_Skill_SpinningAssault");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CVfx_SwordMan_Skill_SpinningAssault::Free()
{
	__super::Free();
}