#include "stdafx.h"
#include "GameInstance.h"
#include "DefenceInvasion_Portal.h"
#include "Level_Loading.h"
#include "Effect_Manager.h"
#include "Vfx.h"

#include "Game_Manager.h"
#include "Player.h"


CDefenceInvasion_Portal::CDefenceInvasion_Portal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"DefenceInvasion_Portal", OBJ_TYPE::OBJ_PORTAL)
{
}

CDefenceInvasion_Portal::CDefenceInvasion_Portal(const CDefenceInvasion_Portal& rhs)
	: CGameObject(rhs)
{
}


HRESULT CDefenceInvasion_Portal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDefenceInvasion_Portal::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDefenceInvasion_Portal::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CDefenceInvasion_Portal::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CDefenceInvasion_Portal::Render()
{
	return S_OK;
}

void CDefenceInvasion_Portal::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CDefenceInvasion_Portal::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CDefenceInvasion_Portal::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CDefenceInvasion_Portal::Ready_Components()
{
	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_InvasionPortal"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}



CDefenceInvasion_Portal* CDefenceInvasion_Portal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDefenceInvasion_Portal* pInstance = new CDefenceInvasion_Portal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CDefenceInvasion_Portal");
		Safe_Release<CDefenceInvasion_Portal*>(pInstance);
	}

	return pInstance;
}

CGameObject* CDefenceInvasion_Portal::Clone(void* pArg)
{
	CDefenceInvasion_Portal* pInstance = new CDefenceInvasion_Portal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CDefenceInvasion_Portal");
		Safe_Release<CDefenceInvasion_Portal*>(pInstance);
	}

	return pInstance;
}

void CDefenceInvasion_Portal::Free()
{
	__super::Free();
	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
