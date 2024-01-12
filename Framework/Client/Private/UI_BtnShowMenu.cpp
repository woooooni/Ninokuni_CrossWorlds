#include "stdafx.h"
#include "UI_BtnShowMenu.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "UI_Manager.h"

CUI_BtnShowMenu::CUI_BtnShowMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnShowMenu")
{
}

CUI_BtnShowMenu::CUI_BtnShowMenu(const CUI_BtnShowMenu& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_BtnShowMenu::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnShowMenu::Initialize(void* pArg)
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

	return S_OK;
}

void CUI_BtnShowMenu::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		Movement_BasedOnHiding(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnShowMenu::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnShowMenu::Render()
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

void CUI_BtnShowMenu::On_MouseEnter(_float fTimeDelta)
{
	// 사이즈 조절
}

void CUI_BtnShowMenu::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_TAP(KEY::LBTN))
		{
			// UIManager에 기본으로 Scene에 생성되는 UI들이 자연스럽게 사라지고 등장할 수 있는 함수를 생성한다.
			GI->Stop_Sound(CHANNELID::SOUND_UI2);
			GI->Play_Sound(TEXT("UI_Fx_Comm_Btn_1.mp3"), CHANNELID::SOUND_UI2, GI->Get_ChannelVolume(CHANNELID::SOUND_UI2));

			CUI_Manager::GetInstance()->OnOff_GamePlaySetting(false);
			CUI_Manager::GetInstance()->OnOff_MainMenu(true);
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_BtnShowMenu::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

HRESULT CUI_BtnShowMenu::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowMenu"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnShowMenu::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnShowMenu::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	return S_OK;
}

void CUI_BtnShowMenu::Movement_BasedOnHiding(_float fTimeDelta)
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

CUI_BtnShowMenu* CUI_BtnShowMenu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnShowMenu* pInstance = new CUI_BtnShowMenu(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnShowMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnShowMenu::Clone(void* pArg)
{
	CUI_BtnShowMenu* pInstance = new CUI_BtnShowMenu(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnShowMenu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnShowMenu::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
