#include "stdafx.h"
#include "DreamMazeWitch_Npc.h"

#include "GameInstance.h"

#include "UI_World_NPCTag.h"
#include "UI_World_NPCSpeechBalloon.h"

#include "NpcDMWState_Idle.h"
#include "NpcDMWState_InvasionAppear.h"
#include "NpcDMWState_InvasionDisappearTurn.h"
#include "NpcDMWState_InvasionDisappear.h"

CDreamMazeWitch_Npc::CDreamMazeWitch_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CDreamMazeWitch_Npc::CDreamMazeWitch_Npc(const CDreamMazeWitch_Npc& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CDreamMazeWitch_Npc::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
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
	m_pTag->Set_Owner(this, m_strKorName, 1.f);

	CGameObject* pBalloon = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_SpeechBalloon"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBalloon)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon))
		return E_FAIL;
	m_pBalloon = dynamic_cast<CUI_World_NPCSpeechBalloon*>(pBalloon);
	m_pBalloon->Set_Owner(this, 1.2f);
	m_pBalloon->Set_Balloon(TEXT(""));

	return S_OK;
}

void CDreamMazeWitch_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	//if (nullptr != m_pTag)
	//	m_pTag->Tick(fTimeDelta);
	//if (nullptr != m_pBalloon)
	//	m_pBalloon->Tick(fTimeDelta);
}

void CDreamMazeWitch_Npc::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	//if (nullptr != m_pTag)
	//	m_pTag->LateTick(fTimeDelta);
	//if (nullptr != m_pBalloon)
	//	m_pBalloon->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CDreamMazeWitch_Npc::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CDreamMazeWitch_Npc::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CDreamMazeWitch_Npc::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CDreamMazeWitch_Npc::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CDreamMazeWitch_Npc::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_DreamerMazeWitch"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Ready_States()
{
	m_tStat.fSpeed = 6.f;

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_IDLE, CNpcDMWState_Idle::Create(m_pStateCom, strAnimationName));

	/* Invasion */
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_CSDreamersMazeWitch01");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_APPEAR, CNpcDMWState_InvasionAppear::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Stand");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_DISAPPEARTURN, CNpcDMWState_InvasionDisappearTurn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_DreamersMazeWitch.ao|DreamersMazeWitch_Walk");
	m_pStateCom->Add_State(WITCHSTATE_INVASION_DISAPPEAR, CNpcDMWState_InvasionDisappear::Create(m_pStateCom, strAnimationName));

	/* Boss Room */

	/* Battle */

	m_pStateCom->Change_State(WITCHSTATE_INVASION_IDLE);

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDreamMazeWitch_Npc::Ready_Sockets()
{
	return S_OK;
}

CDreamMazeWitch_Npc* CDreamMazeWitch_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CDreamMazeWitch_Npc* pInstance = new CDreamMazeWitch_Npc(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CDreamMazeWitch_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDreamMazeWitch_Npc::Clone(void* pArg)
{
	CDreamMazeWitch_Npc* pInstance = new CDreamMazeWitch_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CDreamMazeWitch_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDreamMazeWitch_Npc::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pBalloon);
}