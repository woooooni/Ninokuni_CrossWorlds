#include "stdafx.h"
#include "UI_Damage_Critical.h"
#include "GameInstance.h"

CUI_Damage_Critical::CUI_Damage_Critical(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Damage_Critiacal")
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

	memcpy(&m_FontDesc, pArg, sizeof(CRITICAL_DESC));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTargetTransform = m_FontDesc.pTargetTransform;
	if (nullptr == m_pTargetTransform)
		return E_FAIL;
	m_vTargetPosition = Get_ProjectionPosition(m_pTargetTransform);
	m_tInfo.fX = m_vTargetPosition.x;
	m_tInfo.fY = m_vTargetPosition.y;

	m_eFontType = m_FontDesc.eType;

	_float fRandom = GI->RandomFloat(0.2f, 0.4f);
	_float2 fNumSize = _float2(256.f * fRandom, 57.f * fRandom);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	//m_fArrivedPosY = m_tInfo.fY - 100.f;

	CUI::UI_INFO UIDesc = {};
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));
	UIDesc.fX = m_tInfo.fX;
	UIDesc.fY = m_tInfo.fY;
	UIDesc.fCX = fNumSize.x;
	UIDesc.fCY = fNumSize.y;
	UIDesc.pParent = nullptr;
	UIDesc.pDesc = nullptr;
	Set_UI_Info(UIDesc);

	m_fMaxScale = _float2(fNumSize.x * 1.8f, fNumSize.y * 1.8f);
	m_fOriginScale = _float2(fNumSize.x, fNumSize.y);

	m_bFadeOut = false;
	m_bResize = false;
	m_bSetPosition = false;
	m_fAlpha = 1.f;

	m_tInfo.fCX = m_fMaxScale.x;
	m_tInfo.fCY = m_fMaxScale.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	
	return S_OK;
}

void CUI_Damage_Critical::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!Is_Dead())
		{
			if (!m_bSetPosition)
			{
				m_bSetPosition = true;
				m_fRandomOffset = _float2(GI->RandomFloat(-70.f, 70.f), GI->RandomFloat(-200.f, -100.f));

				m_vTargetPosition.x += m_fRandomOffset.x;
				m_vTargetPosition.y += m_fRandomOffset.y;

				m_tInfo.fX = m_vTargetPosition.x;
				m_tInfo.fY = m_vTargetPosition.y;
				m_fArrivedPosY = m_tInfo.fY - 100.f;

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
			}
			else
			{
				if (!m_bResize)
				{
					// 사이즈를 줄인다
					m_tInfo.fCX -= fTimeDelta * (m_tInfo.fCX * 2.f);
					m_tInfo.fCY -= fTimeDelta * (m_tInfo.fCY * 2.f);

					if (m_tInfo.fCX <= m_fOriginScale.x)
					{
						m_tInfo.fCX = m_fOriginScale.x;
						m_tInfo.fCY = m_fOriginScale.y;

						m_bResize = true;

					}
				}
				else // 사이즈 줄이는 것이 끝났으면
				{
					if (!m_bFadeOut)
					{
						m_tInfo.fY -= fTimeDelta * 200.f;
						m_fAlpha -= fTimeDelta;

						if (m_fAlpha <= 0.f)
						{
							m_fAlpha = 0.f;
							Set_Dead(true);
						}

						if (m_tInfo.fY < m_fArrivedPosY)
						{
							m_tInfo.fY = m_fArrivedPosY;
							m_bFadeOut = true;
							Set_Dead(true);
						}
					}
				}

				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
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

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Damage_Critical::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(m_iPass);

	m_pVIBufferCom->Render();

	return S_OK;
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

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eFontType))))
		return E_FAIL;

	return S_OK;
}

CUI_Damage_Critical* CUI_Damage_Critical::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Damage_Critical* pInstance = new CUI_Damage_Critical(pDevice, pContext);

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
