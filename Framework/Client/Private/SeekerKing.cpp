#include "stdafx.h"
#include "SeekerKing.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CSeekerKing::CSeekerKing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CSeekerKing::CSeekerKing(const CSeekerKing& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CSeekerKing::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerKing::Initialize(void* pArg)
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

void CSeekerKing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CSeekerKing::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CSeekerKing::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CSeekerKing::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CSeekerKing::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CSeekerKing::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CSeekerKing::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -6.f, 0.f, 0.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SeekerKing"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerKing::Ready_States()
{
	m_tStat.fSpeed = 2.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerCat_Stand");
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerCat_Idle01");
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerKing_Hello01");
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerKing_Confuse");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerKing_Talk01");
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerCat_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	// SKM_SeekerKing.ao|SeekerCat_Walk
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerCat_Run");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerCat_Run");
	strAnimationName.push_back(L"SKM_SeekerKing.ao|SeekerCat_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CSeekerKing::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSeekerKing::Ready_Sockets()
{
	return S_OK;
}

CSeekerKing* CSeekerKing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CSeekerKing* pInstance = new CSeekerKing(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CSeekerKing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSeekerKing::Clone(void* pArg)
{
	CSeekerKing* pInstance = new CSeekerKing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CSeekerKing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSeekerKing::Free()
{
	__super::Free();
}