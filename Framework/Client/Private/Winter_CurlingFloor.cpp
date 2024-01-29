#include "stdafx.h"
#include "Winter_CurlingFloor.h"

CWinter_CurlingFloor::CWinter_CurlingFloor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_CurlingFloor::CWinter_CurlingFloor(const CWinter_CurlingFloor& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_CurlingFloor::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_CurlingFloor::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_CurlingFloor::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_CurlingFloor::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_CurlingFloor::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_CurlingFloor::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_CurlingFloor::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_CurlingFloor* CWinter_CurlingFloor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_CurlingFloor* pInstance = new CWinter_CurlingFloor(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_CurlingFloor*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_CurlingFloor");
	}

	return pInstance;
}

CGameObject* CWinter_CurlingFloor::Clone(void* pArg)
{
	CWinter_CurlingFloor* pInstance = new CWinter_CurlingFloor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_CurlingFloor*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_CurlingFloor");
	}

	return pInstance;
}

void CWinter_CurlingFloor::Free()
{
	__super::Free();
}
