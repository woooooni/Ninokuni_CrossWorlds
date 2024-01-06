#include "stdafx.h"
#include "Ghost2.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CGhost2::CGhost2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGhost2::CGhost2(const CGhost2& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGhost2::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost2::Initialize(void* pArg)
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

void CGhost2::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGhost2::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGhost2::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGhost2::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGhost2::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGhost2::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CGhost2::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -2.f, 0.f, -5.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ghost2"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGhost2::Ready_States()
{
	m_tStat.fSpeed = 1.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	// SKM_RunnerCat.ao|RunnerCat_Stand
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Stand");
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Idle01");
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Idle02");
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Hello");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Talk");
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Walk");
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CGhost2::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGhost2::Ready_Sockets()
{
	return S_OK;
}

CGhost2* CGhost2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGhost2* pInstance = new CGhost2(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGhost2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGhost2::Clone(void* pArg)
{
	CGhost2* pInstance = new CGhost2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGhost2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhost2::Free()
{
	__super::Free();
}