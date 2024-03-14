#include "stdafx.h"
#include "Winter_MiniWall.h"

CWinter_MiniWall::CWinter_MiniWall(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniWall::CWinter_MiniWall(const CWinter_MiniWall& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniWall::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniWall::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniWall::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniWall::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniWall::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniWall::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniWall::Ready_Components()
{
 	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniWall* CWinter_MiniWall::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniWall* pInstance = new CWinter_MiniWall(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniWall*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniWall");
	}

	return pInstance;
}

CGameObject* CWinter_MiniWall::Clone(void* pArg)
{
	CWinter_MiniWall* pInstance = new CWinter_MiniWall(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniWall*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniWall");
	}

	return pInstance;
}

void CWinter_MiniWall::Free()
{
	__super::Free();
}
