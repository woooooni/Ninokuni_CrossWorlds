#include "stdafx.h"
#include "GrimalKinML02.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CGrimalKinML02::CGrimalKinML02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGrimalKinML02::CGrimalKinML02(const CGrimalKinML02& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGrimalKinML02::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalKinML02::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
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
	m_pTag->Set_Owner(this, m_strKorName, 2.4f);

	return S_OK;
}

void CGrimalKinML02::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CGrimalKinML02::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGrimalKinML02::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML02::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML02::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML02::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrimalKinML02::Ready_States()
{
	m_strKorName = TEXT("»÷½¼");

	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01");
	strAnimationName.push_back(L"Stand02Idle01");
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand03Idle01");
	strAnimationName.push_back(L"Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CGrimalKinML02::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalKinML02::Ready_Sockets()
{
	return S_OK;
}

CGrimalKinML02* CGrimalKinML02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrimalKinML02* pInstance = new CGrimalKinML02(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGrimalKinML02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrimalKinML02::Clone(void* pArg)
{
	CGrimalKinML02* pInstance = new CGrimalKinML02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGrimalKinML02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrimalKinML02::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}
