#include "stdafx.h"
#include "..\Public\Mirror.h"
#include "GameInstance.h"


CMirror::CMirror(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CGameObject(pDevice, pContext, strObjectTag, eType)
{
}

CMirror::CMirror(const CMirror& rhs)
	: CGameObject(rhs)
{
}

HRESULT CMirror::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMirror::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Shader_State()))
		return E_FAIL;

	return S_OK;
}

void CMirror::Tick(_float fTimeDelta)
{
	SimpleMath::Plane ReflectPlane = Vec4(0.0f, 0.0f, 1.0f, -4.9f);
	XMMATRIX ReflectMatrix = Matrix::CreateReflection(ReflectPlane);

	XMMATRIX world = m_pTransformCom->Get_WorldMatrix();

	Matrix result = world * ReflectMatrix;
}

void CMirror::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_NONBLEND_UI, this);
}

HRESULT CMirror::Render()
{
	if (FAILED(m_pShaderCom->Bind_RawValue("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("viewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("projectionMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	// TODO Mirror Mark DepthStencilState
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState, 1);

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "ShaderTexture")))
			return E_FAIL;

		//if (FAILED(m_pModelCom->SetUp_OnShader(pShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	iPassIndex = 0;
		//else
		//	iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMirror::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Mirror"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Mirror"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMirror::Ready_Shader_State()
{
	D3D11_DEPTH_STENCIL_DESC DSDESC;
	::ZeroMemory(&DSDESC, sizeof(D3D11_DEPTH_STENCIL_DESC));
	{
		DSDESC.DepthEnable = true;
		DSDESC.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		DSDESC.DepthFunc = D3D11_COMPARISON_LESS;
		DSDESC.StencilEnable = true;
		DSDESC.StencilReadMask = 0xff;
		DSDESC.StencilWriteMask = 0xff;
		// 이전 설정
		DSDESC.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DSDESC.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DSDESC.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		DSDESC.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

		// 면 설정 래스터화 상태에서 뒷면을 컬링할 때 이 설정은 쓸모가 없지만 그래도 설정 해야한다.
		// 그렇지 않으면 깊이 상태를 생성할 수 없음.
		DSDESC.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		DSDESC.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		DSDESC.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		DSDESC.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	}
	if (FAILED(m_pDevice->CreateDepthStencilState(&DSDESC, &m_pDepthStencilState)))
		return E_FAIL;

	return S_OK;
}

CMirror* CMirror::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CMirror* pInstance = new CMirror(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CMirror");
		Safe_Release<CMirror*>(pInstance);
	}

	return pInstance;
}

CGameObject* CMirror::Clone(void* pArg)
{
	CMirror* pInstance = new CMirror(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMirror");
		Safe_Release<CMirror*>(pInstance);
	}

	return pInstance;
}

void CMirror::Free()
{
	__super::Free();

	Safe_Release<CShader*>(m_pShaderCom);
	Safe_Release<CRenderer*>(m_pRendererCom);
	Safe_Release<CTransform*>(m_pTransformCom);
	Safe_Release<CModel*>(m_pModelCom);

	Safe_Release<ID3D11DepthStencilState*>(m_pDepthStencilState);

}
