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
	if (nullptr == m_pTargetTransform)
		E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float2 fNumsize = _float2(0.f, 0.f);

	if (UI_DMG_FONTTYPE::DMG_GENERALATTACK == m_eFontType)
	{
		m_iDamage = m_FontDesc.iDamage;
		fNumsize.x = 112.f;
		fNumsize.y = fNumsize.x;

		m_fOffsetX = fNumsize.x * 0.6f;
	}
	else
	{
		m_iDamage = -1;

		fNumsize.x = 197.f;
		fNumsize.y = 62.f;
	}

	m_pTargetTransform = m_FontDesc.pTargetTransform;
	if (nullptr == m_pTargetTransform)
		return E_FAIL;
	m_vTargetPosition = Get_ProjectionPosition(m_pTargetTransform);

	m_tInfo.fX = m_vTargetPosition.x;
	m_tInfo.fY = m_vTargetPosition.y;

	m_pTransformCom->Set_Scale(XMVectorSet(fNumsize.x, fNumsize.y, 1.f, 0.f));
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
	m_bSetPosition = false;

	return S_OK;
}

void CUI_Damage_General::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!Is_Dead())
		{
			if (!m_bSetPosition)
			{
				m_bSetPosition = true;
				m_fRandomOffset = _float2(GI->RandomFloat(-70.f, 70.f), GI->RandomFloat(-200.f, 0.f));

				m_tInfo.fX += m_fRandomOffset.x;
				m_tInfo.fY += m_fRandomOffset.y;

				m_vTargetPosition.x = m_tInfo.fX;
				m_vTargetPosition.y = m_tInfo.fY;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}

			if (UI_DMG_FONTTYPE::DMG_GENERALATTACK == m_eFontType)
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

		if (UI_DMG_FONTTYPE::DMG_GENERALATTACK == m_eFontType)
		{
			if (0 == m_iDamage || 999999 < m_iDamage)
				return;

			//if (m_tInfo.fY == m_fArrivedPosY)
			//	Set_Dead(true);
		}
		else
		{

		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Damage_General::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	switch (m_eFontType)
	{
	case UI_DMG_FONTTYPE::DMG_GENERALATTACK:
		if (FAILED(Render_GeneralDamage()))
			return E_FAIL;
		break;

	case UI_DMG_FONTTYPE::DMG_MISS:
		if (FAILED(Render_Miss()))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_Damage_General::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch(m_eFontType)
	{
	case UI_DMG_FONTTYPE::DMG_GENERALATTACK:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_White"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_DMG_FONTTYPE::DMG_MISS:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Miss"),
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
	
	if (UI_DMG_FONTTYPE::DMG_MISS == m_eFontType)
	{
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CUI_Damage_General::Render_Miss()
{
	m_pShaderCom->Begin(m_iPass);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Damage_General::Render_GeneralDamage()
{
	if (100000 <= m_iDamage)
	{
		m_iTextNum = (_uint)m_iDamage / 100000;

		m_tInfo.fX = m_vTargetPosition.x;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10000 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 100000) / 10000);

		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (1000 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 10000) / 1000);

		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 2.f;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (100 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 1000) / 100);
		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 3.f;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (10 <= m_iDamage)
	{
		m_iTextNum = ((m_iDamage % 100) / 10);

		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 4.f;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

	if (0 < m_iDamage)
	{
		m_iTextNum = (m_iDamage % 10);

		m_tInfo.fX = m_vTargetPosition.x + m_fOffsetX * 5.f;
		m_tInfo.fY = m_vTargetPosition.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
			return E_FAIL;
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextNum)))
			return E_FAIL;

		m_pShaderCom->Begin(m_iPass);
		m_pVIBufferCom->Render();
	}

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
