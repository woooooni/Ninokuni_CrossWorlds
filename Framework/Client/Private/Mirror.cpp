#include "stdafx.h"
#include "..\Public\Mirror.h"
#include "GameInstance.h"

#include "Game_Manager.h"
#include "Player.h"

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

	// UI
	CPlayer* pPlayer = nullptr;
	pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return E_FAIL;
	CCharacter* pCharacter = nullptr;
	pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

	_float3 vCamPos, vLook, vUp;

	switch (pCharacter->Get_CharacterType())
	{
	case CHARACTER_TYPE::SWORD_MAN:
		vCamPos = _float3(0.f, 0.9f, -3.f);
		vLook = _float3(0.f, 0.9, 0.f);
		vUp = _float3(0.f, 1.f, 0.f);
		break;

	case CHARACTER_TYPE::ENGINEER:
		vCamPos = _float3(0.f, 0.7f, -2.3f);
		vLook = _float3(0.f, 0.7, 0.f);
		vUp = _float3(0.f, 1.f, 0.f);
		break;

	case CHARACTER_TYPE::DESTROYER:
		vCamPos = _float3(0.f, 0.9f, -3.f);
		vLook = _float3(0.f, 0.9, 0.f);
		vUp = _float3(0.f, 1.f, 0.f);
		break;
	}

	m_vCamMatrix = XMMatrixLookAtLH(XMLoadFloat3(&vCamPos), XMLoadFloat3(&vLook), XMLoadFloat3(&vUp));
	m_vCamPosition = XMVectorSet(vCamPos.x, vCamPos.y, vCamPos.z, 1.f);

	::XMStoreFloat4x4(&m_ViewMatrix, m_vCamMatrix);
	m_ProjMatrix = XMMatrixPerspectiveFovLH(::XMConvertToRadians(60.0f), static_cast<_float>(g_iWinSizeX) / static_cast<_float>(g_iWinSizeY), 0.2f, 1000.0f);

	m_pTransformCom->Set_Scale(_float3(1.f, 1.1f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(190.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(-0.3f, 0.f, 0.3f, 1.f));

	return S_OK;
}

void CMirror::Tick(_float fTimeDelta)
{
}

void CMirror::LateTick(_float fTimeDelta)
{
	if (true == m_bActive)
	{
		// Stencil Buffer Reference를 1.0로 초기화한다.
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_STENCIL_ONLY, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDERGROUP::RENDER_MIRROR, this);
	}
}

HRESULT CMirror::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("viewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("projectionMatrix", &GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))))
		return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("projectionMatrix", &m_ProjMatrix)))
	//	return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "ShaderTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMirror::Render_Reflect()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("worldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("viewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("projectionMatrix", &GI->Get_TransformFloat4x4(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))))
		return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_Matrix("projectionMatrix", &m_ProjMatrix)))
	//	return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "ShaderTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
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

}
