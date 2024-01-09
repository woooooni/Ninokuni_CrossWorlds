#include "stdafx.h"
#include "HumanChildHalloweenA.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanChildHalloweenA::CHumanChildHalloweenA(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanChildHalloweenA::CHumanChildHalloweenA(const CHumanChildHalloweenA& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanChildHalloweenA::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChildHalloweenA::Initialize(void* pArg)
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

void CHumanChildHalloweenA::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanChildHalloweenA::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanChildHalloweenA::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanChildHalloweenA::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanChildHalloweenA::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanChildHalloweenA::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanChildHalloweenA::Ready_States()
{
	m_tStat.fSpeed = 4.f;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Stand");
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Success");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Success");
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Run");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Run");
	strAnimationName.push_back(L"SKM_HumanChildHalloweenA.ao|HumanChildHalloweenA_Success");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_vecRoaming.push_back({ 10.f, 0.f, -10.f, 1.f });
	m_vecRoaming.push_back({ 10.f, 0.f, 10.f, 1.f });
	m_vecRoaming.push_back({ -10.f, 0.f, 10.f, 1.f });

	m_pStateCom->Change_State(NPC_MOVE_ONEWAY);

	return S_OK;
}

HRESULT CHumanChildHalloweenA::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChildHalloweenA::Ready_Sockets()
{
	return S_OK;
}

CHumanChildHalloweenA* CHumanChildHalloweenA::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanChildHalloweenA* pInstance = new CHumanChildHalloweenA(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanChildHalloweenA");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanChildHalloweenA::Clone(void* pArg)
{
	CHumanChildHalloweenA* pInstance = new CHumanChildHalloweenA(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanChildHalloweenA");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanChildHalloweenA::Free()
{
	__super::Free();
}