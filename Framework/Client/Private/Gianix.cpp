#include "stdafx.h"
#include "Gianix.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"




CGianix::CGianix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType)
	: CCharacter(pDevice, pContext, strObjectTag, eCharacterType)
{
}

CGianix::CGianix(const CGianix& rhs)
	: CCharacter(rhs)
{

}

HRESULT CGianix::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGianix::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//if (FAILED(Ready_Sockets()))
	//	return E_FAIL;

	//if (FAILED(Ready_Parts()))
	//	return E_FAIL;

	//if (FAILED(Ready_States()))
	//	return E_FAIL;

 //	if (FAILED(Ready_Colliders()))
	//	return E_FAIL;

	return S_OK;
}

void CGianix::Tick(_float fTimeDelta)
{
	m_pStateCom->Tick_State(fTimeDelta);
	__super::Tick(fTimeDelta);

	_bool bKeyInput = false;
	if (KEY_HOLD(KEY::UP_ARROW))
	{
		bKeyInput = true;
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::DOWN_ARROW))
	{
		bKeyInput = true;
		_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
		m_pTransformCom->Move(-1.f * vLook, 10.f, fTimeDelta);
	}

	if (KEY_HOLD(KEY::LEFT_ARROW))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-90.f) * fTimeDelta);
		if (false == bKeyInput)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
			m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
		}


	}

	if (KEY_HOLD(KEY::RIGHT_ARROW))
	{
		m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f) * fTimeDelta);
		if (false == bKeyInput)
		{
			_vector vLook = XMVector3Normalize(m_pTransformCom->Get_Look());
			m_pTransformCom->Move(vLook, 10.f, fTimeDelta);
		}

	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_pRigidBodyCom->Add_Velocity(XMVectorSet(0.f, 1.f, 0.f, 0.f), 10.f, false);
	}
}

void CGianix::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
}

HRESULT CGianix::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

void CGianix::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CGianix::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CGianix::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CGianix::On_Damaged(const COLLISION_INFO& tInfo)
{
	__super::On_Damaged(tInfo);
}





HRESULT CGianix::Ready_Components()
{

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Gianix"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_States
HRESULT CGianix::Ready_States()
{
	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CGianix::Ready_Colliders()
{
	Set_ActiveColliders(CCollider::DETECTION_TYPE::ATTACK, false);

	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CGianix::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CGianix::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

CGianix* CGianix::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, CHARACTER_TYPE eCharacterType)
{
	CGianix* pInstance = new CGianix(pDevice, pContext, strObjectTag, eCharacterType);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGianix");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGianix::Clone(void* pArg)
{
	CGianix* pInstance = new CGianix(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGianix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGianix::Free()
{
	__super::Free();
}

