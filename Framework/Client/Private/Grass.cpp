#include "Grass.h"
#include "GameInstance.h"

CGrass::CGrass(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_GRASS)
{
}

CGrass::CGrass(const CGrass& rhs)
	: CGameObject(rhs)
{
}

HRESULT CGrass::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CGrass::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CGrass::Tick(_float fTimeDelta)
{
}

void CGrass::LateTick(_float fTimeDelta)
{
}

HRESULT CGrass::Render()
{
	if (FAILED(Bind_ShaderResource()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render(5);

	return S_OK;
}

HRESULT CGrass::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGrass::Bind_ShaderResource()
{
	return S_OK;
}

CGrass* CGrass::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGrass* pInstance = new CGrass(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CGrass");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CGrass::Clone(void* pArg)
{
	CGrass* pInstance = new CGrass(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGrass");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CGrass::Free()
{
	__super::Free();

	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CVIBuffer_Grass*>(m_pVIBufferCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CTexture*>(m_pTextureCom);
}
