#include "stdafx.h"
#include "Grandprix_Enemy_HumanFL04.h"

#include "GameInstance.h"

#include "State_Enemy_Flying_RunStart.h"
#include "State_Enemy_Flying_Stand.h"
#include "State_Enemy_Flying_Run.h"

CGrandprix_Enemy_HumanFL04::CGrandprix_Enemy_HumanFL04(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGrandprix_Enemy(pDevice, pContext, strObjectTag)
{
}

CGrandprix_Enemy_HumanFL04::CGrandprix_Enemy_HumanFL04(const CGrandprix_Enemy_HumanFL04& rhs)
	: CGrandprix_Enemy(rhs)
{
}

HRESULT CGrandprix_Enemy_HumanFL04::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Enemy_HumanFL04::Initialize(void* pArg)
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
//	m_pModelCom->Set_Animation(0);

	return S_OK;
}

void CGrandprix_Enemy_HumanFL04::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CGrandprix_Enemy_HumanFL04::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

void CGrandprix_Enemy_HumanFL04::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_Enemy_HumanFL04::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CGrandprix_Enemy_HumanFL04::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

HRESULT CGrandprix_Enemy_HumanFL04::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	// For Model Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_HumanFL04"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrandprix_Enemy_HumanFL04::Ready_States()
{
//	m_pStateCom->Set_Owner(this);
//
	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_Stand");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUNSTART, CState_Enemy_Flying_RunStart::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_CSResurrectionOfTheKingdom01");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND, CState_Enemy_Flying_Stand::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_HumanFL04.ao|HumanFL04_CSResurrectionOfTheKingdom01");
	m_pStateCom->Add_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_RUN, CState_Enemy_Flying_Run::Create(m_pStateCom, strAnimationName));

	//SKM_HumanFL04.ao|HumanFL04_CSPurifiedForestOfNiallSickStand 피격 애니메이션

	m_pStateCom->Change_State(CGrandprix_Enemy::ENEMY_STATE::FLYING_STAND);

	return S_OK;
}

HRESULT CGrandprix_Enemy_HumanFL04::Ready_Colliders()
{

	return S_OK;
}

HRESULT CGrandprix_Enemy_HumanFL04::Ready_Sockets()
{
	return S_OK;
}

CGrandprix_Enemy_HumanFL04* CGrandprix_Enemy_HumanFL04::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrandprix_Enemy_HumanFL04* pInstance = new CGrandprix_Enemy_HumanFL04(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CGrandprix_Enemy_HumanFL04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CGrandprix_Enemy_HumanFL04::Clone(void* pArg)
{
	CGrandprix_Enemy_HumanFL04* pInstance = new CGrandprix_Enemy_HumanFL04(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CGrandprix_Enemy_HumanFL04");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGrandprix_Enemy_HumanFL04::Free()
{
	__super::Free();
}