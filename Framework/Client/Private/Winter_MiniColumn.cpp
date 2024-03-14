#include "stdafx.h"
#include "Winter_MiniColumn.h"

CWinter_MiniColumn::CWinter_MiniColumn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniColumn::CWinter_MiniColumn(const CWinter_MiniColumn& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniColumn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniColumn::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniColumn::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniColumn::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniColumn::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniColumn::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniColumn::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniColumn* CWinter_MiniColumn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniColumn* pInstance = new CWinter_MiniColumn(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniColumn*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniColumn");
	}

	return pInstance;
}

CGameObject* CWinter_MiniColumn::Clone(void* pArg)
{
	CWinter_MiniColumn* pInstance = new CWinter_MiniColumn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniColumn*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniColumn");
	}

	return pInstance;
}

void CWinter_MiniColumn::Free()
{
	__super::Free();
}
