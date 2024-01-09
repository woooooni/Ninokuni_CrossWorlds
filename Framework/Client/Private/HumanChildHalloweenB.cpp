#include "stdafx.h"
#include "HumanChildHalloweenB.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanChildHalloweenB::CHumanChildHalloweenB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanChildHalloweenB::CHumanChildHalloweenB(const CHumanChildHalloweenB& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanChildHalloweenB::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChildHalloweenB::Initialize(void* pArg)
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

void CHumanChildHalloweenB::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanChildHalloweenB::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanChildHalloweenB::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanChildHalloweenB::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanChildHalloweenB::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanChildHalloweenB::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanChildHalloweenB::Ready_States()
{
	m_tStat.fSpeed = 4.f;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Stand");
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Success");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Success");
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Run");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Run");
	strAnimationName.push_back(L"SKM_HumanChildHalloweenB.ao|HumanChildHalloweenB_Success");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_vecRoaming.push_back({ 10.f, 0.f, 10.f, 1.f });
	m_vecRoaming.push_back({ -10.f, 0.f, 10.f, 1.f });
	m_vecRoaming.push_back({ -10.f, 0.f, -10.f, 1.f });

	m_pStateCom->Change_State(NPC_MOVE_ONEWAY);

	return S_OK;
}

HRESULT CHumanChildHalloweenB::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChildHalloweenB::Ready_Sockets()
{
	return S_OK;
}

CHumanChildHalloweenB* CHumanChildHalloweenB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanChildHalloweenB* pInstance = new CHumanChildHalloweenB(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanChildHalloweenB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanChildHalloweenB::Clone(void* pArg)
{
	CHumanChildHalloweenB* pInstance = new CHumanChildHalloweenB(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanChildHalloweenB");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanChildHalloweenB::Free()
{
	__super::Free();
}