#include "stdafx.h"
#include "Grandprix_Enemy_Ghost2.h"

#include "GameInstance.h"

#include "State_Enemy_Flying_RunStart.h"
#include "State_Enemy_Flying_Stand.h"
#include "State_Enemy_Flying_Run.h"

CGrandprix_Enemy_Ghost2::CGrandprix_Enemy_Ghost2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGrandprix_Enemy(pDevice, pContext, strObjectTag)
{
}

CGrandprix_Enemy_Ghost2::CGrandprix_Enemy_Ghost2(const CGrandprix_Enemy_Ghost2& rhs)
	: CGrandprix_Enemy(rhs)
{
}

HRESULT CGrandprix_Enemy_Ghost2::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Enemy_Ghost2::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	m_pRigidBodyCom->Set_Use_Gravity(false);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(10.f, 0.5f, 30.2f, 1.f));
	m_bActive = false;

	return S_OK;
}

void CGrandprix_Enemy_Ghost2::Tick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::Tick(fTimeDelta);

}

void CGrandprix_Enemy_Ghost2::LateTick(_float fTimeDelta)
{
	if (false == m_bActive)
		return;

	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGrandprix_Enemy_Ghost2::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_Enemy_Ghost2::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_Enemy_Ghost2::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrandprix_Enemy_Ghost2::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Ghost2"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Enemy_Ghost2::Ready_States()
{
	list<wstring> strAnimationName;
	/*
	SKM_Ghost2.ao|Ghost_Walk
	SKM_Ghost2.ao|Ghost_Run
	SKM_Ghost2.ao|Ghost_Spawn
	SKM_Ghost2.ao|Ghost_Stand
	SKM_Ghost2.ao|Ghost_Idle01 // Anim Loop X
	SKM_Ghost2.ao|Ghost_Idle02 // Anim Loop X
	*/
	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Run");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUNSTART, CState_Enemy_Flying_RunStart::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Walk");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND, CState_Enemy_Flying_Stand::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Ghost2.ao|Ghost_Run");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN, CState_Enemy_Flying_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND);

	return S_OK;
}

HRESULT CGrandprix_Enemy_Ghost2::Ready_Colliders()
{

	return S_OK;
}

HRESULT CGrandprix_Enemy_Ghost2::Ready_Sockets()
{
	return S_OK;
}

CGrandprix_Enemy_Ghost2* CGrandprix_Enemy_Ghost2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrandprix_Enemy_Ghost2* pInstance = new CGrandprix_Enemy_Ghost2(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGrandprix_Enemy_Ghost2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrandprix_Enemy_Ghost2::Clone(void* pArg)
{
	CGrandprix_Enemy_Ghost2* pInstance = new CGrandprix_Enemy_Ghost2(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGrandprix_Enemy_Ghost2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrandprix_Enemy_Ghost2::Free()
{
	__super::Free();
}