#include "stdafx.h"
#include "CaliaHuman.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CCaliaHuman::CCaliaHuman(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CCaliaHuman::CCaliaHuman(const CCaliaHuman& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CCaliaHuman::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCaliaHuman::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 2.f);

	return S_OK;
}

void CCaliaHuman::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CCaliaHuman::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CCaliaHuman::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CCaliaHuman::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CCaliaHuman::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CCaliaHuman::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CCaliaHuman::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Stand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Talk01");
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Walk");
	strAnimationName.push_back(L"SKM_CaliaHuman.ao|CaliaHuman_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_TALK);

	return S_OK;
}

HRESULT CCaliaHuman::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCaliaHuman::Ready_Sockets()
{
	return S_OK;
}

CCaliaHuman* CCaliaHuman::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CCaliaHuman* pInstance = new CCaliaHuman(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CCaliaHuman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCaliaHuman::Clone(void* pArg)
{
	CCaliaHuman* pInstance = new CCaliaHuman(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CCaliaHuman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCaliaHuman::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}