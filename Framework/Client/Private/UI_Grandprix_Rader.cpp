#include "stdafx.h"
#include "UI_Grandprix_Rader.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Minigame_Basic.h"
#include "UIMinigame_Manager.h"

CUI_Grandprix_Rader::CUI_Grandprix_Rader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Grandprix_Rader")
{
}

CUI_Grandprix_Rader::CUI_Grandprix_Rader(const CUI_Grandprix_Rader& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Grandprix_Rader::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_Rader::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	CGameObject* pTemp = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Text_Error"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTemp)
		return E_FAIL;
	if (nullptr == dynamic_cast<CUI_Minigame_Basic*>(pTemp))
		return E_FAIL;
	m_pError = dynamic_cast<CUI_Minigame_Basic*>(pTemp);

	m_bActive = false;

	return S_OK;
}

void CUI_Grandprix_Rader::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (true == CUIMinigame_Manager::GetInstance()->Is_RaderError())
		{
			if (nullptr != m_pError)
				m_pError->Tick(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_Rader::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (true == CUIMinigame_Manager::GetInstance()->Is_RaderError())
		{
			if (nullptr != m_pError)
				m_pError->LateTick(fTimeDelta);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Grandprix_Rader::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(5);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Grandprix_Rader::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_Evermore_Grandprix_Rader_MaskTexture"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_Rader::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_Rader::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(GI->Bind_SRV(m_pShaderCom, TEXT("Target_MiniMap"), "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_MaskTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Grandprix_Rader* CUI_Grandprix_Rader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Grandprix_Rader* pInstance = new CUI_Grandprix_Rader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_Rader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_Rader::Clone(void* pArg)
{
	CUI_Grandprix_Rader* pInstance = new CUI_Grandprix_Rader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_Rader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_Rader::Free()
{
	__super::Free();

	Safe_Release(m_pError);
	Safe_Release(m_pTextureCom);
}
