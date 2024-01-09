#include "stdafx.h"
#include "Kuu.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

#include "Game_Manager.h"
#include "Player.h"
#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

CKuu::CKuu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CKuu::CKuu(const CKuu& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CKuu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKuu::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Kuu.ao|Kuu_NeutralStand"));


	m_pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (m_pPlayer != nullptr)
		m_pPlayerTransform = m_pPlayer->Get_Component<CTransform>(TEXT("Com_Transform"));

	// 쿠우 네임태그 생성
//	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
//	if (nullptr == pTag)
//		return E_FAIL;
//
//	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
//	m_pTag->Set_Owner(this, TEXT("쿠우"), 1.f);
//
//	// NPC 말풍선 생성
//	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
//	if (nullptr == pBalloon)
//		return E_FAIL;
//
//	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
//	m_pBalloon->Set_Owner(this, 1.3f);
//	m_pBalloon->Set_Balloon(TEXT("나는 테스트용 쿠우다."));


	return S_OK;
}

void CKuu::Tick(_float fTimeDelta)
{
	if (m_pPlayer != nullptr)
	{
		Vec4 vReleativePos = m_pPlayerTransform->Get_RelativeOffset({ 1.f, 1.f, -1.f, 1.f });
		Vec4 vPlayerPos = m_pPlayerTransform->Get_Position();

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPlayerPos + vReleativePos);
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_pPlayerTransform->Get_Right());
		m_pTransformCom->Set_State(CTransform::STATE_UP, m_pPlayerTransform->Get_Up());
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_pPlayerTransform->Get_Look());
	}


	// __super::Tick(fTimeDelta);
	// m_pRigidBodyCom->Update_RigidBody(fTimeDelta);

//	if (nullptr != m_pTag)
//		m_pTag->Tick(fTimeDelta);
//	if (nullptr != m_pBalloon)
//		m_pBalloon->Tick(fTimeDelta);

	m_pControllerCom->Tick_Controller(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::NPC, this);

}

void CKuu::LateTick(_float fTimeDelta)
{
//	if (nullptr != m_pTag)
//		m_pTag->LateTick(fTimeDelta);
//	if (nullptr != m_pBalloon)
//		m_pBalloon->LateTick(fTimeDelta);

	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CKuu::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CKuu::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CKuu::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CKuu::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CKuu::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	//m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f, 0.f, 12.f, 1.f });
	//m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Kuu"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKuu::Ready_States()
{
	m_tStat.fSpeed = 1.f;
	m_tStat.bIsUnique = true;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	return S_OK;
}

HRESULT CKuu::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CKuu::Ready_Sockets()
{
	return S_OK;
}

CKuu* CKuu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CKuu* pInstance = new CKuu(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CKuu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CKuu::Clone(void* pArg)
{
	CKuu* pInstance = new CKuu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CKuu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKuu::Free()
{
	__super::Free();

//	Safe_Release(m_pBalloon);
//	Safe_Release(m_pTag);
}
