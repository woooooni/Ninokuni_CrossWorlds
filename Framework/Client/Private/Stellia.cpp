#include "stdafx.h"
#include "Stellia.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "StelliaState_Spawn.h"
#include "StelliaState_CombatIdle.h"
#include "StelliaState_Chase.h"

#include "StelliaState_Attack1.h"
#include "StelliaState_Attack2.h"
#include "StelliaState_Laser.h"
#include "StelliaState_TripleLaser.h"
#include "StelliaState_JumpStamp.h"
#include "StelliaState_Charge.h"
#include "StelliaState_SpinTail.h"
#include "StelliaState_BigBang.h"

#include "StelliaState_Rage.h"

#include "StelliaState_Turn.h"

#include "StelliaState_Dead.h"

CStellia::CStellia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CBoss(pDevice, pContext, strObjectTag, tStat)
{
}

CStellia::CStellia(const CStellia& rhs)
	: CBoss(rhs)
{

}

HRESULT CStellia::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CStellia::Initialize(void* pArg)
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

void CStellia::Tick(_float fTimeDelta)
{
	/* юс╫ц */
	if (KEY_TAP(KEY::Z))
		m_tStat.fHp -= 600.f;

	if (KEY_TAP(KEY::B))
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = !m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND];

	m_pStateCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CStellia::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	__super::LateTick(fTimeDelta);
}

HRESULT CStellia::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CStellia::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
}

void CStellia::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
}

void CStellia::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = false;
}

void CStellia::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}

void CStellia::Set_SkillTree()
{
	map<_uint, CState*> pStates = m_pStateCom->Get_States();

	for (auto& iter : pStates)
	{
		dynamic_cast<CStelliaState_Base*>(iter.second)->Init_Pattern();
	}
}

HRESULT CStellia::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Stellia"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
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
HRESULT CStellia::Ready_States()
{
	m_tStat.fMaxHp = 1000;
	m_tStat.fHp = 1000;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Spawn");
	m_pStateCom->Add_State(STELLIA_SPAWN, CStelliaState_Spawn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Stand02");
	m_pStateCom->Add_State(STELLIA_COMBATIDLE, CStelliaState_CombatIdle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Run");
	m_pStateCom->Add_State(STELLIA_CHASE, CStelliaState_Chase::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Attack01");
	m_pStateCom->Add_State(STELLIA_ATTACK1, CStelliaState_Attack1::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Attack02");
	m_pStateCom->Add_State(STELLIA_ATTACK2, CStelliaState_Attack2::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill01");
	m_pStateCom->Add_State(STELLIA_SPINTAIL, CStelliaState_SpinTail::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill02_New");
	m_pStateCom->Add_State(STELLIA_JUMPSTAMP, CStelliaState_JumpStamp::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill05_New");
	m_pStateCom->Add_State(STELLIA_LASER, CStelliaState_Laser::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill03_New");
	m_pStateCom->Add_State(STELLIA_TRIPLELASER, CStelliaState_TripleLaser::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill06");
	m_pStateCom->Add_State(STELLIA_CHARGE, CStelliaState_Charge::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkill04_New");
	m_pStateCom->Add_State(STELLIA_BIGBANG, CStelliaState_BigBang::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_BossSkillRage");
	m_pStateCom->Add_State(STELLIA_RAGE, CStelliaState_Rage::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_RightTurn180");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_LeftTurn");
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_InstantTurn");
	m_pStateCom->Add_State(STELLIA_TURN, CStelliaState_Turn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Stellia.ao|Stellia_Death");
	m_pStateCom->Add_State(STELLIA_DEAD, CStelliaState_Dead::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(STELLIA_SPAWN);

	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CStellia::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 200.f, 250.f, 350.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 150.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	/* Atk */
	OBBDesc.vOffsetPosition = Vec3(0.f, 150.f, -200.f);
	OBBBox.Extents = { 200.f, 200.f, 100.f };
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	ZeroMemory(&SphereDesc, sizeof SphereDesc);

	BoundingSphere tSphere;
	ZeroMemory(&tSphere, sizeof(BoundingSphere));
	tSphere.Radius = 5.f;
	SphereDesc.tSphere = tSphere;

	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3(0.f, 50.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CStellia::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CStellia::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

CStellia* CStellia::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
{
	CStellia* pInstance = new CStellia(pDevice, pContext, strObjectTag, tStat);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CStellia");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CStellia::Clone(void* pArg)
{
	CStellia* pInstance = new CStellia(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStellia");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStellia::Free()
{
	__super::Free();
}

