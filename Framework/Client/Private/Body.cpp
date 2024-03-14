#include "stdafx.h"
#include "Body.h"

CBody::CBody(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CPart(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
{

}

CBody::CBody(CBody& rhs)
	: CPart(rhs)
{

}

HRESULT CBody::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CBody::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBody::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBody::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBody::Ready_Components()
{
	return S_OK;
}


CBody* CBody::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CBody* pInstance = new CBody(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed. : CBody");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CBody::Clone(void* pArg)
{
	CBody* pInstance = new CBody(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CBody");
		return nullptr;
	}
	return pInstance;
}

void CBody::Free()
{
	__super::Free();
}


