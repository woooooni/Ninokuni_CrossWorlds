#include "stdafx.h"
#include "Winter_MiniWalls.h"

CWinter_MiniWalls::CWinter_MiniWalls(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniWalls::CWinter_MiniWalls(const CWinter_MiniWalls& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniWalls::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniWalls::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniWalls::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniWalls::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniWalls::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniWalls::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniWalls::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniWalls* CWinter_MiniWalls::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniWalls* pInstance = new CWinter_MiniWalls(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniWalls*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniWalls");
	}

	return pInstance;
}

CGameObject* CWinter_MiniWalls::Clone(void* pArg)
{
	CWinter_MiniWalls* pInstance = new CWinter_MiniWalls(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniWalls*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniWalls");
	}

	return pInstance;
}

void CWinter_MiniWalls::Free()
{
	__super::Free();
}
