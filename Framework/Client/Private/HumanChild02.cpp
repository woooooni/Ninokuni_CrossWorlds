#include "stdafx.h"
#include "HumanChild02.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

CHumanChild02::CHumanChild02(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanChild02::CHumanChild02(const CHumanChild02& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanChild02::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChild02::Initialize(void* pArg)
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

	return S_OK;
}

void CHumanChild02::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanChild02::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanChild02::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanChild02::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanChild02::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanChild02::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanChild02::Ready_States()
{
	m_tStat.fSpeed = 0.25f;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild02.ao|HumanChild_IdleSleepNofacial");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild02.ao|HumanChild_IdleSleepNofacial");
	strAnimationName.push_back(L"SKM_HumanChild02.ao|HumanChild_IdleSleepNofacial");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild02.ao|HumanChild_WalkSleepNoFacial");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild02.ao|HumanChild_WalkSleepNoFacial");
	strAnimationName.push_back(L"SKM_HumanChild02.ao|HumanChild_IdleSleepNofacial");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanChild02::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChild02::Ready_Sockets()
{
	return S_OK;
}

CHumanChild02* CHumanChild02::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanChild02* pInstance = new CHumanChild02(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanChild02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanChild02::Clone(void* pArg)
{
	CHumanChild02* pInstance = new CHumanChild02(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanChild02");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanChild02::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}