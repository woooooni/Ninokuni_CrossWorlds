#include "stdafx.h"
#include "Character_SwordMan.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"




CCharacter_SwordMan::CCharacter_SwordMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType)
	: CCharacter(pDevice, pContext, strObjectTag, eCharacterType)
{
}

CCharacter_SwordMan::CCharacter_SwordMan(const CCharacter_SwordMan& rhs)
	: CCharacter(rhs)
{

}

HRESULT CCharacter_SwordMan::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CCharacter_SwordMan::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//if (FAILED(Ready_Sockets()))
	//	return E_FAIL;

	//if (FAILED(Ready_Parts()))
	//	return E_FAIL;

	//if (FAILED(Ready_States()))
	//	return E_FAIL;

 //	if (FAILED(Ready_Colliders()))
	//	return E_FAIL;

	return S_OK;
}

void CCharacter_SwordMan::Tick(_float fTimeDelta)
{
	m_pStateCom->Tick_State(fTimeDelta);
	__super::Tick(fTimeDelta);

	_bool bKeyInput = false;
	if (KEY_HOLD(KEY::UP_ARROW))
	{
		bKeyInput = true;
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		bKeyInput = true;
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		m_pTransformCom->Move(-1.f * vLook, 10.f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f) * fTimeDelta);
		if (false == bKeyInput)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
			m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
		}
				
			
	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f) * fTimeDelta);
		if (false == bKeyInput)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
			m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
		}
		
	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_pRigidBodyCom->Add_Velocity(XMVectorSet(0.f, 1.f, 0.f, 0.f), 10.f, false);
	}
}

void CCharacter_SwordMan::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
}

HRESULT CCharacter_SwordMan::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CCharacter_SwordMan::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CCharacter_SwordMan::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CCharacter_SwordMan::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CCharacter_SwordMan::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}





HRESULT CCharacter_SwordMan::Ready_Components()
{

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Witch"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;



	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;
	//RigidDesc.pNavigation = m_pNavigationCom;
	//


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


	return S_OK;
}

#pragma region Ready_States
HRESULT CCharacter_SwordMan::Ready_States()
{
	//list<wstring> strAnimationName;

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AdvNut01_1");
	//m_pStateCom->Add_State(CCharacter::BASIC_IDLE, CState_Tanjiro_Basic_Idle::Create(m_pDevice, m_pContext, m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AdvJumpF01_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AdvJumpF01_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AdvJumpF01_2");
	//m_pStateCom->Add_State(CCharacter::BASIC_JUMP, CState_Tanjiro_Basic_Jump::Create(m_pDevice, m_pContext, m_pStateCom, strAnimationName));


	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AdvWalk01_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AdvRun01_1");
	//m_pStateCom->Add_State(CCharacter::BASIC_MOVE, CState_Tanjiro_Basic_Move::Create(m_pDevice, m_pContext, m_pStateCom, strAnimationName));




	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseNut01_1");
	//m_pStateCom->Add_State(CCharacter::BATTLE_IDLE, CState_Character_Battle_Idle::Create(m_pDevice, m_pContext, m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseRun01_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseDashF01_1");
	//m_pStateCom->Add_State(CCharacter::BATTLE_MOVE,
	//	CState_Character_Battle_Move::Create(m_pDevice, 
	//		m_pContext, 
	//		m_pStateCom, 
	//		strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseJump01_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseJump01_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseJump01_2");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseJump01_3");
	//m_pStateCom->Add_State(CCharacter::BATTLE_JUMP,
	//	CState_Character_Battle_Jump::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));


	//strAnimationName.clear();
	//// Left
	//// Right
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseStepL01");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseStepL02");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseStepR01");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseStepR02");

	//m_pStateCom->Add_State(CCharacter::BATTLE_DASH,
	//	CState_Character_Battle_Dash::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//// Left
	//// Right
	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseStepAL01");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseStepAR01");
	//m_pStateCom->Add_State(CCharacter::BATTLE_AIRDASH,
	//	CState_Character_Battle_AirDash::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));
	//




	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_Death");
	//m_pStateCom->Add_State(CCharacter::DIE,
	//	CState_Character_Dead::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));


	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_Dmg01_F");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_Dmg01_L");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_Dmg01_R");
	//m_pStateCom->Add_State(CCharacter::DAMAGED_BASIC,
	//	CState_Character_Damaged_Basic::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgBound01_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgBound02_2");
	//m_pStateCom->Add_State(CCharacter::DAMAGED_BOUND,
	//	CState_Character_Damaged_Bound::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgBlowF01_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgBlowF01_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgBlowF01_2");
	//m_pStateCom->Add_State(CCharacter::DAMAGED_BLOW,
	//	CState_Character_Damaged_Blow::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgUpperF01_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgUpperF01_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0000_V00_C00_DmgUpperF01_2");
	//m_pStateCom->Add_State(CCharacter::DAMAGED_AIRBORN,
	//	CState_Character_Damaged_AirBorn::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));


	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseTired01_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseTired01_1");
	//m_pStateCom->Add_State(CCharacter::KNOCKDOWN,
	//	CState_Character_Down::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));



	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbW01");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbW02");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbW03");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbW03D01");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbW04");

	//m_pStateCom->Add_State(CCharacter::ATTACK,
	//	CState_Tanjiro_Attack::Create(m_pDevice,
	//		m_pContext,			
	//		m_pStateCom,
	//		strAnimationName));


	//strAnimationName.clear();
	//
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_BaseJump01_2");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbAW01");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkCmbAW02");

	//m_pStateCom->Add_State(CCharacter::AIR_ATTACK,
	//	CState_Tanjiro_Air_Attack::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));


	//strAnimationName.clear();

	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl02_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl02_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl02_2");

	//m_pStateCom->Add_State(CCharacter::SKILL_0,
	//	CState_Tanjiro_Skill_0::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//strAnimationName.clear();

	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl03_0");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl03_1");
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl03_2");

	//m_pStateCom->Add_State(CCharacter::SKILL_1,
	//	CState_Tanjiro_Skill_1::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SK_P0001_V00_C00.ao|A_P0001_V00_C00_AtkSkl04");
	//m_pStateCom->Add_State(CCharacter::SKILL_2,
	//	CState_Tanjiro_Skill_2::Create(m_pDevice,
	//		m_pContext,
	//		m_pStateCom,
	//		strAnimationName));

	//

	//m_pStateCom->Change_State(CCharacter::BASIC_IDLE);
	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CCharacter_SwordMan::Ready_Colliders()
{

	//CCollider_Sphere::SPHERE_COLLIDER_DESC ColliderDesc;
	//ZeroMemory(&ColliderDesc, sizeof ColliderDesc);

	//BoundingSphere tSphere;
	//ZeroMemory(&tSphere, sizeof(BoundingSphere));
	//tSphere.Radius = 1.f;

	//XMStoreFloat4x4(&ColliderDesc.ModePivotMatrix, m_pModelCom->Get_PivotMatrix());
	//ColliderDesc.pOwnerTransform = m_pTransformCom;

	//ColliderDesc.tSphere = tSphere;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"Root");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 1.f, 0.f);
	//
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &ColliderDesc)))
	//	return E_FAIL;


	//ColliderDesc.tSphere.Radius = .2f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"C_TongueA_2");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);

	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::HEAD, &ColliderDesc)))
	//	return E_FAIL;

	//ColliderDesc.tSphere.Radius = .6f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"C_Spine_1");
	//ColliderDesc.vOffsetPosition = _float3(0.f, -0.25f, 0.f);
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::BODY, &ColliderDesc)))
	//	return E_FAIL;



	//ColliderDesc.tSphere.Radius = .1f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"L_Foot_End");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &ColliderDesc)))
	//	return E_FAIL;


	//ColliderDesc.tSphere.Radius = .1f;
	//ColliderDesc.pNode = m_pModelCom->Get_HierarchyNode(L"R_Foot_End");
	//ColliderDesc.vOffsetPosition = _float3(0.f, 0.f, 0.f);
	//if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::SPHERE, CCollider::DETECTION_TYPE::ATTACK, &ColliderDesc)))
	//	return E_FAIL;


	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CCharacter_SwordMan::Ready_Sockets()
{
	//if (nullptr == m_pModelCom)
	//	return E_FAIL;

	//m_Sockets.resize(SOCKET_END);

	//m_Sockets[SOCKET_SWORD] = m_pModelCom->Get_HierarchyNode(L"R_HandCommon_1_Lct");
	//m_Sockets[SOCKET_RIGHT_HAND] = m_pModelCom->Get_HierarchyNode(L"R_Hand_1");
	//m_Sockets[SOCKET_SWEATH] = m_pModelCom->Get_HierarchyNode(L"L_Weapon_1_Lct");

	//m_Sockets[SOCKET_LEFT_FOOT] = m_pModelCom->Get_HierarchyNode(L"L_Foot_End");
	//m_Sockets[SOCKET_RIGHT_FOOT] = m_pModelCom->Get_HierarchyNode(L"R_Foot_End");
	//

	//CTrail::TRAIL_DESC TrailDesc = {};
	//TrailDesc.bTrail = true;
	//TrailDesc.fAccGenTrail = 0.f;
	//TrailDesc.fGenTrailTime = 0.01f;
	//TrailDesc.vDiffuseColor = { 0.f, 0.5f, 1.f, 0.5f };


	//// LeftFoot
	//m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT] = CTrail::Create(m_pDevice, m_pContext, L"LeftFoot_Trail", TrailDesc);
	//if (m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT] == nullptr)
	//	return E_FAIL;

	//if (FAILED(m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT]->Initialize(nullptr)))
	//	return E_FAIL;

	//m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT]->SetUp_Position(XMVectorSet(0.f, 0.0f, -0.0125f, 1.f), XMVectorSet(0.f, 0.0f, 0.0125f, 1.f));
	//m_pTrails[SOCKET_TYPE::SOCKET_LEFT_FOOT]->Set_VtxCount(66);



	//// RightFoot
	//m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT] = CTrail::Create(m_pDevice, m_pContext, L"RightFoot_Trail", TrailDesc);
	//if (m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT] == nullptr)
	//	return E_FAIL;

	//if (FAILED(m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT]->Initialize(nullptr)))
	//	return E_FAIL;

	//m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT]->SetUp_Position(XMVectorSet(0.f, 0.0f, -0.0125f, 1.f), XMVectorSet(0.f, 0.0f, 0.0125f, 1.f));
	//m_pTrails[SOCKET_TYPE::SOCKET_RIGHT_FOOT]->Set_VtxCount(66);
	//



	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CCharacter_SwordMan::Ready_Parts()
{
	//m_Parts.resize(PARTTYPE::PART_END);

	//CSweath::SWEATH_DESC			SweathDesc;

	//SweathDesc.eType = CSweath::SWEATH_TYPE::TANJIRO;
	//SweathDesc.pOwner = this;
	//SweathDesc.pParentTransform = m_pTransformCom;
	//SweathDesc.pSocketBone = m_Sockets[SOCKET_SWEATH];
	//XMStoreFloat3(&SweathDesc.vRotationDegree, 
	//	XMVectorSet(XMConvertToRadians(-90.f), 
	//	XMConvertToRadians(180.f), 
	//	XMConvertToRadians(0.f), 
	//	XMConvertToRadians(0.f)));
	//XMStoreFloat4x4(&SweathDesc.SocketPivot, m_pModelCom->Get_PivotMatrix());

	//CGameObject* pGameObject = GI->Clone_GameObject(TEXT("Prototype_GameObject_Sweath_Tanjiro"), LAYER_TYPE::LAYER_CHARACTER, &SweathDesc);
	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//Safe_AddRef(pGameObject);
	//m_Parts[PART_SWEATH] = (pGameObject);


	//CSword::SWORD_DESC			SwordDesc;

	//SwordDesc.eType = CSword::SWORD_TYPE::TANJIRO;
	//SwordDesc.pOwner = this;
	//SwordDesc.pParentTransform = m_pTransformCom;
	//SwordDesc.pSocketBone = m_Sockets[SOCKET_SWEATH];
	//XMStoreFloat3(&SwordDesc.vRotationDegree,
	//	XMVectorSet(XMConvertToRadians(-90.f),
	//		XMConvertToRadians(180.f),
	//		XMConvertToRadians(0.f),
	//		XMConvertToRadians(0.f)));

	//XMStoreFloat4x4(&SwordDesc.SocketPivot, m_pModelCom->Get_PivotMatrix());


	//pGameObject = GI->Clone_GameObject(TEXT("Prototype_GameObject_Sword_Tanjiro"), LAYER_TYPE::LAYER_CHARACTER, &SwordDesc);
	//
	//if (nullptr == pGameObject)
	//	return E_FAIL;

	//Safe_AddRef(pGameObject);
	//m_Parts[PART_SWORD] = pGameObject;


	return S_OK;
}
#pragma endregion

CCharacter_SwordMan* CCharacter_SwordMan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType)
{
	CCharacter_SwordMan* pInstance = new CCharacter_SwordMan(pDevice, pContext, strObjectTag, eCharacterType);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CCharacter_SwordMan");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CCharacter_SwordMan::Clone(void* pArg)
{
	CCharacter_SwordMan* pInstance = new CCharacter_SwordMan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCharacter_SwordMan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacter_SwordMan::Free()
{
	__super::Free();
}

