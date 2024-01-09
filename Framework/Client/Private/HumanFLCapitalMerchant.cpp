#include "stdafx.h"
#include "HumanFLCapitalMerchant.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanFLCapitalMerchant::CHumanFLCapitalMerchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFLCapitalMerchant::CHumanFLCapitalMerchant(const CHumanFLCapitalMerchant& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFLCapitalMerchant::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFLCapitalMerchant::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CHumanFLCapitalMerchant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanFLCapitalMerchant::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFLCapitalMerchant::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFLCapitalMerchant::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFLCapitalMerchant::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFLCapitalMerchant::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFLCapitalMerchant::Ready_States()
{
	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasMove = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFLCapitalMerchant.ao|CapitalMerchant_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFLCapitalMerchant.ao|CapitalMerchant_Talk");
	strAnimationName.push_back(L"SKM_HumanFLCapitalMerchant.ao|CapitalMerchant_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFLCapitalMerchant.ao|CapitalMerchant_Stand");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFLCapitalMerchant.ao|CapitalMerchant_Stand");
	strAnimationName.push_back(L"SKM_HumanFLCapitalMerchant.ao|CapitalMerchant_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanFLCapitalMerchant::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFLCapitalMerchant::Ready_Sockets()
{
	return S_OK;
}

CHumanFLCapitalMerchant* CHumanFLCapitalMerchant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFLCapitalMerchant* pInstance = new CHumanFLCapitalMerchant(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFLCapitalMerchant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFLCapitalMerchant::Clone(void* pArg)
{
	CHumanFLCapitalMerchant* pInstance = new CHumanFLCapitalMerchant(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFLCapitalMerchant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFLCapitalMerchant::Free()
{
	__super::Free();
}