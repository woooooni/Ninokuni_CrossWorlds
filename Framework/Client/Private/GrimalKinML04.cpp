#include "stdafx.h"
#include "GrimalKinML04.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CGrimalKinML04::CGrimalKinML04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGrimalKinML04::CGrimalKinML04(const CGrimalKinML04& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGrimalKinML04::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalKinML04::Initialize(void* pArg)
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

void CGrimalKinML04::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGrimalKinML04::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGrimalKinML04::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML04::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML04::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGrimalKinML04::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrimalKinML04::Ready_States()
{
	m_strKorName = TEXT("경비 장교");
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

HRESULT CGrimalKinML04::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrimalKinML04::Ready_Sockets()
{
	return S_OK;
}

CGrimalKinML04* CGrimalKinML04::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrimalKinML04* pInstance = new CGrimalKinML04(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGrimalKinML04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrimalKinML04::Clone(void* pArg)
{
	CGrimalKinML04* pInstance = new CGrimalKinML04(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGrimalKinML04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrimalKinML04::Free()
{
	__super::Free();
}
