#include "stdafx.h"
#include "Gosling.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

CGosling::CGosling(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CGosling::CGosling(const CGosling& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CGosling::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGosling::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pModelCom->Set_Animation(TEXT("SKM_Gosling.ao|Gosling_CSCrysterBallSirius01"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CGosling::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CGosling::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CGosling::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CGosling::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGosling::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGosling::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CGosling::On_Damaged(const COLLISION_INFO& tInfo)
{
}


HRESULT CGosling::Ready_States()
{
	m_strKorName = TEXT("°í½½¸µ");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Gosling.ao|Gosling_CSCrysterBallSirius01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Gosling.ao|Gosling_CSCrysterBallSirius01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Gosling.ao|Gosling_CSCrysterBallSirius01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Gosling.ao|Gosling_CSCrysterBallSirius01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CGosling::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 50.f, 70.f, 50.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 70.f, 0.f);

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	OBBBox.Extents = { 100.f, 100.f, 50.f };
	OBBDesc.vOffsetPosition = Vec3(0.f, 70.f, -100.f);
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::ATTACK, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGosling::Ready_Sockets()
{
	return S_OK;
}

CGosling* CGosling::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGosling* pInstance = new CGosling(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGosling");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGosling::Clone(void* pArg)
{
	CGosling* pInstance = new CGosling(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGosling");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGosling::Free()
{
	__super::Free();
}