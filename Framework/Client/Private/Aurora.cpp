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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	// Aurora Shader.
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Skydome"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

CAurora* CAurora::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CAurora* pInstance = new CAurora(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CAurora");
		Safe_Release<CAurora*>(pInstance);
	}

	return pInstance;
}

CGameObject* CAurora::Clone(void* pArg)
{
	CAurora* pInstance = new CAurora(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CAurora");
		Safe_Release<CAurora*>(pInstance);
	}

	return pInstance;
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
