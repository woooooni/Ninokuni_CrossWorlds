#include "stdafx.h"
#include "KingdomGuard.h"

#include "GameInstance.h"
#include "NpcWeapon_Halberd.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "UI_World_NPCTag.h"

CKingdomGuard::CKingdomGuard(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CKingdomGuard::CKingdomGuard(const CKingdomGuard& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CKingdomGuard::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKingdomGuard::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pWeapon = dynamic_cast<CWeapon*>(GI->Clone_GameObject(TEXT("Prorotype_GameObject_NpcWeapon_Halberd"), LAYER_WEAPON));

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 2.2f);

	return S_OK;
}

void CKingdomGuard::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CKingdomGuard::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_pWeapon != nullptr)
	{
		Matrix matSocketLocal = m_pModelCom->Get_SocketLocalMatrix(0); // 내가 추가한 소켓에서 인덱스 선택.
		Matrix matSocketWorld = matSocketLocal * m_pTransformCom->Get_WorldMatrix();

		m_pWeapon->Set_SocketWorld(matSocketWorld);
	}

	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);


#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CKingdomGuard::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CKingdomGuard::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CKingdomGuard::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CKingdomGuard::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CKingdomGuard::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_EvermoreGuard02.ao|EvermoreGuard02_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_EvermoreGuard02.ao|EvermoreGuard02_Talk01");
	strAnimationName.push_back(L"SKM_EvermoreGuard02.ao|EvermoreGuard02_NeutralStand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_EvermoreGuard02.ao|EvermoreGuard02_NeutralWalk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_EvermoreGuard02.ao|EvermoreGuard02_NeutralWalk");
	strAnimationName.push_back(L"SKM_EvermoreGuard02.ao|EvermoreGuard02_NeutralStand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CKingdomGuard::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKingdomGuard::Ready_Sockets()
{
	return S_OK;
}

CKingdomGuard* CKingdomGuard::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CKingdomGuard* pInstance = new CKingdomGuard(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CKingdomGuard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKingdomGuard::Clone(void* pArg)
{
	CKingdomGuard* pInstance = new CKingdomGuard(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CKingdomGuard");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKingdomGuard::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}
