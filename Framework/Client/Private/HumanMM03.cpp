#include "stdafx.h"
#include "HumanMM03.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanMM03::CHumanMM03(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanMM03::CHumanMM03(const CHumanMM03& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanMM03::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM03::Initialize(void* pArg)
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

void CHumanMM03::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanMM03::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanMM03::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanMM03::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanMM03::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanMM03::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanMM03::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 2.f, 0.f, 0.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_HumanMM03"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM03::Ready_States()
{
	m_strObjectTag = TEXT("HumanMM03");

	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasMove = false;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSHeroOfEvermore01");
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM_03_CSCheer2");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM_03_CSCheer2");
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM_03_CSCheer2");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSHopeOfNewPeriod01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSHopeOfNewPeriod01");
	strAnimationName.push_back(L"SKM_HumanMM03.ao|HumanMM03_CSMechaRagonAppear02");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanMM03::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanMM03::Ready_Sockets()
{
	return S_OK;
}

CHumanMM03* CHumanMM03::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanMM03* pInstance = new CHumanMM03(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanMM03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanMM03::Clone(void* pArg)
{
	CHumanMM03* pInstance = new CHumanMM03(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanMM03");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanMM03::Free()
{
	__super::Free();
}