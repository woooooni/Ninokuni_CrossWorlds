#include "stdafx.h"
#include "Kuu.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "NpcState_Talk.h"
#include "NpcState_OneWay.h"
#include "NpcState_TwoWay.h"

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

	return S_OK;
}

void CKuu::Tick(_float fTimeDelta)
{
	// __super::Tick(fTimeDelta);
	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	m_pControllerCom->Tick_Controller(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::NPC, this);

}

void CKuu::LateTick(_float fTimeDelta)
{
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
}
