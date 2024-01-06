#include "stdafx.h"
#include "ThiefCat.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

CThiefCat::CThiefCat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CThiefCat::CThiefCat(const CThiefCat& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CThiefCat::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CThiefCat::Initialize(void* pArg)
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

void CThiefCat::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CThiefCat::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CThiefCat::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CThiefCat::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CThiefCat::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CThiefCat::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CThiefCat::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { -6.f, 0.f, -5.f, 1.f });
	m_vInitPos = m_pTransformCom->Get_Position();

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_ThiefCat"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	if (FAILED(__super::Ready_Components()))
		return E_FAIL;


	return S_OK;
}

HRESULT CThiefCat::Ready_States()
{
	m_tStat.fSpeed = 2.f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	// SKM_RunnerCat.ao|RunnerCat_Stand
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Gather");
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Confuse");
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Idle");
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Hello01");
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Attack01");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Talk01");
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Stand");
	m_pStateCom->Add_State(NPC_TALK, CNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	// SKM_RunnerCat.ao|RunnerCat_Run
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Walk");
	m_pStateCom->Add_State(NPC_MOVE_ONEWAY, CNpcState_OneWay::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Walk");
	strAnimationName.push_back(L"SKM_ThiefCat.ao|RunnerCat_Stand");
	m_pStateCom->Add_State(NPC_MOVE_TWOWAY, CNpcState_TwoWay::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CThiefCat::Ready_Colliders()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

HRESULT CThiefCat::Ready_Sockets()
{
	return S_OK;
}

CThiefCat* CThiefCat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CThiefCat* pInstance = new CThiefCat(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CThiefCat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CThiefCat::Clone(void* pArg)
{
	CThiefCat* pInstance = new CThiefCat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CThiefCat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CThiefCat::Free()
{
	__super::Free();
}