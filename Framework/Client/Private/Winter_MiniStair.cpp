#include "stdafx.h"
#include "Winter_MiniStair.h"

CWinter_MiniStair::CWinter_MiniStair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CMiniGameMap(pDevice, pContext, strObjectTag, eType)
{
}

CWinter_MiniStair::CWinter_MiniStair(const CWinter_MiniStair& rhs)
	: CMiniGameMap(rhs)
{

}

HRESULT CWinter_MiniStair::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniStair::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CWinter_MiniStair::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CWinter_MiniStair::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWinter_MiniStair::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;

	return S_OK;
}

HRESULT CWinter_MiniStair::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (FAILED(__super::Render_Instance_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices)))
		return E_FAIL;


	return S_OK;
}

HRESULT CWinter_MiniStair::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

CWinter_MiniStair* CWinter_MiniStair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
{
	CWinter_MiniStair* pInstance = new CWinter_MiniStair(pDevice, pContext, strObjectTag, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		Safe_Release<CWinter_MiniStair*>(pInstance);
		MSG_BOX("Failed to Created : CWinter_MiniStair");
	}

	return pInstance;
}

CGameObject* CWinter_MiniStair::Clone(void* pArg)
{
	CWinter_MiniStair* pInstance = new CWinter_MiniStair(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		Safe_Release<CWinter_MiniStair*>(pInstance);
		MSG_BOX("Failed to Cloned : CWinter_MiniStair");
	}

	return pInstance;
}

void CWinter_MiniStair::Free()
{
	__super::Free();
}
