#include "stdafx.h"
#include "HumanFL04.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CHumanFL04::CHumanFL04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFL04::CHumanFL04(const CHumanFL04& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFL04::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFL04::Initialize(void* pArg)
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

void CHumanFL04::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanFL04::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFL04::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFL04::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFL04::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFL04::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFL04::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;
	
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Stand");
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Hello");
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_CSPurifiedForestOfNiall");
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Idle01");
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Idle02");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Talk");
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Walk");
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanFL04::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFL04::Ready_Sockets()
{
	return S_OK;
}

CHumanFL04* CHumanFL04::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFL04* pInstance = new CHumanFL04(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFL04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFL04::Clone(void* pArg)
{
	CHumanFL04* pInstance = new CHumanFL04(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFL04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFL04::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}
