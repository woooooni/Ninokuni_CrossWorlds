#include "stdafx.h"
#include "UI_SkillSection_BtnRoll.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"

CUI_SkillSection_BtnRoll::CUI_SkillSection_BtnRoll(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_SkillSection_BtnRoll")
{
}

CUI_SkillSection_BtnRoll::CUI_SkillSection_BtnRoll(const CUI_SkillSection_BtnRoll& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_SkillSection_BtnRoll::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_SkillSection_BtnRoll::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	
	m_vOriginSize.x = m_tInfo.fCX;
	m_vOriginSize.y = m_tInfo.fCY;

	m_vMinSize.x = m_vOriginSize.x * 0.9f;
	m_vMinSize.y = m_vOriginSize.y * 0.9f;

	if (nullptr == m_pCharacter)
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return E_FAIL;

		m_pCharacter = pPlayer->Get_Character();
		if (nullptr == m_pCharacter)
			return E_FAIL;
	}

	return S_OK;
}

void CUI_SkillSection_BtnRoll::Tick(_float fTimeDelta)
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
		else
		{
			CStateMachine* pStateMachine = m_pCharacter->Get_Component<CStateMachine>(TEXT("Com_StateMachine"));

			if (CCharacter::STATE::BATTLE_DASH == pStateMachine->Get_CurrState())
				m_bResizeStart = true;
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_SkillSection_BtnRoll::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0.2f < m_fTimeAcc) // 누적한 시간이 기준치 이상이되면
		{
			m_fTimeAcc = 0.f; // 0.f로 초기화하고,
			m_bFinish = true; // Finish를 true로 변경해준다
		}

		if (m_bFinish)
		{
			m_tInfo.fCX = m_vOriginSize.x;
			m_tInfo.fCY = m_vOriginSize.y;

			m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

			m_bResizeStart = false;
			m_bFinish = false;
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_SkillSection_BtnRoll::Render()
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

void CUI_SkillSection_BtnRoll::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_SkillSection_BtnRoll::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		Key_Input(fTimeDelta);
	}
}

void CUI_SkillSection_BtnRoll::On_MouseExit(_float fTimeDelta)
{
}

HRESULT CUI_SkillSection_BtnRoll::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_Btn_Roll"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_SkillSection_BtnRoll::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_SkillSection_BtnRoll::Bind_ShaderResources()
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

void CUI_SkillSection_BtnRoll::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
		// 캐릭터가 Roll 상태가 아니라면
		// Roll State를 던진다.

		if (!m_bResizeStart)
			m_bResizeStart = true;
	}
}

CUI_SkillSection_BtnRoll* CUI_SkillSection_BtnRoll::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_SkillSection_BtnRoll* pInstance = new CUI_SkillSection_BtnRoll(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_SkillSection_BtnRoll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_SkillSection_BtnRoll::Clone(void* pArg)
{
	CUI_SkillSection_BtnRoll* pInstance = new CUI_SkillSection_BtnRoll(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_SkillSection_BtnRoll");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_SkillSection_BtnRoll::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
