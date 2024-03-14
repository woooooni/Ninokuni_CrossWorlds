#include "stdafx.h"
#include "UI_PlayerEXPBar.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Character.h"

CUI_PlayerEXPBar::CUI_PlayerEXPBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EXPTYPE eType)
	: CUI(pDevice, pContext, L"UI_PlayerEXPBar")
	, m_eType(eType)
{
}

CUI_PlayerEXPBar::CUI_PlayerEXPBar(const CUI_PlayerEXPBar& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

HRESULT CUI_PlayerEXPBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bHide = false;
	m_bHideFinish = false;
	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);
	m_vHidePosition.x = m_tInfo.fX;
	m_vHidePosition.y = g_iWinSizeY + m_tInfo.fCY * 0.5f;

	m_pPlayer = CUI_Manager::GetInstance()->Get_Character();

	if (nullptr != m_pPlayer)
	{
		if (m_eType == UIEXP_BAR)
		{
			m_iPass = 4;

			m_fMaxEXP = _float(m_pPlayer->Get_Stat().iMaxExp);
			m_fCurEXP = _float(m_pPlayer->Get_Stat().iExp);
			m_fPreEXP = m_fCurEXP;
			m_fProgress = 0.f;
			m_fTimeAcc = 0.f;
			m_iPreLevel = m_pPlayer->Get_Stat().iLevel;
		}
	}

	return S_OK;
}

void CUI_PlayerEXPBar::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		if (nullptr == m_pPlayer)
		{
			if (nullptr == CUI_Manager::GetInstance()->Get_Character())
				return;

			m_pPlayer = CUI_Manager::GetInstance()->Get_Character();
			m_fMaxEXP = _float(m_pPlayer->Get_Stat().iMaxExp);
			m_fCurEXP = _float(m_pPlayer->Get_Stat().iExp);
			m_fPreEXP = m_fCurEXP;
			m_fProgress = 0.f;
		}

		if (UIEXP_BAR == m_eType)
		{
			if (m_iPreLevel == m_pPlayer->Get_Stat().iLevel)
			{
				m_fTimeAcc += fTimeDelta * 0.5f;
				m_fCurEXP = _float(m_pPlayer->Get_Stat().iExp);

				if (m_fCurEXP > m_fPreEXP)
					m_bLerp = false;

				if (false == m_bLerp && m_fPreEXP < m_fCurEXP)
				{
					_float m_fRatio = m_fCurEXP / m_fMaxEXP;

					m_fPreEXP += m_fTimeAcc;
					m_fProgress = m_fPreEXP / m_fMaxEXP;

					if (m_fPreEXP >= m_fCurEXP)
					{
						m_fTimeAcc = 0.f;
						m_fPreEXP = m_fCurEXP;
						m_fProgress = m_fRatio;
						m_bLerp = true;
					}
				}
			}
			else if (m_iPreLevel < m_pPlayer->Get_Stat().iLevel)
			{
				// EXPBar가 저장했던 Level이 Player의 현재 레벨과 다를 때 (작을 때)
				m_fTimeAcc += fTimeDelta * 0.5f;
				m_fPreEXP += m_fTimeAcc;

				if (m_fPreEXP >= m_fMaxEXP)
					m_fPreEXP = m_fMaxEXP;

				m_fProgress = m_fPreEXP / m_fMaxEXP;

				if (m_fProgress == 1.f) // 바가 다 차면 정보를 갱신한다.
				{
					// 0에서 m_fCurEXP로 올리는 부분 추가해야함
					m_iPreLevel = m_pPlayer->Get_Stat().iLevel;
					m_fMaxEXP = m_pPlayer->Get_Stat().iMaxExp;
					m_fCurEXP = m_pPlayer->Get_Stat().iExp;
					m_fPreEXP = m_fCurEXP;

					m_fTimeAcc = 0.f;
					m_fPreEXP = m_fCurEXP;
					m_fProgress = m_fCurEXP / m_fMaxEXP;
					m_bLerp = true;
				}
			}
		}
		__super::Tick(fTimeDelta);
	}
}

void CUI_PlayerEXPBar::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pPlayer)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_PlayerEXPBar::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UIEXP_BACKGROUND:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus_EXP_Background"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UIEXP_BAR:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus_EXP_Bar"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}
	
	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_PlayerEXPBar::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (m_eType == UIEXP_BAR)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LoadingProgress", &m_fProgress, sizeof(_float))))
			return E_FAIL;
	}

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_PlayerEXPBar::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (m_tInfo.fY >= m_vHidePosition.y)
			{
				m_bHideFinish = true;
				m_tInfo.fY = m_vHidePosition.y;
			}
			else
			{
				m_tInfo.fY += fTimeDelta * m_fHideSpeed;
			}
		}
		else // 드러낸다
		{
			if (m_tInfo.fY <= m_vOriginPosition.y)
			{
				m_bHideFinish = true;
				m_tInfo.fY = m_vOriginPosition.y;
			}
			else
			{
				m_tInfo.fY -= fTimeDelta * m_fHideSpeed;
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_PlayerEXPBar* CUI_PlayerEXPBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EXPTYPE eType)
{
	CUI_PlayerEXPBar* pInstance = new CUI_PlayerEXPBar(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_PlayerEXPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_PlayerEXPBar::Clone(void* pArg)
{
	CUI_PlayerEXPBar* pInstance = new CUI_PlayerEXPBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_PlayerEXPBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_PlayerEXPBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
