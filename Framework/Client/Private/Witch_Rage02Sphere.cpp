#include "stdafx.h"
#include "Witch_Rage02Sphere.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Stellia.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Vfx.h"

CWitch_Rage02Sphere::CWitch_Rage02Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_Rage02Sphere::CWitch_Rage02Sphere(const CWitch_Rage02Sphere& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_Rage02Sphere::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWitch_Rage02Sphere::Initialize(void* pArg)
{
	// 테스트 끝나면 풀자.
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

	CSound_Manager::GetInstance()->Play_Sound(TEXT("Witch_Rage02_Sphere.ogg"), CHANNELID::SOUND_VOICE_WITCH_QUEST, .75f, true);

	return S_OK;
}

void CWitch_Rage02Sphere::Tick(_float fTimeDelta)
{
	if (m_pSphere == nullptr && !m_bIsSphereCreate)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_Rage02Sphere_Ready"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pSphere);

		m_bIsSphereCreate = true;
	}
	else
	{
		if (m_pSphere->Is_Dead() && !m_bIsBombCreate)
		{
			Vec4 vPos = m_pTransformCom->Get_Position();
			vPos.y = 1.8f;
			m_pTransformCom->Set_Position(vPos);

			// 폭발 생성
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_BlackHole_Bomb"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pBomb);
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
			m_bIsBombCreate = true;

			CSound_Manager::GetInstance()->Play_Sound(TEXT("Witch_Rage02_SphereBomb.ogg"), CHANNELID::SOUND_VOICE_WITCH_QUEST, .75f, true);
		}
	}

	// 폭발 이펙트
	if (m_pBomb != nullptr)
	{
		m_fAccTime += fTimeDelta;
		if (m_fAccTime > m_fActiveTime)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

		}

		if (m_pBomb->Is_Dead())
		{
			Set_Dead(true);
		}
	}

	__super::Tick(fTimeDelta);
}

void CWitch_Rage02Sphere::LateTick(_float fTimeDelta)
{
	LateUpdate_Collider(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

void CWitch_Rage02Sphere::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
	}
}

void CWitch_Rage02Sphere::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CWitch_Rage02Sphere::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CWitch_Rage02Sphere::Ready_Components()
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
HRESULT CWitch_Rage02Sphere::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 1.5f;
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

CWitch_Rage02Sphere* CWitch_Rage02Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_Rage02Sphere* pInstance = new CWitch_Rage02Sphere(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_Rage02Sphere");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_Rage02Sphere::Clone(void* pArg)
{
	CWitch_Rage02Sphere* pInstance = new CWitch_Rage02Sphere(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_Rage02Sphere");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_Rage02Sphere::Free()
{
	__super::Free();
}

