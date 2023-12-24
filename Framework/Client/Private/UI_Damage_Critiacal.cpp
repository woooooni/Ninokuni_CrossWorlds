#include "stdafx.h"
#include "UI_Damage_Critical.h"
#include "GameInstance.h"

CUI_Damage_Critical::CUI_Damage_Critical(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType)
	: CUI(pDevice, pContext, L"UI_Damage_Critiacal")
	, m_eFontType(eType)
{
}

CUI_Damage_Critical::CUI_Damage_Critical(const CUI_Damage_Critical& rhs)
	: CUI(rhs)
	, m_eFontType(rhs.m_eFontType)
{
}

HRESULT CUI_Damage_Critical::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Damage_Critical::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

//	CUI::UI_INFO UIDesc = {};
//	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
//	UIDesc.fX = g_iWinSizeX * 0.5f;
//	UIDesc.fY = g_iWinSizeY * 0.5f;
//	UIDesc.fCX = 112.f;
//	UIDesc.fCY = 112.f;
//	UIDesc.pParent = nullptr;
//	UIDesc.pDesc = nullptr;
//	Set_UI_Info(UIDesc);

	memcpy(&m_FontDesc, pArg, sizeof(DAMAGE_DESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetTransform = m_FontDesc.pTargetTransform;
	m_iDamage = m_FontDesc.iDamage;
	if (nullptr == m_pTargetTransform)
		return E_FAIL;
	m_vTargetPosition = Get_ProjectionPosition(m_pTargetTransform);
	m_fRandomOffset = _float2(GI->RandomFloat(0.f, 100.f), GI->RandomFloat(0.f, 60.f));

	m_tInfo.fX = m_vTargetPosition.x + m_fRandomOffset.x;
	m_tInfo.fY = m_vTargetPosition.y + m_fRandomOffset.y;

	_float fNumSize = 112.f * 0.3f;
	m_pTransformCom->Set_Scale(XMVectorSet(fNumSize, fNumSize, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	m_fOffsetX = fNumSize * 0.6f;
	m_fArrivedPosY = m_tInfo.fY - 100.f;

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fX = m_tInfo.fX;
	UIDesc.fY = m_tInfo.fY;
	UIDesc.fCX = fNumSize;
	UIDesc.fCY = fNumSize;
	UIDesc.pParent = nullptr;
	UIDesc.pDesc = nullptr;
	Set_UI_Info(UIDesc);

	m_bFadeOut = false;
	m_fAlpha = 1.f;

	return S_OK;
}

void CUI_Damage_Critical::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!Is_Dead())
		{
			if (!m_bFadeOut)
			{
				// FadeOut상태가 아니라면 시간을 누적한다.
				m_fFadeTimeAcc += fTimeDelta;

				if (m_fFadeTimeAcc > 0.3f)
				{
					m_bFadeOut = true;
					m_fFadeTimeAcc = 0.f;
				}
			}
			else // FadeOut 상태라면.
			{
				m_tInfo.fY -= fTimeDelta * 100.f;
				m_fAlpha -= fTimeDelta;
				if (m_fAlpha <= 0.f)
					m_fAlpha = 0.f;

				if (m_tInfo.fY <= m_fArrivedPosY)
				{
					m_tInfo.fY = m_fArrivedPosY;
				}

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}

			__super::Tick(fTimeDelta);
		}
	}

}

void CUI_Damage_Critical::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (Is_Dead())
			return;


		if (0 == m_iDamage || 999999 < m_iDamage)
			return;

		if (m_tInfo.fY == m_fArrivedPosY)
			Set_Dead(true);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Damage_Critical::Render()
{
	if (0 == m_iDamage || 999999 < m_iDamage) // Damage변수를 가지고 숫자가 총 몇자리인지 파악하여 Render할 것이다.
		return E_FAIL;

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_Damage_Critical::Resize_Scale()
{
	// Resize를 시작한다.

	m_bResize = true;

	m_tInfo.fCX = m_fMaxScale;
	m_tInfo.fCY = m_fMaxScale;
}

HRESULT CUI_Damage_Critical::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Critical"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Damage_Critical::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Damage_Critical::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

CUI_Damage_Critical* CUI_Damage_Critical::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType)
{
	CUI_Damage_Critical* pInstance = new CUI_Damage_Critical(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Damage_Critical");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Damage_Critical::Clone(void* pArg)
{
	CUI_Damage_Critical* pInstance = new CUI_Damage_Critical(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Damage_Critical");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Damage_Critical::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
