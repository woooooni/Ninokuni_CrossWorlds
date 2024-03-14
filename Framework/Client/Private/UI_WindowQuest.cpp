#include "stdafx.h"
#include "UI_WindowQuest.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CUI_WindowQuest::CUI_WindowQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_WindowQuest")
{
}

CUI_WindowQuest::CUI_WindowQuest(const CUI_WindowQuest& rhs)
	: CUI(rhs)
{
}

void CUI_WindowQuest::Set_Active(_bool bActive)
{
	if (bActive)
	{
		m_tInfo.fX = m_vOffPosition.x;
		m_tInfo.fY = m_vOffPosition.y; // UI가 출발할 Position으로 다시 위치를 세팅한다.
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

		m_fAlpha = 0.1f; // 초기 알파값을 세팅해준다.
		m_bShowInfo = false;
		m_bActive = true;

		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(true);
		}

	}
	else
	{
		m_bShowInfo = false;
		m_bActive = false;

		for (auto& pChildUI : m_pChild)
		{
			if (nullptr != pChildUI)
				pChildUI->Set_Active(false);
		}

	}
}

HRESULT CUI_WindowQuest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WindowQuest::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	// TestCode
	m_eProcessType = QUEST_ACCEPT;
	
	m_fAlpha = 0.3f;

	m_vOnPosition.x = m_tInfo.fX; // UI가 이동할 최종 위치
	m_vOnPosition.y = m_tInfo.fY;

	m_vOffPosition.x = m_tInfo.fX + (m_tInfo.fCX * 0.5f); // UI가 Active되는 시점에 등장할 위치
	m_vOffPosition.y = m_tInfo.fY;

	// ChildUI를 추가한다 (Accept버튼)
	Make_Child(0.f, g_iWinSizeY * 0.41f, 400.f, 100.f, TEXT("Prototype_GameObject_UI_Btn_AcceptQuest"));
	
	switch (m_eProcessType)
	{
	case QUEST_ACCEPT:
		Make_Child(0.f, g_iWinSizeY * 0.41f, 188.f * 0.6f, 53.f * 0.6f, TEXT("Prototype_GameObject_UI_Quest_Text_Accept"));
		break;

	case QUEST_FINISH:
		Make_Child(0.f, g_iWinSizeY * 0.41f, 250.f * 0.6f, 53.f * 0.6f, TEXT("Prototype_GameObject_UI_Quest_Text_Finish"));
		break;

	default:
		break;
	}

	m_bActive = false;

	return S_OK;
}

void CUI_WindowQuest::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{

		if (!m_bShowInfo)
		{
			if (m_fAlpha > 0.9f)
				m_fAlpha = 0.9f;
			else
				m_fAlpha += fTimeDelta;

			if (m_tInfo.fX < m_vOnPosition.x) // 목표 위치 이상으로 넘어가지 않도록 한다.
			{
				m_bShowInfo = true;

				m_tInfo.fX = m_vOnPosition.x;
			}
			else
			{
				m_tInfo.fX -= fTimeDelta * 200.f;
			}

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
		}
		
	}

	__super::Tick(fTimeDelta);
}

void CUI_WindowQuest::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}

	__super::LateTick(fTimeDelta);
}

HRESULT CUI_WindowQuest::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_WindowQuest::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestWindow"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_WindowQuest::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_WindowQuest::Bind_ShaderResources()
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

CUI_WindowQuest* CUI_WindowQuest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WindowQuest* pInstance = new CUI_WindowQuest(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_WindowQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WindowQuest::Clone(void* pArg)
{
	CUI_WindowQuest* pInstance = new CUI_WindowQuest(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_WindowQuest");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WindowQuest::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
