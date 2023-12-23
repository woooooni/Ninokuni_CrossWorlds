#include "stdafx.h"
#include "Character_SwordMan.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"
#include "Part_Manager.h"


#include "State_SwordMan_Neutral_Idle.h"
#include "State_SwordMan_Neutral_Jump.h"
#include "State_SwordMan_Neutral_Walk.h"
#include "State_SwordMan_Neutral_Run.h"

#include "State_SwordMan_Neutral_Crouch_Idle.h"
#include "State_SwordMan_Neutral_Crouch_Move.h"



#include "State_SwordMan_Battle_Attack.h"
#include "State_SwordMan_Battle_Guard.h"
#include "State_SwordMan_Battle_Idle.h"
#include "State_SwordMan_Battle_Jump.h"
#include "State_SwordMan_Battle_Walk.h"
#include "State_SwordMan_Battle_Run.h"



#include "State_SwordMan_Dead.h"



CCharacter_SwordMan::CCharacter_SwordMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CCharacter(pDevice, pContext, strObjectTag, CHARACTER_TYPE::SWORD_MAN)
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

	if (FAILED(Ready_States()))
		return E_FAIL;

 //	if (FAILED(Ready_Colliders()))
	//	return E_FAIL;

	return S_OK;
}

void CCharacter_SwordMan::Tick(_float fTimeDelta)
{
	m_pStateCom->Tick_State(fTimeDelta);

	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	m_pControllerCom->Tick_Controller(fTimeDelta);
	__super::Tick(fTimeDelta);
}

void CCharacter_SwordMan::LateTick(_float fTimeDelta)
{
	
	m_pControllerCom->LateTick_Controller(fTimeDelta);
	__super::LateTick(fTimeDelta);
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordMan_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;



	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 2.5f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;


	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
		m_pCharacterPartModels[i] = CPart_Manager::GetInstance()->Get_PartModel(m_eCharacterType, PART_TYPE(i), 0);

	
	m_pModelCom->Set_Animation(0);

	return S_OK;
}

#pragma region Ready_States
HRESULT CCharacter_SwordMan::Ready_States()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralStand");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralIdle01");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralIdle02");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_IDLE, CState_SwordMan_Neutral_Idle::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralWalk");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_WALK, CState_SwordMan_Neutral_Walk::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_NeutralRun");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_RUN, CState_SwordMan_Neutral_Run::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_JumpUpLoop");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_JUMP, CState_SwordMan_Neutral_Jump::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CrouchStand");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CrouchIdle01");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_CROUCH_IDLE, CState_SwordMan_Neutral_Crouch_Idle::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CrouchWalk");
	m_pStateCom->Add_State(CCharacter::STATE::NEUTRAL_CROUCH_MOVE, CState_SwordMan_Neutral_Crouch_Move::Create(m_pStateCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Death");
	m_pStateCom->Add_State(CCharacter::STATE::DEAD, CState_SwordMan_Dead::Create(m_pStateCom, strAnimationNames));



	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_CSBattleStand");
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_FinishCombat");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_IDLE, CState_SwordMan_Battle_Idle::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_BattleRun");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_WALK, CState_SwordMan_Battle_Walk::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_Dash");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_RUN, CState_SwordMan_Battle_Run::Create(m_pStateCom, strAnimationNames));


	strAnimationNames.clear();	
	strAnimationNames.push_back(L"SKM_Swordsman_Merge.ao|Swordsman_JumpUpLoop");
	m_pStateCom->Add_State(CCharacter::STATE::BATTLE_JUMP, CState_SwordMan_Battle_Jump::Create(m_pStateCom, strAnimationNames));


	
	

	m_pStateCom->Change_State(CCharacter::NEUTRAL_IDLE);
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

	//XMStoreFloat4x4(&ColliderDesc.ModelPivotMatrix, m_pModelCom->Get_PivotMatrix());
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

CCharacter_SwordMan* CCharacter_SwordMan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCharacter_SwordMan* pInstance = new CCharacter_SwordMan(pDevice, pContext, strObjectTag);
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

