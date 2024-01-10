#include "stdafx.h"
#include "BlackSmithMaster.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CBlackSmithMaster::CBlackSmithMaster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CBlackSmithMaster::CBlackSmithMaster(const CBlackSmithMaster& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CBlackSmithMaster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlackSmithMaster::Initialize(void* pArg)
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

void CBlackSmithMaster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBlackSmithMaster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CBlackSmithMaster::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CBlackSmithMaster::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CBlackSmithMaster::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CBlackSmithMaster::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CBlackSmithMaster::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01");
	strAnimationName.push_back(L"Stand01Idle01");
	strAnimationName.push_back(L"Stand04Idle01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand04Idle01");
	strAnimationName.push_back(L"Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"Stand01Idle01");
	strAnimationName.push_back(L"Stand04Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CBlackSmithMaster::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBlackSmithMaster::Ready_Sockets()
{
	return S_OK;
}

CBlackSmithMaster* CBlackSmithMaster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CBlackSmithMaster* pInstance = new CBlackSmithMaster(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CBlackSmithMaster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBlackSmithMaster::Clone(void* pArg)
{
	CBlackSmithMaster* pInstance = new CBlackSmithMaster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CBlackSmithMaster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBlackSmithMaster::Free()
{
	__super::Free();
}
