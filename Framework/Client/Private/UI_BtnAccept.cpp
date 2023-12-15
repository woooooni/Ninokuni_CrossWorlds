#include "stdafx.h"
#include "UI_BtnAccept.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_BtnAccept::CUI_BtnAccept(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_BtnAccept")
{
}

CUI_BtnAccept::CUI_BtnAccept(const CUI_BtnAccept& rhs)
	: CUI(rhs)
{
}

void CUI_BtnAccept::Set_Active(_bool bActive)
{
	if (bActive)
	{
		m_bResizeStart = false;
		m_bFinish = false;
	}

	m_bActive = bActive;
}

HRESULT CUI_BtnAccept::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_BtnAccept::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;
	
	m_vOriginSize.x = m_tInfo.fCX;
	m_vOriginSize.y = m_tInfo.fCY;

	m_vMinSize.x = m_vOriginSize.x * 0.95f;
	m_vMinSize.y = m_vOriginSize.y * 0.95f;

	return S_OK;
}

void CUI_BtnAccept::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_bResizeStart)
		{
			// 시간을 누적한다.
			m_fTimeAcc += fTimeDelta;

			// MinSize로 Info를 변경해서 setting한다.
			m_tInfo.fCX = m_vMinSize.x;
			m_tInfo.fCY = m_vMinSize.y;
			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_BtnAccept::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0.5f < m_fTimeAcc) // 누적한 시간이 기준치 이상이되면
		{
			m_fTimeAcc = 0.f; // 0.f로 초기화하고,

			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f)); // 원래 사이즈로 Set_Salce하고,
			
			m_bFinish = true; // Finish를 true로 변경해준다
			// Finish가 true가 되면 Event가 발생한다.
		}

		if (m_bFinish)
		{
			// UIManager를 통해서 Quest창을 Off한다
			if (FAILED(CUI_Manager::GetInstance()->OnOff_QuestWindow(false)))
				return;

			// 퀘스트를 받는다. (QuestManager)
			// PopUpWindow 갱신.
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_BtnAccept::Render()
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

void CUI_BtnAccept::On_MouseEnter(_float fTimeDelta)
{
	m_tInfo.fCX = m_vMinSize.x;
	m_tInfo.fCY = m_vMinSize.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
}

void CUI_BtnAccept::On_Mouse(_float fTimeDelta)
{
	Key_Input(fTimeDelta);
}

void CUI_BtnAccept::On_MouseExit(_float fTimeDelta)
{
	m_tInfo.fCX = m_vOriginSize.x;
	m_tInfo.fCY = m_vOriginSize.y;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
}

HRESULT CUI_BtnAccept::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Btn_Quest"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_BtnAccept::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_BtnAccept::Bind_ShaderResources()
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

void CUI_BtnAccept::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		if (!m_bResizeStart) // 리사이즈가 진행되지 않은 상황.
			m_bResizeStart = true;
	}
}

CUI_BtnAccept* CUI_BtnAccept::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_BtnAccept* pInstance = new CUI_BtnAccept(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_BtnAccept");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_BtnAccept::Clone(void* pArg)
{
	CUI_BtnAccept* pInstance = new CUI_BtnAccept(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_BtnAccept");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_BtnAccept::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
