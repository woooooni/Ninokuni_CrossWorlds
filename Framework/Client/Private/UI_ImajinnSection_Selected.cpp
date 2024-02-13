#include "stdafx.h"
#include "UI_ImajinnSection_Selected.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Weapon_Manager.h"
#include "Player.h"
#include "Weapon.h"

CUI_ImajinnSection_Selected::CUI_ImajinnSection_Selected(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_ImajinnSection_Selected")
{
}

CUI_ImajinnSection_Selected::CUI_ImajinnSection_Selected(const CUI_ImajinnSection_Selected& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ImajinnSection_Selected::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_Selected::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMaxSize = _float2(m_vOriginSize.x * 1.3f, m_vOriginSize.y * 1.3f);

	m_bActive = true; 

//	m_bHide = false;
//	m_bHideFinish = false;
//	m_vOriginPosition = _float2(m_tInfo.fX, m_tInfo.fY);

	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayer)
		return E_FAIL;
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return E_FAIL;

	Update_Position(pCharacter->Get_CharacterType());

	return S_OK;
}

void CUI_ImajinnSection_Selected::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;
		CCharacter* pCharacter = pPlayer->Get_Character();
		if (nullptr == pCharacter)
			return;

		Update_Position(pCharacter->Get_CharacterType());

		//Movement_BasedOnHiding(fTimeDelta);

//		if (!m_bResize)
//		{
//			m_tInfo.fCX += fTimeDelta * m_fSpeed;
//			m_tInfo.fCY += fTimeDelta * m_fSpeed;
//
//			if (m_tInfo.fCX >= m_vMaxSize.x)
//			{
//				m_bResize = true;
//				m_tInfo.fCX = m_vMaxSize.x;
//				m_tInfo.fCY = m_vMaxSize.y;
//			}
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//		}
//		else
//		{
//			m_tInfo.fCX -= fTimeDelta * m_fSpeed;
//			m_tInfo.fCY -= fTimeDelta * m_fSpeed;
//
//			if (m_tInfo.fCX <= m_vOriginSize.x)
//			{
//				m_bResize = false;
//				m_tInfo.fCX = m_vOriginSize.x;
//				m_tInfo.fCY = m_vOriginSize.y;
//			}
//
//			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
//		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_ImajinnSection_Selected::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_ImajinnSection_Selected::Render()
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

void CUI_ImajinnSection_Selected::Update_Position(CHARACTER_TYPE eType)
{
	if (eType == CHARACTER_TYPE::CHARACTER_END)
		return;

	switch (eType)
	{
	case CHARACTER_TYPE::SWORD_MAN:
		m_tInfo.fX = m_vPosition[0].x;
		m_tInfo.fY = m_vPosition[0].y;
		break;

	case CHARACTER_TYPE::ENGINEER:
		m_tInfo.fX = m_vPosition[1].x;
		m_tInfo.fY = m_vPosition[1].y;
		break;

	case CHARACTER_TYPE::DESTROYER:
		m_tInfo.fX = m_vPosition[2].x;
		m_tInfo.fY = m_vPosition[2].y;
		break;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

HRESULT CUI_ImajinnSection_Selected::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Selected"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ImajinnSection_Selected::Ready_State()
{
	CUI::UI_INFO UIDesc = {}; // Swordman Slot 정보
	UIDesc.fCX = 148.f * 0.48f;
	UIDesc.fCY = UIDesc.fCX;
	UIDesc.fX = g_iWinSizeX * 0.5f - UIDesc.fCX;
	UIDesc.fY = 835.f;

	m_vPosition[0] = _float2(UIDesc.fX, UIDesc.fY);
	m_vPosition[1] = _float2(UIDesc.fX + UIDesc.fCX, UIDesc.fY);
	m_vPosition[2] = _float2(UIDesc.fX + (UIDesc.fCX * 2.f), UIDesc.fY);

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_ImajinnSection_Selected::Bind_ShaderResources()
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

void CUI_ImajinnSection_Selected::Movement_BasedOnHiding(_float fTimeDelta)
{
	if (false == m_bHideFinish)
	{
		if (m_bHide) // 숨긴다
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
				m_bHideFinish = true;
			else
				m_tInfo.fX += (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG() * 1.2f);
		}
		else // 드러낸다
		{
			if (CUI_Manager::GetInstance()->Get_MovementComplete_SkillBG())
			{
				if (m_tInfo.fX <= m_vOriginPosition.x)
				{
					m_bHideFinish = true;
					m_tInfo.fX = m_vOriginPosition.x;
				}
				else
				{
					m_tInfo.fX -= fTimeDelta * m_fHideSpeed;
				}
			}
			else
			{
				if (m_tInfo.fX <= m_vOriginPosition.x)
				{
					m_bHideFinish = true;
					m_tInfo.fX = m_vOriginPosition.x;
				}
				else
				{
					m_tInfo.fX -= (CUI_Manager::GetInstance()->Get_DistanceofMovement_SkillBG() * 1.2f);
				}
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
	}
}

CUI_ImajinnSection_Selected* CUI_ImajinnSection_Selected::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ImajinnSection_Selected* pInstance = new CUI_ImajinnSection_Selected(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_ImajinnSection_Selected");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ImajinnSection_Selected::Clone(void* pArg)
{
	CUI_ImajinnSection_Selected* pInstance = new CUI_ImajinnSection_Selected(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_ImajinnSection_Selected");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ImajinnSection_Selected::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
