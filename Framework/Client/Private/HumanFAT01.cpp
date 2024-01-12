#include "stdafx.h"
#include "HumanFAT01.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include"UI_World_NPCTag.h"

CHumanFAT01::CHumanFAT01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFAT01::CHumanFAT01(const CHumanFAT01& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFAT01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFAT01::Initialize(void* pArg)
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
	m_pTag->Set_Owner(this, m_strKorName, 2.3f);

	return S_OK;
}

void CHumanFAT01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CHumanFAT01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFAT01::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFAT01::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFAT01::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFAT01::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFAT01::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Talk");
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_NeutralStand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Walk");
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_TALK);

	return S_OK;
}

HRESULT CHumanFAT01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFAT01::Ready_Sockets()
{
	return S_OK;
}

CHumanFAT01* CHumanFAT01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFAT01* pInstance = new CHumanFAT01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFAT01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFAT01::Clone(void* pArg)
{
	CHumanFAT01* pInstance = new CHumanFAT01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFAT01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFAT01::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}
