#include "stdafx.h"
#include "Aren.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

CAren::CAren(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CAren::CAren(const CAren& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CAren::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAren::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Aren.ao|Aren_Stand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CAren::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CAren::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CAren::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CAren::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CAren::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CAren::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CAren::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CAren::Ready_States()
{
	m_strKorName = TEXT("¾Æ·»");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Aren.ao|Aren_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Aren.ao|Aren_Talk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Aren.ao|Aren_Walk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Aren.ao|Aren_Run");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CAren::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAren::Ready_Sockets()
{
	return S_OK;
}

CAren* CAren::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CAren* pInstance = new CAren(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CAren");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAren::Clone(void* pArg)
{
	CAren* pInstance = new CAren(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CAren");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAren::Free()
{
	__super::Free();
}