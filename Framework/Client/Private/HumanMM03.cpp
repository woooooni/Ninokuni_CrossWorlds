#include "stdafx.h"
#include "HumanMM03.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CHumanMM03::CHumanMM03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanMM03::CHumanMM03(const CHumanMM03& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanMM03::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM03::Initialize(void* pArg)
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

void CHumanMM03::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanMM03::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanMM03::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanMM03::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanMM03::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanMM03::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanMM03::Ready_States()
{
	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasMove = false;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSHeroOfEvermore01");
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM_03_CSCheer2");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM_03_CSCheer2");
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM_03_CSCheer2");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSHopeOfNewPeriod01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSHopeOfNewPeriod01");
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSMechaRagonAppear02");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanMM03::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM03::Ready_Sockets()
{
	return S_OK;
}

CHumanMM03* CHumanMM03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanMM03* pInstance = new CHumanMM03(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanMM03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanMM03::Clone(void* pArg)
{
	CHumanMM03* pInstance = new CHumanMM03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanMM03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanMM03::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}