#include "stdafx.h"
#include "HumanFM09.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CHumanFM09::CHumanFM09(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFM09::CHumanFM09(const CHumanFM09& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFM09::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFM09::Initialize(void* pArg)
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

void CHumanFM09::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanFM09::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFM09::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFM09::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFM09::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFM09::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFM09::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand0");
	strAnimationName.push_back(L"Stand01Idle01");
	strAnimationName.push_back(L"Stand04Idle01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	strAnimationName.push_back(L"Stand0");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand04Idle01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand04Idle01");
	strAnimationName.push_back(L"Stand04Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanFM09::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFM09::Ready_Sockets()
{
	return S_OK;
}

CHumanFM09* CHumanFM09::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFM09* pInstance = new CHumanFM09(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFM09");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFM09::Clone(void* pArg)
{
	CHumanFM09* pInstance = new CHumanFM09(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFM09");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFM09::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}
