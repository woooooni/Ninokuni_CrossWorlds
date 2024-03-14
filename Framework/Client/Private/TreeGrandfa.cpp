#include "stdafx.h"
#include "TreeGrandfa.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

#include "Monster.h"
#include "MonsterProjectile.h"

CTreeGrandfa::CTreeGrandfa(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CTreeGrandfa::CTreeGrandfa(const CTreeGrandfa& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CTreeGrandfa::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeGrandfa::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_TreeGrandfa.ao|TreeGrandfa_Stand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CTreeGrandfa::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::DEFENCE_TOWER, this);
}

void CTreeGrandfa::LateTick(_float fTimeDelta)
{
	// __super::LateTick(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
	m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);

	LateUpdate_Collider(fTimeDelta);

#ifdef _DEBUG
	m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CTreeGrandfa::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTreeGrandfa::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER_PROJECTILE)
	{
		if ((tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
			&& (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK))
		{
			On_Damaged(tInfo);
		}
	}
}

void CTreeGrandfa::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::On_Damaged(const COLLISION_INFO& tInfo)
{
	if (m_pStateCom->Get_CurrState() == CCharacter::STATE::DEAD || m_pStateCom->Get_CurrState() == CCharacter::STATE::REVIVE)
		return;

	CMonster* pMonster = nullptr;
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_MONSTER_PROJECTILE)
	{
		CMonsterProjectile* pProjectile = dynamic_cast<CMonsterProjectile*>(tInfo.pOther);
		if (nullptr == pProjectile)
		{
			MSG_BOX("CMonsterProjectile Cast Failed.");
			return;
		}
		pMonster = pProjectile->Get_Owner();
	}
	else
	{
		pMonster = dynamic_cast<CMonster*>(tInfo.pOther);
	}

	if (nullptr == pMonster)
		return;

	_int iDamage = max(0, pMonster->Get_Stat().iAtk - (m_tStat.iDef * 0.2f));

	m_tStat.iHp = max(0, m_tStat.iHp - iDamage);
}


HRESULT CTreeGrandfa::Ready_States()
{
	m_strKorName = TEXT("가을 영감");
	m_tStat.iMaxHp = 5000;
	m_tStat.iHp = 5000;
	m_tStat.iDef = 10;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Talk01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Talk01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Talk01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CTreeGrandfa::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 900.f, 600.f, 900.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 600.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeGrandfa::Ready_Sockets()
{
	return S_OK;
}

CTreeGrandfa* CTreeGrandfa::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CTreeGrandfa* pInstance = new CTreeGrandfa(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CTreeGrandfa");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTreeGrandfa::Clone(void* pArg)
{
	CTreeGrandfa* pInstance = new CTreeGrandfa(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CTreeGrandfa");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTreeGrandfa::Free()
{
	__super::Free();
}