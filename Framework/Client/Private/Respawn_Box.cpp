#include "stdafx.h"
#include "GameInstance.h"
#include "Respawn_Box.h"
#include "Utils.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "Whale.h"

#include "Camera_Manager.h"
#include "Camera_CutScene_Map.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

CRespawn_Box::CRespawn_Box(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Respawn_Box", OBJ_TYPE::OBJ_RESPAWN)
{
}

CRespawn_Box::CRespawn_Box(const CRespawn_Box& rhs)
	: CGameObject(rhs)
{
}


HRESULT CRespawn_Box::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CRespawn_Box::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	RESPAWN_DESC* pRespawnDesc = (RESPAWN_DESC*)pArg;

	m_vRespawnPosition = pRespawnDesc->vRespawnPosition;
	m_vExtents = pRespawnDesc->vExtents;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Collider()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(Vec4(pRespawnDesc->vStartPosition), 1.f));

	return S_OK;
}

void CRespawn_Box::Tick(_float fTimeDelta)
{
	GI->Add_CollisionGroup(COLLISION_GROUP::RESPAWN, this);
	__super::Tick(fTimeDelta);
}

void CRespawn_Box::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])		
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CRespawn_Box::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}



void CRespawn_Box::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if (OBJ_TYPE::OBJ_CHARACTER == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		CTransform* pOtherTransform = tInfo.pOther->Get_Component<CTransform>(L"Com_Transform");
		CPhysX_Controller* pOtherController = tInfo.pOther->Get_Component<CPhysX_Controller>(L"Com_Controller");
		if (nullptr != pOtherTransform && nullptr != pOtherController)
		{
			pOtherTransform->Set_State(CTransform::STATE_POSITION, m_vRespawnPosition);
			pOtherController->Set_EnterLevel_Position(m_vRespawnPosition);
		}
	}

	else if (OBJ_TYPE::OBJ_MONSTER == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		tInfo.pOther->Set_Dead(true);
	}

	else if (OBJ_TYPE::OBJ_NPC == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		CTransform* pOtherTransform = tInfo.pOther->Get_Component<CTransform>(L"Com_Transform");
		CPhysX_Controller* pOtherController = tInfo.pOther->Get_Component<CPhysX_Controller>(L"Com_Controller");

		pOtherTransform->Set_State(CTransform::STATE_POSITION, m_vRespawnPosition);
		pOtherController->Set_EnterLevel_Position(m_vRespawnPosition);
	}
}

void CRespawn_Box::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CRespawn_Box::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CRespawn_Box::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRespawn_Box::Ready_Collider()
{

	CCollider_AABB::AABB_COLLIDER_DESC AABBDesc;
	BoundingBox AABBBox;
	AABBBox.Extents = m_vExtents;
	AABBDesc.tBox = AABBBox;
	AABBDesc.pNode = nullptr;
	AABBDesc.pOwnerTransform = m_pTransformCom;
	AABBDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	AABBDesc.vOffsetPosition = { 0.f, m_vExtents.y, 0.f };

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::AABB, CCollider::DETECTION_TYPE::BODY, &AABBDesc)))
		return E_FAIL;

	return S_OK;
}


CRespawn_Box* CRespawn_Box::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRespawn_Box* pInstance = new CRespawn_Box(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CRespawn_Box");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRespawn_Box::Clone(void* pArg)
{
	CRespawn_Box* pInstance = new CRespawn_Box(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CRespawn_Box");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRespawn_Box::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
