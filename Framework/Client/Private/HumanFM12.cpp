#include "stdafx.h"
#include "HumanFM12.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanFM12::CHumanFM12(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFM12::CHumanFM12(const CHumanFM12& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFM12::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFM12::Initialize(void* pArg)
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

void CHumanFM12::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanFM12::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFM12::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFM12::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFM12::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFM12::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFM12::Ready_States()
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

HRESULT CHumanFM12::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFM12::Ready_Sockets()
{
	return S_OK;
}

CHumanFM12* CHumanFM12::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFM12* pInstance = new CHumanFM12(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFM12");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFM12::Clone(void* pArg)
{
	CHumanFM12* pInstance = new CHumanFM12(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFM12");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFM12::Free()
{
	__super::Free();
}
