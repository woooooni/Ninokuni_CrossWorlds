#include "stdafx.h"
#include "TreeGrandfa.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

CTreeGrandfa::CTreeGrandfa(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CTreeGrandfa::CTreeGrandfa(const CTreeGrandfa& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CTreeGrandfa::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeGrandfa::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_TreeGrandfa.ao|TreeGrandfa_Stand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CTreeGrandfa::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CTreeGrandfa::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CTreeGrandfa::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CTreeGrandfa::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CTreeGrandfa::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CTreeGrandfa::Ready_States()
{
	m_strKorName = TEXT("가을 영감");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Talk01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Talk01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_TreeGrandfa.ao|TreeGrandfa_Talk01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CTreeGrandfa::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTreeGrandfa::Ready_Sockets()
{
	return S_OK;
}

CTreeGrandfa* CTreeGrandfa::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CTreeGrandfa* pInstance = new CTreeGrandfa(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CTreeGrandfa");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTreeGrandfa::Clone(void* pArg)
{
	CTreeGrandfa* pInstance = new CTreeGrandfa(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CTreeGrandfa");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTreeGrandfa::Free()
{
	__super::Free();
}