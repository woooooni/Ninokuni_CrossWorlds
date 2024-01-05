#include "stdafx.h"
#include "UI_Dummy_Engineer.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"
#include "Character_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_Dummy_Engineer::CUI_Dummy_Engineer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCharacter(pDevice, pContext, L"UI_Dummy_Engineer", CHARACTER_TYPE::ENGINEER)
{
}

CUI_Dummy_Engineer::CUI_Dummy_Engineer(const CUI_Dummy_Engineer& rhs)
	: CCharacter(rhs)
{

}

HRESULT CUI_Dummy_Engineer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CUI_Dummy_Engineer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(1.5f, 1.23f, -11.2f, 1.f));
	m_pTransformCom->LookAt_ForLandObject(Vec4(1.f, 1.23f, -30.f, 1.f));

	return S_OK;
}

void CUI_Dummy_Engineer::Tick(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(2.4f, 1.23f, -12.5f, 1.f));

	__super::Tick(fTimeDelta);
}

void CUI_Dummy_Engineer::LateTick(_float fTimeDelta)
{
//	m_pModelCom->LateTick(fTimeDelta);
//	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Dummy_Engineer::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };
	if (m_bInfinite)
	{
		vRimColor = { 0.f, 0.5f, 1.f, 1.f };
	}


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;


	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pCharacterPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pCharacterPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pCharacterPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pCharacterPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pCharacterPartModels[i]->Render(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	return S_OK;
}


HRESULT CUI_Dummy_Engineer::Ready_Components()
{

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel" ), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_SwordMan_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
//	
//	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Engineer_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
//		return E_FAIL;
//
//	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Destroyer_Dummy"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
//		return E_FAIL;

	for (_uint i = 0; i < PART_TYPE::PART_END; ++i)
 		m_pCharacterPartModels[i] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::SWORD_MAN, PART_TYPE(i), 0);

	m_pModelCom->Set_Animation(22);

	return S_OK;
}

#pragma region Ready_States
HRESULT CUI_Dummy_Engineer::Ready_States()
{
	return S_OK;
}

#pragma endregion


#pragma region Ready_Colliders
HRESULT CUI_Dummy_Engineer::Ready_Colliders()
{
	return S_OK;
}

#pragma endregion

#pragma region Ready_Sockets
HRESULT CUI_Dummy_Engineer::Ready_Sockets()
{
	return S_OK;
}
#pragma endregion

#pragma region Ready_Parts
HRESULT CUI_Dummy_Engineer::Ready_Parts()
{
	return S_OK;
}
#pragma endregion

_float2 CUI_Dummy_Engineer::Transpose_ProjectionPosition()
{
	_vector vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	_float4x4 matWorld = m_pTransformCom->Get_WorldFloat4x4();
	_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	_float4x4 matWindow;
	XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;
	_float fScreenX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
	_float fScreenY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

	return _float2(fScreenX, fScreenY);
}

CUI_Dummy_Engineer* CUI_Dummy_Engineer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Dummy_Engineer* pInstance = new CUI_Dummy_Engineer(pDevice, pContext);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CUI_Dummy_Engineer");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CUI_Dummy_Engineer::Clone(void* pArg)
{
	CUI_Dummy_Engineer* pInstance = new CUI_Dummy_Engineer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Dummy_Engineer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Dummy_Engineer::Free()
{
	__super::Free();
}

