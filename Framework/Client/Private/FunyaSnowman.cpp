#include "stdafx.h"
#include "FunyaSnowman.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CFunyaSnowman::CFunyaSnowman(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CFunyaSnowman::CFunyaSnowman(const CFunyaSnowman& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CFunyaSnowman::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFunyaSnowman::Initialize(void* pArg)
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

void CFunyaSnowman::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CFunyaSnowman::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CFunyaSnowman::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CFunyaSnowman::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CFunyaSnowman::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CFunyaSnowman::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CFunyaSnowman::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 2.f, 0.f, -5.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_FunyaSnowman"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CFunyaSnowman::Ready_States()
{
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	// SKM_RunnerCat.ao|RunnerCat_Stand
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Stand01");
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Idle01");
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_CSGiftFromSnowman01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Stand01Talk01");
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Stand01");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Walk");
	strAnimationName.push_back(L"SKM_FunyaSnowman.ao|FunyaSnowman_Stand01");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CFunyaSnowman::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFunyaSnowman::Ready_Sockets()
{
	return S_OK;
}

CFunyaSnowman* CFunyaSnowman::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CFunyaSnowman* pInstance = new CFunyaSnowman(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CFunyaSnowman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFunyaSnowman::Clone(void* pArg)
{
	CFunyaSnowman* pInstance = new CFunyaSnowman(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CFunyaSnowman");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFunyaSnowman::Free()
{
	__super::Free();
}