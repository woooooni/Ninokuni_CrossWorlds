#include "stdafx.h"
#include "HumanMM05.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanMM05::CHumanMM05(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanMM05::CHumanMM05(const CHumanMM05& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanMM05::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM05::Initialize(void* pArg)
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

void CHumanMM05::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanMM05::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanMM05::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanMM05::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanMM05::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanMM05::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanMM05::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01");
	strAnimationName.push_back(L"Stand02Idle01");
	strAnimationName.push_back(L"StandIdle01Idle02");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"StandIdle01Idle02");
	strAnimationName.push_back(L"Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Walk01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Walk01");
	strAnimationName.push_back(L"StandIdle01Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanMM05::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM05::Ready_Sockets()
{
	return S_OK;
}

CHumanMM05* CHumanMM05::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanMM05* pInstance = new CHumanMM05(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanMM05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanMM05::Clone(void* pArg)
{
	CHumanMM05* pInstance = new CHumanMM05(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanMM05");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanMM05::Free()
{
	__super::Free();
}
