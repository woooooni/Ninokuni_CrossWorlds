#include "stdafx.h"
#include "HumanMSCrossFieldMerchant.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanMSCrossFieldMerchant::CHumanMSCrossFieldMerchant(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanMSCrossFieldMerchant::CHumanMSCrossFieldMerchant(const CHumanMSCrossFieldMerchant& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanMSCrossFieldMerchant::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMSCrossFieldMerchant::Initialize(void* pArg)
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

void CHumanMSCrossFieldMerchant::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanMSCrossFieldMerchant::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanMSCrossFieldMerchant::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanMSCrossFieldMerchant::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanMSCrossFieldMerchant::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanMSCrossFieldMerchant::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CHumanMSCrossFieldMerchant::Ready_States()
{
	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Stand");
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Idle01");
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Idle02");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Idle02");
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Walk");
	strAnimationName.push_back(L"SKM_HumanMSCrossfieldMerchant.ao|HumanMSCrossfieldMerchant_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanMSCrossFieldMerchant::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMSCrossFieldMerchant::Ready_Sockets()
{
	return S_OK;
}

CHumanMSCrossFieldMerchant* CHumanMSCrossFieldMerchant::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanMSCrossFieldMerchant* pInstance = new CHumanMSCrossFieldMerchant(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanMSCrossFieldMerchant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanMSCrossFieldMerchant::Clone(void* pArg)
{
	CHumanMSCrossFieldMerchant* pInstance = new CHumanMSCrossFieldMerchant(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanMSCrossFieldMerchant");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanMSCrossFieldMerchant::Free()
{
	__super::Free();
}