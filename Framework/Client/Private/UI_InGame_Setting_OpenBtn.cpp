#include "stdafx.h"
#include "UI_InGame_Setting_OpenBtn.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_InGame_Setting_OpenBtn::CUI_InGame_Setting_OpenBtn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_OpenBtn")
{
}

CUI_InGame_Setting_OpenBtn::CUI_InGame_Setting_OpenBtn(const CUI_InGame_Setting_OpenBtn& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_InGame_Setting_OpenBtn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_OpenBtn::Initialize(void* pArg)
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
	m_vHidePosition.x = g_iWinSizeX + m_tInfo.fCX * 0.5f;
	m_vHidePosition.y = m_tInfo.fY;

	m_bUseMouse = true;

	return S_OK;
}

void CUI_InGame_Setting_OpenBtn::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_OpenBtn::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_InGame_Setting_OpenBtn::Render()
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

void CUI_InGame_Setting_OpenBtn::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_InGame_Setting_OpenBtn::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_InGame_Setting_OpenBtn::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_InGame_Setting_OpenBtn::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_OpenBtn"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_InGame_Setting_OpenBtn::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_OpenBtn::Bind_ShaderResources()
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

void CUI_InGame_Setting_OpenBtn::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
//		GI->Stop_Sound(CHANNELID::SOUND_UI);
//		GI->Play_Sound(TEXT("UI_Fx_MainHud_Btn_Inventory_1.mp3"), CHANNELID::SOUND_UI, GI->Get_ChannelVolume(CHANNELID::SOUND_UI));

		CUI_Manager::GetInstance()->OnOff_InGameSetWindow(true);
	}
}

void CUI_InGame_Setting_OpenBtn::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (m_tInfo.fX >= m_vHidePosition.x)
			{
				m_bHideFinish = true;
				m_tInfo.fX = m_vHidePosition.x;
			}
			else
			{
				m_tInfo.fX += fTimeDelta * m_fHideSpeed;

				if (m_fAlpha <= 0.f)
					m_fAlpha = 0.f;
				else
					m_fAlpha -= fTimeDelta;
			}
		}
		else // 드러낸다
		{
			if (m_tInfo.fX <= m_vOriginPosition.x)
			{
				m_bHideFinish = true;
				m_tInfo.fX = m_vOriginPosition.x;
			}
			else
			{
				m_tInfo.fX -= fTimeDelta * m_fHideSpeed;

				if (m_fAlpha >= 1.f)
					m_fAlpha = 1.f;
				else
					m_fAlpha += fTimeDelta;
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_InGame_Setting_OpenBtn* CUI_InGame_Setting_OpenBtn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_InGame_Setting_OpenBtn* pInstance = new CUI_InGame_Setting_OpenBtn(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_OpenBtn::Clone(void* pArg)
{
	CUI_InGame_Setting_OpenBtn* pInstance = new CUI_InGame_Setting_OpenBtn(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BasicButton");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_OpenBtn::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
