#include "stdafx.h"
#include "SwiftSolutionMaster.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

#include "UI_World_NPCTag.h"

CSwiftSolutionMaster::CSwiftSolutionMaster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CSwiftSolutionMaster::CSwiftSolutionMaster(const CSwiftSolutionMaster& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CSwiftSolutionMaster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSwiftSolutionMaster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("c31106000_p010_100001_std"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 2.2f, true);

	return S_OK;
}

void CSwiftSolutionMaster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CSwiftSolutionMaster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CSwiftSolutionMaster::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CSwiftSolutionMaster::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CSwiftSolutionMaster::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CSwiftSolutionMaster::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CSwiftSolutionMaster::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CSwiftSolutionMaster::Ready_States()
{
	m_strKorName = TEXT("ºñ¾îµå");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"c31106000_p010_100001_std");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"c31106000_p010_100201_tlk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"c31106000_p010_150001_wlk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SwiftSolutionMaster_run");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CSwiftSolutionMaster::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSwiftSolutionMaster::Ready_Sockets()
{
	return S_OK;
}

CSwiftSolutionMaster* CSwiftSolutionMaster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSwiftSolutionMaster* pInstance = new CSwiftSolutionMaster(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSwiftSolutionMaster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSwiftSolutionMaster::Clone(void* pArg)
{
	CSwiftSolutionMaster* pInstance = new CSwiftSolutionMaster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSwiftSolutionMaster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSwiftSolutionMaster::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}