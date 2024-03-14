#include "stdafx.h"
#include "Winter_MiniSnowMan.h"

CWinter_MiniSnowMan::CWinter_MiniSnowMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniSnowMan::CWinter_MiniSnowMan(const CWinter_MiniSnowMan& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniSnowMan::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniSnowMan::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniSnowMan::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniSnowMan::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniSnowMan::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniSnowMan::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniSnowMan::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniSnowMan* CWinter_MiniSnowMan::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniSnowMan* pInstance = new CWinter_MiniSnowMan(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniSnowMan*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniSnowMan");
	}

	return pInstance;
}

CGameObject* CWinter_MiniSnowMan::Clone(void* pArg)
{
	CWinter_MiniSnowMan* pInstance = new CWinter_MiniSnowMan(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniSnowMan*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniSnowMan");
	}

	return pInstance;
}

void CWinter_MiniSnowMan::Free()
{
	__super::Free();
}
