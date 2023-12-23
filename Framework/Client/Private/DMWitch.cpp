#include "stdafx.h"
#include "DMWitch.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "DMWitchState_Spawn.h"

#include "DMWitchState_Chase.h"
#include "DMWitchState_Teleport.h"

#include "DMWitchState_Attack1.h"
#include "DMWitchState_Attack2.h"
#include "DMWitchState_QuadBomb.h"
#include "DMWitchState_Charge.h"
#include "DMWitchState_Trampoline.h"
#include "DMWitchState_CollFile.h"
#include "DMWitchState_VulcanBall.h"
#include "DMWitchState_PullAndCombo.h"
#include "DMWitchState_CDStart.h"
#include "DMWitchState_CDVulcanBall.h"
#include "DMWitchState_CDEnd.h"

#include "DMWitchState_Rage.h"
#include "DMWitchState_Turn.h"
#include "DMWitchState_Dead.h"


CDMWitch::CDMWitch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CBoss(pDevice, pContext, strObjectTag, tStat)
{
}

CDMWitch::CDMWitch(const CDMWitch& rhs)
	: CBoss(rhs)
{

}

HRESULT CDMWitch::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDMWitch::Initialize(void* pArg)
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

void CDMWitch::Tick(_float fTimeDelta)
{
	/* 임시 */
	if (KEY_TAP(KEY::Z))
		m_tStat.fHp -= 600.f;

	if (KEY_TAP(KEY::B))
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];

	m_pStateCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CDMWitch::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	__super::LateTick(fTimeDelta);
}

HRESULT CDMWitch::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CDMWitch::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
}

void CDMWitch::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
}

void CDMWitch::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = false;
}

void CDMWitch::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}

void CDMWitch::Set_SkillTree()
{
	map<_uint, CState*> pStates = m_pStateCom->Get_States();

	for (auto& iter : pStates)
	{
		dynamic_cast<CDMWitchState_Base*>(iter.second)->Init_Pattern();
	}
}

HRESULT CDMWitch::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 1.f, 10.f, 1.f));
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(34.511f, -1.229f, 109.102f, 1.f));

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DreamerMazeWitch"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	// 가장 첫 번째 애니메이션의 소리가 재생된다. 방지하자
	m_pModelCom->Set_Animation(TEXT("SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand"));
	GI->Stop_Sound(CHANNELID::SOUND_BOSS);

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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"), TEXT("Com_DissolveTexture"), (CComponent**)&m_pDissoveTexture)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_States
HRESULT CDMWitch::Ready_States()
{
	m_tStat.fMaxHp = 1000;
	m_tStat.fHp = 1000;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(DMWITCH_SPAWN, CDMWitchState_Spawn::Create(m_pStateCom, strAnimationName));
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk");
	m_pStateCom->Add_State(DMWITCH_CHASE, CDMWitchState_Chase::Create(m_pStateCom, strAnimationName));

	// 일단 스탠드로 
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(DMWITCH_TELEPORT, CDMWitchState_Teleport::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack01");
	m_pStateCom->Add_State(DMWITCH_ATTACK1, CDMWitchState_Attack1::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Attack02");
	m_pStateCom->Add_State(DMWITCH_ATTACK2, CDMWitchState_Attack2::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill01");
	m_pStateCom->Add_State(DMWITCH_QUADBOMB, CDMWitchState_QuadBomb::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill03");
	m_pStateCom->Add_State(DMWITCH_CHARGE, CDMWitchState_Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill07");
	m_pStateCom->Add_State(DMWITCH_TRAMPOLINE, CDMWitchState_Trampoline::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill04");
	m_pStateCom->Add_State(DMWITCH_COLLFIRE, CDMWitchState_CollFire::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill06");
	m_pStateCom->Add_State(DMWITCH_VULCANBALL, CDMWitchState_VulcanBall::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill05");
	m_pStateCom->Add_State(DMWITCH_PULLANDCOMBO, CDMWitchState_PullAndCombo::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill021");
	m_pStateCom->Add_State(DMWITCH_CHILDDREAM_START, CDMWitchState_CDStart::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill06");
	m_pStateCom->Add_State(DMWITCH_CHILDDREAM_VULCANBALL, CDMWitchState_CDVulcanBall::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Skill022");
	m_pStateCom->Add_State(DMWITCH_CHILDDREAM_END, CDMWitchState_CDEnd::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_BossSkillRage01");
	m_pStateCom->Add_State(DMWITCH_RAGE, CDMWitchState_Rage::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(DMWITCH_TURN, CDMWitchState_Turn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Death");
	m_pStateCom->Add_State(DMWITCH_DEAD, CDMWitchState_Dead::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(DMWITCH_SPAWN);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CDMWitch::Ready_Colliders()
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
HRESULT CDMWitch::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CDMWitch::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

CDMWitch* CDMWitch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CDMWitch* pInstance = new CDMWitch(pDevice, pContext, strObjectTag, tStat);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CDMWitch");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CDMWitch::Clone(void* pArg)
{
	CDMWitch* pInstance = new CDMWitch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CDMWitch");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDMWitch::Free()
{
	__super::Free();
}

