#include "stdafx.h"
#include "..\Public\Moon.h"
#include "GameInstance.h"
#include "Quest_Manager.h"

CMoon::CMoon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CMoon::CMoon(const CMoon& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMoon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMoon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CMoon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);


}

void CMoon::LateTick(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::C) && KEY_HOLD(KEY::D))
		m_bIsRedColor = true;

	if (KEY_HOLD(KEY::C) && KEY_HOLD(KEY::S))
		m_bIsRedColor = false;

	if (true == m_bIsRedColor)
		m_fWeight += m_fSpeed * fTimeDelta;

	//if (true == m_bAppearMoon)
	//{
		_matrix Temp = XMLoadFloat4x4(&m_pTransformCom->Get_WorldFloat4x4());
		_vector vPosition = Temp.r[CTransform::STATE_POSITION];
		_vector vCamPosition = XMLoadFloat4(&GI->Get_CamPosition());

		_float fLookScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_LOOK]));
		_vector vLook = XMVectorSetW(XMVector3Normalize(vPosition - vCamPosition), 0.f) * fLookScale;

		_float fRightScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_RIGHT]));
		_vector vRight = XMVectorSetW(XMVector3Normalize(XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook)), 0.f) * fRightScale;

		_float fUpScale = XMVectorGetX(XMVector3Length(Temp.r[CTransform::STATE_UP]));
		_vector vUp = XMVectorSetW(XMVector3Normalize(XMVector3Cross(vLook, vRight)), 0.f) * fUpScale;

		Temp.r[CTransform::STATE_RIGHT] = vRight;
		Temp.r[CTransform::STATE_UP] = vUp;
		Temp.r[CTransform::STATE_LOOK] = vLook;
		Temp.r[CTransform::STATE_POSITION] = m_pTransformCom->Get_Position();

		m_pTransformCom->Set_WorldMatrix(Temp);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND, this);
	//}
}

HRESULT CMoon::Render()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL; 
	if (FAILED(m_pShaderCom->Bind_RawValue("bStartChange", &m_bIsRedColor, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::DIFFUSE_TEX]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;
	if (FAILED(m_pTextureCom[TEX_TYPE::RED_TEX]->Bind_ShaderResource(m_pShaderCom, "g_RedTexture", 0)))
		return E_FAIL;

	//_int iIndex = m_pTextureCom[TEX_TYPE::DISSOLVE_TEX]->Find_Index(TEXT("T_360LINE_MASK01"));
	if (FAILED(m_pTextureCom[TEX_TYPE::DISSOLVE_TEX]->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture", 0)))
		return E_FAIL;

	const Vec4 vBloomColor = Vec4(0.3f, 0.3f, 0.3f, 1.0f);
	if (FAILED(m_pShaderCom->Bind_RawValue("vBloomColor", &vBloomColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("fTime", &m_fTime, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("fDissolveWeight", &m_fWeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMoon::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Moon_Diffuse"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::DIFFUSE_TEX]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Moon_Red"),
		TEXT("Com_Texture1"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::RED_TEX]))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Gradation_Dissolve"),
		TEXT("Com_Texture2"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_TYPE::DISSOLVE_TEX]))))
		return E_FAIL;

	return S_OK;
}

CMoon* CMoon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CMoon* pInstance = new CMoon(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMoon");
		Safe_Release<CMoon*>(pInstance);
	}

	return pInstance;
}

CGameObject* CMoon::Clone(void* pArg)
{
	CMoon* pInstance = new CMoon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMoon");
		Safe_Release<CMoon*>(pInstance);
	}

	return pInstance;
}

void CMoon::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CVIBuffer_Rect*>(m_pVIBufferCom);

	for(_uint i = 0; i < TEX_TYPE::TEX_END; ++i)
		Safe_Release<CTexture*>(m_pTextureCom[i]);
}
