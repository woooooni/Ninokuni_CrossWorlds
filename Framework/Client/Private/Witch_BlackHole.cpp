#include "stdafx.h"
#include "Witch_BlackHole.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Stellia.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"

#include "Vfx.h"

CWitch_BlackHole::CWitch_BlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CMonsterProjectile(pDevice, pContext, strObjectTag)
{
}

CWitch_BlackHole::CWitch_BlackHole(const CWitch_BlackHole& rhs)
	: CMonsterProjectile(rhs)
{

}

HRESULT CWitch_BlackHole::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CWitch_BlackHole::Initialize(void* pArg)
{
	// Å×½ºÆ® ³¡³ª¸é Ç®ÀÚ.
	//if (pArg == nullptr)
	//	return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	m_fActiveTime = 0.5f;

	CSound_Manager::GetInstance()->Play_Sound(TEXT("Witch_BlackHole.ogg"), CHANNELID::SOUND_VOICE_WITCH_QUEST, .75f, true);

	return S_OK;
}

void CWitch_BlackHole::Tick(_float fTimeDelta)
{
	if (m_pBlackHole == nullptr && !m_bIsBlackHoleCreate)
	{
		GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_BlackHole_Drain"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pBlackHole);

		m_bIsBlackHoleCreate = true;
	}
	else
	{
		if (m_pBlackHole->Is_Dead() && !m_bIsBombCreate)
		{
			// ºí·¢È¦ Æø¹ß »ý¼º
			GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Witch_Skill_BlackHole_Bomb"), m_pTransformCom->Get_WorldMatrix(), nullptr, &m_pEffectBomb);
			Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, true);
			m_bIsBombCreate = true;

			CSound_Manager::GetInstance()->Play_Sound(TEXT("Witch_BlackHoleBomb.ogg"), CHANNELID::SOUND_VOICE_WITCH_QUEST, .75f, true);
		}
	}

	// Æø¹ß ÀÌÆåÆ®
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

void CWitch_BlackHole::LateTick(_float fTimeDelta)
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

void CWitch_BlackHole::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
	{
		Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);
	}
}

void CWitch_BlackHole::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CWitch_BlackHole::Collision_Exit(const COLLISION_INFO& tInfo)
{

}

HRESULT CWitch_BlackHole::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(161.5f, 2.311f, 147.5f, 1.f));

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CWitch_BlackHole::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 2.5f;
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

CWitch_BlackHole* CWitch_BlackHole::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CWitch_BlackHole* pInstance = new CWitch_BlackHole(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CWitch_BlackHole");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CWitch_BlackHole::Clone(void* pArg)
{
	CWitch_BlackHole* pInstance = new CWitch_BlackHole(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CWitch_BlackHole");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWitch_BlackHole::Free()
{
	__super::Free();
}

