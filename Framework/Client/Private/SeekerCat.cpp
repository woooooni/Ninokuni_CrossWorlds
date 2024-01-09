#include "stdafx.h"
#include "SeekerCat.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CSeekerCat::CSeekerCat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CSeekerCat::CSeekerCat(const CSeekerCat& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CSeekerCat::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerCat::Initialize(void* pArg)
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

void CSeekerCat::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSeekerCat::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CSeekerCat::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CSeekerCat::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CSeekerCat::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CSeekerCat::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CSeekerCat::Ready_States()
{
	m_tStat.fSpeed = 2.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerCat_Stand");
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerCat_Idle01");
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerKing_Hello01");
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerKing_Confuse");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerKing_Talk01");
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerCat_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	// SKM_SeekerCat.ao|SeekerCat_Walk
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerCat_Run");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerCat_Run");
	strAnimationName.push_back(L"SKM_SeekerCat.ao|SeekerCat_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CSeekerCat::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerCat::Ready_Sockets()
{
	return S_OK;
}

CSeekerCat* CSeekerCat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSeekerCat* pInstance = new CSeekerCat(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSeekerCat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSeekerCat::Clone(void* pArg)
{
	CSeekerCat* pInstance = new CSeekerCat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSeekerCat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSeekerCat::Free()
{
	__super::Free();
}