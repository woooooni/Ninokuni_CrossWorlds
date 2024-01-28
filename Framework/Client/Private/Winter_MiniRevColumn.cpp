#include "stdafx.h"
#include "Winter_MiniRevColumn.h"

CWinter_MiniRevColumn::CWinter_MiniRevColumn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniRevColumn::CWinter_MiniRevColumn(const CWinter_MiniRevColumn& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniRevColumn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniRevColumn::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniRevColumn::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniRevColumn::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniRevColumn::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniRevColumn::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniRevColumn::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniRevColumn* CWinter_MiniRevColumn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniRevColumn* pInstance = new CWinter_MiniRevColumn(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniRevColumn*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniRevColumn");
	}

	return pInstance;
}

CGameObject* CWinter_MiniRevColumn::Clone(void* pArg)
{
	CWinter_MiniRevColumn* pInstance = new CWinter_MiniRevColumn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniRevColumn*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniRevColumn");
	}

	return pInstance;
}

void CWinter_MiniRevColumn::Free()
{
	__super::Free();
}
