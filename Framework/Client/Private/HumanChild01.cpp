#include "stdafx.h"
#include "HumanChild01.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanChild01::CHumanChild01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanChild01::CHumanChild01(const CHumanChild01& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanChild01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChild01::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CHumanChild01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanChild01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanChild01::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanChild01::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanChild01::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanChild01::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanChild01::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 10.f, 0.f, 0.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();
	m_vecRoaming.push_back(m_vInitPos);

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_HumanChild01"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CHumanChild01::Ready_States()
{
	m_strObjectTag = TEXT("HumanChild01");

	m_tStat.fSpeed = 5.f;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch02");
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CS1stAnniversary01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CS1stAnniversary01");
	strAnimationName.push_back(L"SKM_HumanChild01.ao|HumanChild01_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));


	m_vecRoaming.push_back({ 0.f, 0.f, 10.f });
	m_vecRoaming.push_back({ -10.f, 0.f, 0.f });
	m_vecRoaming.push_back({ 0.f, 0.f, -10.f });

	// m_pStateCom->Change_State(NPC_IDLE);
	m_pStateCom->Change_State(NPC_MOVE_ONEWAY);

	return S_OK;
}

HRESULT CHumanChild01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanChild01::Ready_Sockets()
{
	return S_OK;
}

CHumanChild01* CHumanChild01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanChild01* pInstance = new CHumanChild01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanChild01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanChild01::Clone(void* pArg)
{
	CHumanChild01* pInstance = new CHumanChild01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanChild01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanChild01::Free()
{
	__super::Free();
}