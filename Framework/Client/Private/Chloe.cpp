#include "stdafx.h"
#include "Chloe.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

CChloe::CChloe(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CChloe::CChloe(const CChloe& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CChloe::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CChloe::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Chloe.ao|Chloe_NeutralStand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;


	return S_OK;
}

void CChloe::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CChloe::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CChloe::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CChloe::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CChloe::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CChloe::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CChloe::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CChloe::Ready_States()
{
	return S_OK;
}

HRESULT CChloe::Ready_Colliders()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Chloe.ao|Chloe_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Chloe.ao|Chloe_Attack");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Chloe.ao|Chloe_NeutralWalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Chloe.ao|Chloe_NeutralRun");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CChloe::Ready_Sockets()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

CChloe* CChloe::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CChloe* pInstance = new CChloe(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CChloe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChloe::Clone(void* pArg)
{
	CChloe* pInstance = new CChloe(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CChloe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChloe::Free()
{
	__super::Free();
}