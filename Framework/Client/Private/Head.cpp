#include "stdafx.h"
#include "Head.h"

CHead::CHead(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CPart(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
{

}

CHead::CHead(CHead& rhs)
	: CPart(rhs)
{

}

HRESULT CHead::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHead::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CHead::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHead::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CHead::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CHead::Ready_Components()
{
	return S_OK;
}


CHead* CHead::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHead* pInstance = new CHead(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed. : CHead");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHead::Clone(void* pArg)
{
	CHead* pInstance = new CHead(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CHead");
		return nullptr;
	}
	return pInstance;
}

void CHead::Free()
{
	__super::Free();
}


