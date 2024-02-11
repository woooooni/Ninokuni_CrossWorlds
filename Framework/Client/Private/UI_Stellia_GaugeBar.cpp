#include "stdafx.h"
#include "UI_Stellia_GaugeBar.h"
#include "GameInstance.h"

CUI_Stellia_GaugeBar::CUI_Stellia_GaugeBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Stellia_GaugeBar")
{
}

CUI_Stellia_GaugeBar::CUI_Stellia_GaugeBar(const CUI_Stellia_GaugeBar& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Stellia_GaugeBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Stellia_GaugeBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CUI::UI_INFO UIDesc = {};
	UIDesc.fCX = 200.f;
	UIDesc.fCY = 200.f;
	UIDesc.fX = 1200.f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
//	UIDesc.fY = 650.f;
	if (FAILED(__super::Initialize(&UIDesc)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_iMaxGauge = 100;
	m_iCurGauge = 0;

	m_bActive = true;

	return S_OK;
}

void CUI_Stellia_GaugeBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		if (m_iMaxDamage <= m_iCurDamage)
//		{
//			m_iCurDamage = m_iMaxDamage;
//
//			m_fTimeAcc += fTimeDelta; // 데미지가 다 차면 시간을 누적한다 -> 바로 사라지는 것은 어색함.
//			if (1.f < m_fTimeAcc)
//			{
//				m_fTimeAcc = 0.f;
//				Set_Active(false);
//				Set_Dead(true);
//			}
//		}
//
//		if (m_iCurDamage > m_iPreDamage)
//			m_bLerp = false;
//
//		if ((false == m_bLerp) && (m_iPreDamage < m_iCurDamage))
//		{
//			m_iPreDamage += fTimeDelta * (m_iMaxDamage / 9.f);
//
//			if (m_iPreDamage >= m_iCurDamage)
//			{
//				m_iPreDamage = m_iCurDamage;
//				m_bLerp = true;
//			}
//		}
//
//		m_fTextureAcc += fTimeDelta;
//
//		if (0.01f < m_fTextureAcc)
//		{
//			m_fTextureAcc = 0.f;
//
//			if (7 == m_iTextureIndex)
//				m_iTextureIndex = 0;
//			else
//				m_iTextureIndex++;
//		}

		//TestCode
		if (KEY_TAP(KEY::O))
			m_iCurGauge += 10;

		__super::Tick(fTimeDelta);
	}
}

void CUI_Stellia_GaugeBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Stellia_GaugeBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Stellia_GaugeBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Gauge_Background"),
		TEXT("Com_Texture_Background"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Gauge_Bar"),
		TEXT("Com_Texture"), (CComponent**)&m_pGaugeTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Stellia_GaugeBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Stellia_GaugeBar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;


	_float fRatio = _float(m_iMaxGauge - m_iCurGauge) / _float(m_iMaxGauge); // 값이 0이면 원 전체가 그려진다.
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_HPGaugeTexture")))
		return E_FAIL;
	if (FAILED(m_pGaugeTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

CUI_Stellia_GaugeBar* CUI_Stellia_GaugeBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Stellia_GaugeBar* pInstance = new CUI_Stellia_GaugeBar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Stellia_GaugeBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Stellia_GaugeBar::Clone(void* pArg)
{
	CUI_Stellia_GaugeBar* pInstance = new CUI_Stellia_GaugeBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Stellia_GaugeBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Stellia_GaugeBar::Free()
{
	__super::Free();

	Safe_Release(m_pGaugeTextureCom);
	Safe_Release(m_pTextureCom);
}
