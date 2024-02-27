#include "stdafx.h"
#include "Witch_Rage01QuadBlackHole.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Stellia.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Vfx.h"

CWitch_Rage01QuadBlackHole::CWitch_Rage01QuadBlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_Rage01QuadBlackHole::CWitch_Rage01QuadBlackHole(const CWitch_Rage01QuadBlackHole& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_Rage01QuadBlackHole::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWitch_Rage01QuadBlackHole::Initialize(void* pArg)
{
	// �׽�Ʈ ������ Ǯ��.
	if (pArg == nullptr)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	m_fActiveTime = 0.5f;

	return S_OK;
}

void CWitch_Rage01QuadBlackHole::Tick(_float fTimeDelta)
{
	if (m_pBlackHole == nullptr && !m_bIsBlackHoleCreate)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_Rage01QuadBlackHole_Explosive"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pBlackHole);

		m_bIsBlackHoleCreate = true;
	}
	else
	{
		if (m_pBlackHole->Is_Dead() && !m_bIsBombCreate)
		{
			// ��Ȧ ���� ����
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_Rage01QuadBlackHole_Bomb"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pEffectBomb);
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
			m_bIsBombCreate = true;
		}
	}

	// ���� ����Ʈ
	if (m_pEffectBomb != nullptr)
	{
		m_fAccTime += fTimeDelta;
		if (m_fAccTime > m_fActiveTime)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
		}

		if (m_pEffectBomb->Is_Dead())
		{
			Set_Dead(true);
		}
	}

	__super::Tick(fTimeDelta);
}

void CWitch_Rage01QuadBlackHole::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

#ifdef NDEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

void CWitch_Rage01QuadBlackHole::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
	}
}

void CWitch_Rage01QuadBlackHole::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CWitch_Rage01QuadBlackHole::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CWitch_Rage01QuadBlackHole::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;



	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CWitch_Rage01QuadBlackHole::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 4.5f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = {};
	SphereDesc.vOffsetPosition = Vec3(0.f, 0.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CWitch_Rage01QuadBlackHole* CWitch_Rage01QuadBlackHole::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_Rage01QuadBlackHole* pInstance = new CWitch_Rage01QuadBlackHole(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_Rage01QuadBlackHole");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_Rage01QuadBlackHole::Clone(void* pArg)
{
	CWitch_Rage01QuadBlackHole* pInstance = new CWitch_Rage01QuadBlackHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_Rage01QuadBlackHole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_Rage01QuadBlackHole::Free()
{
	__super::Free();
}

