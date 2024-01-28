#include "stdafx.h"
#include "Winter_MiniBKHouse.h"

CWinter_MiniBKHouse::CWinter_MiniBKHouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniBKHouse::CWinter_MiniBKHouse(const CWinter_MiniBKHouse& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniBKHouse::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniBKHouse::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniBKHouse::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniBKHouse::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniBKHouse::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniBKHouse::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniBKHouse::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniBKHouse* CWinter_MiniBKHouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniBKHouse* pInstance = new CWinter_MiniBKHouse(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniBKHouse*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniBKHouse");
	}

	return pInstance;
}

CGameObject* CWinter_MiniBKHouse::Clone(void* pArg)
{
	CWinter_MiniBKHouse* pInstance = new CWinter_MiniBKHouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniBKHouse*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniBKHouse");
	}

	return pInstance;
}

void CWinter_MiniBKHouse::Free()
{
	__super::Free();
}
