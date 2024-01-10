#include "stdafx.h"
#include "Edellian.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

CEdellian::CEdellian(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CEdellian::CEdellian(const CEdellian& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CEdellian::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEdellian::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Edellian.ao|Edellian_BattleStand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;


	return S_OK;
}

void CEdellian::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEdellian::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CEdellian::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CEdellian::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CEdellian::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CEdellian::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CEdellian::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CEdellian::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Edellian.ao|Edellian_BattleStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Edellian.ao|Edellian_Skill01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Edellian.ao|Edellian_BattleWalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Edellian.ao|Edellian_BattleRun");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CEdellian::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEdellian::Ready_Sockets()
{
	return S_OK;
}

CEdellian* CEdellian::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CEdellian* pInstance = new CEdellian(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CEdellian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEdellian::Clone(void* pArg)
{
	CEdellian* pInstance = new CEdellian(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CEdellian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEdellian::Free()
{
	__super::Free();
}