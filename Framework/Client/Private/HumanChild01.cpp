#include "stdafx.h"
#include "HumanChild01.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

CHumanChild01::CHumanChild01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanChild01::CHumanChild01(const CHumanChild01& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanChild01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChild01::Initialize(void* pArg)
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
	m_pTag->Set_Owner(this, m_strKorName, 1.f);

	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBalloon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
	m_pBalloon->Set_Owner(this, 1.2f);
	m_pBalloon->Set_Balloon(TEXT("¾ö¸¾ ¸¾¸¾¸¾¸¾¸¾¸¶¹ò¹ò "));

	return S_OK;
}

void CHumanChild01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
	if (nullptr != m_pBalloon)
		m_pBalloon->Tick(fTimeDelta);
}

void CHumanChild01::LateTick(_float fTimeDelta)
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

void CHumanChild01::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanChild01::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanChild01::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanChild01::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanChild01::Ready_States()
{
	m_tStat.fSpeed = 5.f;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch02");
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CS1stAnniversary01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CS1stAnniversary01");
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));


	m_vecRoaming.push_back({ 0.f, 0.f, 10.f ,1.0f});
	m_vecRoaming.push_back({ -10.f, 0.f, 0.f, 1.0f });
	m_vecRoaming.push_back({ 0.f, 0.f, -10.f, 1.0f });

	// m_pStateCom->Change_State(NPC_IDLE);
	m_pStateCom->Change_State(NPC_MOVE_ONEWAY);

	return S_OK;
}

HRESULT CHumanChild01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChild01::Ready_Sockets()
{
	return S_OK;
}

CHumanChild01* CHumanChild01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanChild01* pInstance = new CHumanChild01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanChild01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanChild01::Clone(void* pArg)
{
	CHumanChild01* pInstance = new CHumanChild01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanChild01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanChild01::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pBalloon);
}