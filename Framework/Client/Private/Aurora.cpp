#include "..\Public\Aurora.h"
#include "Aurora.h"
#include "GameInstance.h"

CAurora::CAurora(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag,eType)
{

}

CAurora::CAurora(const CAurora& rhs)
	: CGameObject(rhs)
{
}

HRESULT CAurora::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAurora::Initialize(void* pArg)
{
	return S_OK;
}

void CAurora::Tick(_float fTimeDelta)
{
}

void CAurora::LateTick(_float fTimeDelta)
{
}

HRESULT CAurora::Render()
{
	return S_OK;
}

HRESULT CAurora::Ready_Components()
{
	return S_OK;
}

CAurora* CAurora::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	return nullptr;
}

CGameObject* CAurora::Clone(void* pArg)
{
	return nullptr;
}

void CAurora::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIBufferCom);
	
	for (_uint i = 0; i < LEVEL_AURORA::AURORA_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);
}
