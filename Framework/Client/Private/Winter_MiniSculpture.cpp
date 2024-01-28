#include "stdafx.h"
#include "Winter_MiniSculpture.h"

CWinter_Sculpture::CWinter_Sculpture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_Sculpture::CWinter_Sculpture(const CWinter_Sculpture& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_Sculpture::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_Sculpture::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_Sculpture::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_Sculpture::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_Sculpture::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_Sculpture::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_Sculpture::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_Sculpture* CWinter_Sculpture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_Sculpture* pInstance = new CWinter_Sculpture(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_Sculpture*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_Sculpture");
	}

	return pInstance;
}

CGameObject* CWinter_Sculpture::Clone(void* pArg)
{
	CWinter_Sculpture* pInstance = new CWinter_Sculpture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_Sculpture*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_Sculpture");
	}

	return pInstance;
}

void CWinter_Sculpture::Free()
{
	__super::Free();
}
