#include "stdafx.h"
#include "Hair.h"

CHair::CHair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CPart(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_CHARACTER)
{

}

CHair::CHair(CHair& rhs)
	: CPart(rhs)
{

}

HRESULT CHair::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CHair::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;


	return S_OK;
}

void CHair::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CHair::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CHair::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CHair::Ready_Components()
{
	return S_OK;
}


CHair* CHair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CHair* pInstance = new CHair(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create Failed. : CHair");
		return nullptr;
	}

	return pInstance;
}

CGameObject* CHair::Clone(void* pArg)
{
	CHair* pInstance = new CHair(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Clone Failed. : CHair");
		return nullptr;
	}
	return pInstance;
}

void CHair::Free()
{
	__super::Free();
}


