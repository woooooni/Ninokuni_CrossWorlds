#include "stdafx.h"
#include "Ruslan.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

CRuslan::CRuslan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CRuslan::CRuslan(const CRuslan& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CRuslan::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRuslan::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Ruslan.ao|Ruslan_NeutralStand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;


	return S_OK;
}

void CRuslan::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CRuslan::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CRuslan::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CRuslan::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CRuslan::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CRuslan::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CRuslan::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CRuslan::Ready_States()
{
	m_strKorName = TEXT("·ç½½¶õ");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruslan.ao|Ruslan_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruslan.ao|Ruslan_CSTalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruslan.ao|Ruslan_NeutralWalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ruslan.ao|Ruslan_NeutralRun");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CRuslan::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRuslan::Ready_Sockets()
{
	return S_OK;
}

CRuslan* CRuslan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CRuslan* pInstance = new CRuslan(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CRuslan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CRuslan::Clone(void* pArg)
{
	CRuslan* pInstance = new CRuslan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CRuslan");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CRuslan::Free()
{
	__super::Free();
}