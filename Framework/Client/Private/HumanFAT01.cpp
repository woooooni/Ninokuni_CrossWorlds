#include "stdafx.h"
#include "HumanFAT01.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanFAT01::CHumanFAT01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanFAT01::CHumanFAT01(const CHumanFAT01& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanFAT01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFAT01::Initialize(void* pArg)
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

void CHumanFAT01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanFAT01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanFAT01::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanFAT01::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanFAT01::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanFAT01::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanFAT01::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 1.f, 0.f, 2.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_HumanFAT01"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CHumanFAT01::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Talk");
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_NeutralStand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Walk");
	strAnimationName.push_back(L"SKM_HumanFAT01.ao|HumanFAT01_Idle01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_TALK);

	return S_OK;
}

HRESULT CHumanFAT01::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanFAT01::Ready_Sockets()
{
	return S_OK;
}

CHumanFAT01* CHumanFAT01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanFAT01* pInstance = new CHumanFAT01(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanFAT01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanFAT01::Clone(void* pArg)
{
	CHumanFAT01* pInstance = new CHumanFAT01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanFAT01");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanFAT01::Free()
{
	__super::Free();
}
