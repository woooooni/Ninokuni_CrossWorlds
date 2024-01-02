#include "stdafx.h"
#include "UI_Damage_General.h"
#include "GameInstance.h"

CUI_Damage_General::CUI_Damage_General(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DMG_FONTTYPE eType)
	: CUI(pDevice, pContext, L"UI_Damage_General")
	, m_eFontType(eType)
{
}

CUI_Damage_General::CUI_Damage_General(const CUI_Damage_General& rhs)
	: CUI(rhs)
	, m_eFontType(rhs.m_eFontType)
{
}

HRESULT CUI_Damage_General::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Damage_General::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_FontDesc, pArg, sizeof(GENERAL_DESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float2 fNumsize = _float2(0.f, 0.f);

	switch (m_eFontType)
	{
	case DMG_MISS:
		fNumsize.x = 197.f;
		fNumsize.y = 62.f;
		break;

	case DMG_DODGE:
		fNumsize.x = 273.f;
		fNumsize.y = 62.f;
		break;
	}


	m_vTargetPosition = m_FontDesc.vPosition;
	m_tInfo.fX = m_vTargetPosition.x;
	m_tInfo.fY = m_vTargetPosition.y;

	m_pTransformCom->Set_Scale(XMVectorSet(fNumsize.x * 0.3f, fNumsize.y * 0.3f, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fX = m_tInfo.fX;
	UIDesc.fY = m_tInfo.fY;
	UIDesc.fCX = fNumsize.x;
	UIDesc.fCY = fNumsize.y;
	UIDesc.pParent = nullptr;
	UIDesc.pDesc = nullptr;
	Set_UI_Info(UIDesc);

	m_bFadeOut = false;
	m_fAlpha = 1.f;

	return S_OK;
}

void CUI_Damage_General::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!Is_Dead())
		{
			if (!m_bFadeOut)
			{
				m_fFadeTimeAcc += fTimeDelta;

				if (1.f <= m_fFadeTimeAcc)
				{
					m_fFadeTimeAcc = 0.f;
					m_bFadeOut = true;
				}
			}
			else
			{
				m_fAlpha -= fTimeDelta;

				if (m_fAlpha <= 0)
				{
					m_fAlpha = 0.f;
					Set_Dead(true);
				}
			}

			__super::Tick(fTimeDelta);
		}
	}

}

void CUI_Damage_General::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (Is_Dead())
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Damage_General::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Damage_General::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch(m_eFontType)
	{
	case UI_DMG_FONTTYPE::DMG_MISS:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Miss"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_DMG_FONTTYPE::DMG_DODGE:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Dodge"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_Damage_General::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Damage_General::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;


	return S_OK;
}


CUI_Damage_General* CUI_Damage_General::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DMG_FONTTYPE eType)
{
	CUI_Damage_General* pInstance = new CUI_Damage_General(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Damage_General");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Damage_General::Clone(void* pArg)
{
	CUI_Damage_General* pInstance = new CUI_Damage_General(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Damage_General");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Damage_General::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
