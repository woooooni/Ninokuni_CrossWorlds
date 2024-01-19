#include "stdafx.h"
#include "Criminal_Npc.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"
#include "NpcCriminalState_Escape.h"
#include "NpcCriminalState_EscapeRun.h"
#include "NpcCriminalState_LockPick.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

CCriminal_Npc::CCriminal_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CCriminal_Npc::CCriminal_Npc(const CCriminal_Npc& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CCriminal_Npc::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCriminal_Npc::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 1.f);

	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBalloon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
	m_pBalloon->Set_Owner(this, 1.2f);
	m_pBalloon->Set_Balloon(TEXT(""));

	return S_OK;
}

void CCriminal_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
	if (nullptr != m_pBalloon)
		m_pBalloon->Tick(fTimeDelta);
}

void CCriminal_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);
	if (nullptr != m_pBalloon)
		m_pBalloon->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CCriminal_Npc::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CCriminal_Npc::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CCriminal_Npc::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CCriminal_Npc::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CCriminal_Npc::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	OBJECT_INIT_DESC Init_Data = {};

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -40.f, -6.75f, 34.7f, 1.f });
	m_pTransformCom->FixRotation(0.f, -160.f, 0.f);

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Criminal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
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
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);
	//m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Vec4(-10.0f, 50.0f, 0.0f, 1.0f));
	return S_OK;
}

HRESULT CCriminal_Npc::Ready_States()
{
	m_tStat.fSpeed = 7.2f;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Idle");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Fun");
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Fun");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Run");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Run");
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Fun");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Escape");
	m_pStateCom->Add_State(NPC_CRIMINAL_ESCAPE, CNpcCriminalState_Escape::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|Run");
	m_pStateCom->Add_State(NPC_CRIMINAL_ESCAPERUN, CNpcCriminalState_EscapeRun::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_BlackCircleAgent.ao|LockPick");
	m_pStateCom->Add_State(NPC_CRIMINAL_LOCKPICK, CNpcCriminalState_LockPick::Create(m_pStateCom, strAnimationName));


	m_vecRoaming.push_back({ -47.f, -6.75f, 40.f, 1.0f });
	m_vecRoaming.push_back({ -72.f, -5.f, 52.f, 1.0f });
	m_vecRoaming.push_back({ -92.f, -5.f, 39.f, 1.0f });
	m_vecRoaming.push_back({ -88.f, -2.7f, -4.5f, 1.0f });
	m_vecRoaming.push_back({ -100.f, -5.f, -14.f, 1.0f });
	m_vecRoaming.push_back({ -89.f, -7.6f, -36.f, 1.0f });
	m_vecRoaming.push_back({ -57.f, -5.3f, -35.5f, 1.0f });
	m_vecRoaming.push_back({ -37.f, -5.3f, -44.5f, 1.0f });

	m_pStateCom->Change_State(NPC_CRIMINAL_LOCKPICK);
	m_pModelCom->Set_Animation(TEXT("SKM_BlackCircleAgent.ao|LockPick"));

	return S_OK;
}

HRESULT CCriminal_Npc::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCriminal_Npc::Ready_Sockets()
{
	return S_OK;
}

CCriminal_Npc* CCriminal_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCriminal_Npc* pInstance = new CCriminal_Npc(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CCriminal_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCriminal_Npc::Clone(void* pArg)
{
	CCriminal_Npc* pInstance = new CCriminal_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCriminal_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCriminal_Npc::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pBalloon);
}