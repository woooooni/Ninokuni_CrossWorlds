#include "stdafx.h"
#include "Ruby.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"
#include "UniqueNpcState_Seat.h"

#include "UI_World_NPCTag.h"

#include "RubyCarriage.h"

CRuby::CRuby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CRuby::CRuby(const CRuby& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CRuby::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRuby::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Ruby.ao|Ruby_NeutralStand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 2.f, true);

	// LEVEL_TOOL or LEVEL_WITCH
	list<CGameObject*>& pGameObjects = GI->Find_GameObjects(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_DYNAMIC);

	for (auto& pObj : pGameObjects)
	{
		if (pObj->Get_ObjectTag() == TEXT("Ruby_Carriage"))
		{
			m_pRidingObject = pObj;
			break;
		}
	}
	

	return S_OK;
}

void CRuby::Tick(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::SHIFT) && KEY_HOLD(KEY::V))
	{
		// TEST
		if (nullptr != m_pRidingObject)
		{
			CRubyCarriage* pRubyCarriage = static_cast<CRubyCarriage*>(m_pRidingObject);
			pRubyCarriage->Set_TakeTheCarriage(true);

			m_pStateCom->Change_State(NPC_STATE::NPC_UNIQUENPC_SEAT);		
		}
	}

	if (KEY_HOLD(KEY::SHIFT) && KEY_HOLD(KEY::B))
	{
		// TEST
		if (nullptr != m_pRidingObject)
		{
			CRubyCarriage* pRubyCarriage = static_cast<CRubyCarriage*>(m_pRidingObject);
			pRubyCarriage->Set_TakeTheCarriage(true);

			_bool bTakeCarriage = pRubyCarriage->TakeTheCarriage();
			if (true == bTakeCarriage)
				Set_QuestSection(ESCORT_SECTION::SECTION1, false);
		}
	}

	if (KEY_HOLD(KEY::SHIFT) && KEY_HOLD(KEY::N))
	{
		// TEST
		if (nullptr != m_pRidingObject)
		{
			CRubyCarriage* pRubyCarriage = static_cast<CRubyCarriage*>(m_pRidingObject);
			pRubyCarriage->Set_TakeTheCarriage(true);

			_bool bTakeCarriage = pRubyCarriage->TakeTheCarriage();
			if (true == bTakeCarriage)
				Set_QuestSection(ESCORT_SECTION::SECTION2, false);
		}
	}

	__super::Tick(fTimeDelta);



	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CRuby::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CRuby::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CRuby::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CRuby::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CRuby::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CRuby::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CRuby::Ready_States()
{
	m_strKorName = TEXT("·çºñ");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_BattleRun");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_BattleWalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Damage");
	//m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Death");
	//m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Hello");
	//m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Idle01");
	//m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Idle02");
	//m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Stun");
	//m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Talk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	//strAnimationName.clear();
	//strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Attack01");
	//m_pStateCom->Add_State(NPC_UNIQUENPC_SEAT, CUniqueNpcState_Seat::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruby.ao|Ruby_Seat");
	m_pStateCom->Add_State(NPC_UNIQUENPC_SEAT, CUniqueNpcState_Seat::Create(m_pStateCom, strAnimationName));


	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CRuby::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRuby::Ready_Sockets()
{
	return S_OK;
}

CRuby* CRuby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CRuby* pInstance = new CRuby(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CRuby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRuby::Clone(void* pArg)
{
	CRuby* pInstance = new CRuby(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CRuby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuby::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}