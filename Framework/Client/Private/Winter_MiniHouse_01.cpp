#include "stdafx.h"
#include "Winter_MiniHouse_01.h"

CWinter_MiniHouse_01::CWinter_MiniHouse_01(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniHouse_01::CWinter_MiniHouse_01(const CWinter_MiniHouse_01& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniHouse_01::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniHouse_01::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniHouse_01::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniHouse_01::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniHouse_01::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniHouse_01::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniHouse_01::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniHouse_01* CWinter_MiniHouse_01::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniHouse_01* pInstance = new CWinter_MiniHouse_01(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniHouse_01*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniHouse_01");
	}

	return pInstance;
}

CGameObject* CWinter_MiniHouse_01::Clone(void* pArg)
{
	CWinter_MiniHouse_01* pInstance = new CWinter_MiniHouse_01(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniHouse_01*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniHouse_01");
	}

	return pInstance;
}

void CWinter_MiniHouse_01::Free()
{
	__super::Free();
}
