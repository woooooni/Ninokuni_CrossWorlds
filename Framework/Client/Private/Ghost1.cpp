#include "stdafx.h"
#include "Ghost1.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CGhost1::CGhost1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGhost1::CGhost1(const CGhost1& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGhost1::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost1::Initialize(void* pArg)
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
	m_pTag->Set_Owner(this, m_strKorName, 2.f);

	return S_OK;
}

void CGhost1::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CGhost1::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGhost1::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGhost1::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGhost1::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGhost1::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CGhost1::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	// SKM_RunnerCat.ao|RunnerCat_Stand
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Stand");
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Idle01");
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Idle02");
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Hello");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Talk");
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Walk");
	strAnimationName.push_back(L"SKM_Ghost1.ao|Ghost_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CGhost1::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost1::Ready_Sockets()
{
	return S_OK;
}

CGhost1* CGhost1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGhost1* pInstance = new CGhost1(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGhost1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost1::Clone(void* pArg)
{
	CGhost1* pInstance = new CGhost1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGhost1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost1::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}