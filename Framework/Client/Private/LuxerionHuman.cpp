#include "stdafx.h"
#include "LuxerionHuman.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CLuxerionHuman::CLuxerionHuman(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CLuxerionHuman::CLuxerionHuman(const CLuxerionHuman& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CLuxerionHuman::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLuxerionHuman::Initialize(void* pArg)
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
	m_pTag->Set_Owner(this, m_strKorName, 2.2f);

	return S_OK;
}

void CLuxerionHuman::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CLuxerionHuman::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CLuxerionHuman::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CLuxerionHuman::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CLuxerionHuman::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CLuxerionHuman::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CLuxerionHuman::Ready_States()
{
	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasMove = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_CSToGuardian01");
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_Hello");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_talk");
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_CSToGuardian01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_CSToGuardian01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_CSToGuardian01");
	strAnimationName.push_back(L"SKM_LuxerionHuman.ao|LuxerionHuman_CSToGuardian01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CLuxerionHuman::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLuxerionHuman::Ready_Sockets()
{
	return S_OK;
}

CLuxerionHuman* CLuxerionHuman::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CLuxerionHuman* pInstance = new CLuxerionHuman(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CLuxerionHuman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLuxerionHuman::Clone(void* pArg)
{
	CLuxerionHuman* pInstance = new CLuxerionHuman(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CLuxerionHuman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLuxerionHuman::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}