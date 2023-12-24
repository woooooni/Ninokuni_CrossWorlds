#include "stdafx.h"
#include "Glanix.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "GlanixState_Spawn.h"
#include "GlanixState_CombatIdle.h"
#include "GlanixState_Chase.h"

#include "GlanixState_Attack1.h"
#include "GlanixState_Attack2.h"
#include "GlanixState_SpinBomb.h"
#include "GlanixState_JumpStamp.h"
#include "GlanixState_QuadBlow.h"
#include "GlanixState_IceWave.h"
#include "GlanixState_Charge.h"
#include "GlanixState_SnowBall.h"
#include "GlanixState_SpinBombBomb.h"

#include "GlanixState_Rage.h"

#include "GlanixState_Turn.h"

#include "GlanixState_Dead.h"

CGlanix::CGlanix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CBoss(pDevice, pContext, strObjectTag, tStat)
{
}

CGlanix::CGlanix(const CGlanix& rhs)
	: CBoss(rhs)
{

}

HRESULT CGlanix::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlanix::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//if (FAILED(Ready_Sockets()))
	//	return E_FAIL;

	//if (FAILED(Ready_Parts()))
	//	return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CGlanix::Tick(_float fTimeDelta)
{
	/* юс╫ц */
	if (KEY_TAP(KEY::Z))
		m_tStat.fHp -= 600.f;

	if (KEY_TAP(KEY::B))
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];

	m_pStateCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CGlanix::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	__super::LateTick(fTimeDelta);
}

HRESULT CGlanix::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CGlanix::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
}

void CGlanix::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
}

void CGlanix::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = false;
}

void CGlanix::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}

void CGlanix::Set_SkillTree()
{
	map<_uint, CState*> pStates = m_pStateCom->Get_States();

	for (auto& iter : pStates)
	{
		dynamic_cast<CGlanixState_Base*>(iter.second)->Init_Pattern();
	}
}

HRESULT CGlanix::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 10.f, 10.f, 1.f));

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Glanix"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;
	//RigidDesc.pNavigation = m_pNavigationCom;
	//RigidDesc.PhysXDesc.vOffsetPos = { 0.f, 2.5f, 0.f };
	//RigidDesc.PhysXDesc.vExtents = { 5.f, 5.f, 10.f };

	//RigidDesc.PhysXDesc.eColliderType = PHYSX_COLLIDER_TYPE::BOX;
	//RigidDesc.PhysXDesc.eRigidType = PHYSX_RIGID_TYPE::DYNAMIC;

	//RigidDesc.PhysXDesc.bLockAngle_X = true;
	//RigidDesc.PhysXDesc.bLockAngle_Y = false;
	//RigidDesc.PhysXDesc.bLockAngle_Z = true;

	//RigidDesc.PhysXDesc.bKinematic = false;
	//RigidDesc.PhysXDesc.fAngularDamping = 30.f;
	//RigidDesc.PhysXDesc.fDensity = 1.f;


	//RigidDesc.PhysXDesc.fStaticFriction = 0.f;
	//RigidDesc.PhysXDesc.fDynamicFriction = 1.f;
	//RigidDesc.PhysXDesc.fRestitution = 0.f;

	//RigidDesc.PhysXDesc.fMaxVelocity = 10.f;
	//RigidDesc.PhysXDesc.pGameObject = this;

	//RigidDesc.PhysXDesc.bKinematic = false;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;
	
	m_pStateCom->Set_Owner(this);

	/* For. Disslove Texture */
	//if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), TEXT("Com_DissolveTexture"), (CComponent**)&m_pDissoveTexture)))
	//	return E_FAIL;

	return S_OK;
}

#pragma region Ready_States
HRESULT CGlanix::Ready_States()
{
	m_tStat.fMaxHp = 1000;
	m_tStat.fHp = 1000;
	
	list<wstring> strAnimationName;
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Spawn_New");
	m_pStateCom->Add_State(GLANIX_SPAWN, CGlanixState_Spawn::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BattleStand");
	m_pStateCom->Add_State(GLANIX_COMBATIDLE, CGlanixState_CombatIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Battlerun");
	m_pStateCom->Add_State(GLANIX_CHASE, CGlanixState_Chase::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Attack01");
	m_pStateCom->Add_State(GLANIX_ATTACK1, CGlanixState_Attack1::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Attack02");
	m_pStateCom->Add_State(GLANIX_ATTACK2, CGlanixState_Attack2::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill01");
	m_pStateCom->Add_State(GLANIX_SPINBOMB, CGlanixState_SpinBomb::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill03");
	m_pStateCom->Add_State(GLANIX_JUMPSTAMP, CGlanixState_JumpStamp::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill02_New");
	m_pStateCom->Add_State(GLANIX_QUADBLOW, CGlanixState_QuadBlow::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill06");
	m_pStateCom->Add_State(GLANIX_ICEWAVE, CGlanixState_IceWave::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill07_RootMotion");
	m_pStateCom->Add_State(GLANIX_CHARGE, CGlanixState_Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill08");
	m_pStateCom->Add_State(GLANIX_SNOWBALL, CGlanixState_SnowBall::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Skill09");
	m_pStateCom->Add_State(GLANIX_SPINBOMBBOMB, CGlanixState_SpinBombBomb::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_BossSkillRage");
	m_pStateCom->Add_State(GLANIX_RAGE, CGlanixState_Rage::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_RightTurn180");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn");
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_LeftTurn180");
	m_pStateCom->Add_State(GLANIX_TURN, CGlanixState_Turn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Glanix.ao|Glanix_Death");
	m_pStateCom->Add_State(GLANIX_DEAD, CGlanixState_Dead::Create(m_pStateCom, strAnimationName));


	m_pStateCom->Change_State(GLANIX_SPAWN);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CGlanix::Ready_Colliders()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC ColliderDesc;
	ZeroMemory(&ColliderDesc, sizeof ColliderDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 4.f;
	ColliderDesc.tSphere = tSphere;

	ColliderDesc.pOwner = this;
	ColliderDesc.pNode = nullptr;
	ColliderDesc.pOwnerTransform = m_pTransformCom;
	ColliderDesc.ModePivotMatrix = m_pModelCom->Get_PivotMatrix();
	ColliderDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);
	ColliderDesc.bLockAngle_X = false;
	ColliderDesc.bLockAngle_Y = false;
	ColliderDesc.bLockAngle_Z = false;

	ColliderDesc.fAngularDamping = 0.f;
	ColliderDesc.fDensity = 1.f;

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
		return E_FAIL;


	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CGlanix::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CGlanix::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

CGlanix* CGlanix::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CGlanix* pInstance = new CGlanix(pDevice, pContext, strObjectTag, tStat);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGlanix");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGlanix::Clone(void* pArg)
{
	CGlanix* pInstance = new CGlanix(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGlanix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix::Free()
{
	__super::Free();
}

