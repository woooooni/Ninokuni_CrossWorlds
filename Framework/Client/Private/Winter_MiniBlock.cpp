#include "stdafx.h"
#include "Winter_MiniBlock.h"

CWinter_MiniBlock::CWinter_MiniBlock(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniBlock::CWinter_MiniBlock(const CWinter_MiniBlock& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniBlock::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniBlock::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniBlock::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniBlock::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniBlock::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniBlock::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniBlock::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniBlock* CWinter_MiniBlock::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniBlock* pInstance = new CWinter_MiniBlock(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniBlock*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniBlock");
	}

	return pInstance;
}

CGameObject* CWinter_MiniBlock::Clone(void* pArg)
{
	CWinter_MiniBlock* pInstance = new CWinter_MiniBlock(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniBlock*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniBlock");
	}

	return pInstance;
}

void CWinter_MiniBlock::Free()
{
	__super::Free();
}
