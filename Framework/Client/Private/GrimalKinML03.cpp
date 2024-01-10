#include "stdafx.h"
#include "GrimalKinML03.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CGrimalKinML03::CGrimalKinML03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGrimalKinML03::CGrimalKinML03(const CGrimalKinML03& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGrimalKinML03::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalKinML03::Initialize(void* pArg)
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

void CGrimalKinML03::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGrimalKinML03::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGrimalKinML03::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML03::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML03::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML03::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrimalKinML03::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01");
	strAnimationName.push_back(L"Stand02Idle01");
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand03Idle01");
	strAnimationName.push_back(L"Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CGrimalKinML03::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalKinML03::Ready_Sockets()
{
	return S_OK;
}

CGrimalKinML03* CGrimalKinML03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrimalKinML03* pInstance = new CGrimalKinML03(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGrimalKinML03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrimalKinML03::Clone(void* pArg)
{
	CGrimalKinML03* pInstance = new CGrimalKinML03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGrimalKinML03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrimalKinML03::Free()
{
	__super::Free();
}
