#include "stdafx.h"
#include "HumanML12.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CHumanML12::CHumanML12(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CHumanML12::CHumanML12(const CHumanML12& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CHumanML12::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanML12::Initialize(void* pArg)
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

void CHumanML12::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHumanML12::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CHumanML12::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CHumanML12::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CHumanML12::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CHumanML12::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CHumanML12::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 4.f, 0.f, 0.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_HumanML12"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanML12::Ready_States()
{
	m_strObjectTag = TEXT("HumanML12");

	m_tStat.fSpeed = 0.5f;
	m_tStat.bHasMove = false;
	m_tStat.bHasTalk = false;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanML12.ao|HumanML12_CSRunningLlapaca01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanML12.ao|HumanML12_CSRunningLlapaca01");
	strAnimationName.push_back(L"SKM_HumanML12.ao|HumanML12_CSRunningLlapaca01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanML12.ao|HumanML12_CSRunningLlapaca01");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanML12.ao|HumanML12_CSRunningLlapaca01");
	strAnimationName.push_back(L"SKM_HumanML12.ao|HumanML12_CSRunningLlapaca01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CHumanML12::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHumanML12::Ready_Sockets()
{
	return S_OK;
}

CHumanML12* CHumanML12::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHumanML12* pInstance = new CHumanML12(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CHumanML12");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHumanML12::Clone(void* pArg)
{
	CHumanML12* pInstance = new CHumanML12(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CHumanML12");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHumanML12::Free()
{
	__super::Free();
}