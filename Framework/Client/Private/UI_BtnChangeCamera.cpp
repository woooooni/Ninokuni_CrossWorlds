#include "stdafx.h"
#include "UI_BtnChangeCamera.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_BtnChangeCamera::CUI_BtnChangeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnShowMenu")
{
}

CUI_BtnChangeCamera::CUI_BtnChangeCamera(const CUI_BtnChangeCamera& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BtnChangeCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnChangeCamera::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	return S_OK;
}

void CUI_BtnChangeCamera::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		// Temp
		switch (m_iTextureIndex)
		{
		case 0:
			// 카메라 전환 1
			break;

		case 1:
			// 카메라 전환 2
			break;

		case 2:
			// 카메라 전환 3
			break;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnChangeCamera::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnChangeCamera::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

void CUI_BtnChangeCamera::On_MouseEnter(_float fTimeDelta)
{
	// 사이즈 조절
}

void CUI_BtnChangeCamera::On_Mouse(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
}

void CUI_BtnChangeCamera::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_BtnChangeCamera::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChangeCamera"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnChangeCamera::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnChangeCamera::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

void CUI_BtnChangeCamera::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (0 <= m_iTextureIndex && m_iTextureIndex <= 1)
			m_iTextureIndex++;
		else if (2 == m_iTextureIndex)
			m_iTextureIndex = 0;
	}
}

CUI_BtnChangeCamera* CUI_BtnChangeCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnChangeCamera* pInstance = new CUI_BtnChangeCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnChangeCamera::Clone(void* pArg)
{
	CUI_BtnChangeCamera* pInstance = new CUI_BtnChangeCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnChangeCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
