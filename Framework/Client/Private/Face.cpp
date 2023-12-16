#include "stdafx.h"
#include "Face.h"

CFace::CFace(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CPart(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
{

}

CFace::CFace(CFace& rhs)
	: CPart(rhs)
{

}

HRESULT CFace::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFace::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CFace::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CFace::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CFace::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CFace::Ready_Components()
{
	return S_OK;
}


CFace* CFace::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CFace* pInstance = new CFace(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed. : CFace");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CFace::Clone(void* pArg)
{
	CFace* pInstance = new CFace(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CFace");
		return nullptr;
	}
	return pInstance;
}

void CFace::Free()
{
	__super::Free();
}


