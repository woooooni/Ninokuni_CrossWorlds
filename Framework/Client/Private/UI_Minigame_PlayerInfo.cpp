#include "stdafx.h"
#include "UI_Minigame_PlayerInfo.h"
#include "GameInstance.h"
#include "UI_Grandprix_PlaneHP.h"
#include "Riding_Manager.h"
#include "Vehicle_Flying.h"

#include "Game_Manager.h"
#include "Player.h"

CUI_Minigame_PlayerInfo::CUI_Minigame_PlayerInfo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_PlayerInfo")
{
}

CUI_Minigame_PlayerInfo::CUI_Minigame_PlayerInfo(const CUI_Minigame_PlayerInfo& rhs)
	: CUI(rhs)
{
}

void CUI_Minigame_PlayerInfo::Set_Owner(CVehicle_Flying* pOwner)
{
	m_pOwner = pOwner;

	if (nullptr != m_pHP)
		m_pHP->Set_Owner(pOwner);
}

HRESULT CUI_Minigame_PlayerInfo::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_PlayerInfo::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = false;

//	CUI::UI_INFO UIDesc = {};
//	UIDesc.fCX = 102.f;
//	UIDesc.fCY =16.f;
//	UIDesc.fX = m_tInfo.fX + 30.f;
//	UIDesc.fY = m_tInfo.fY + 19.f;
//	CGameObject* pBar = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBar"), _uint(LAYER_TYPE::LAYER_UI), &UIDesc);
//	if (nullptr == pBar)
//		return E_FAIL;
//	if (nullptr == dynamic_cast<CUI_Grandprix_PlaneHP*>(pBar))
//		return E_FAIL;
//	m_pHP = dynamic_cast<CUI_Grandprix_PlaneHP*>(pBar);

	Make_Child(41.5f, -10.f, 192.f, 15.f, TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBar"));

	if (nullptr != CRiding_Manager::GetInstance()->Get_Character_Biplane())
	{
		Set_Owner(CRiding_Manager::GetInstance()->Get_Character_Biplane());
	}

	if (nullptr != m_pOwner)
	{
		dynamic_cast<CUI_Grandprix_PlaneHP*>(Get_Child(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBar")))->Set_Owner(m_pOwner);
	}

	return S_OK;
}

void CUI_Minigame_PlayerInfo::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOwner)
		{
			dynamic_cast<CUI_Grandprix_PlaneHP*>(Get_Child(TEXT("Prototype_GameObject_UI_Minigame_Granprix_Plane_HPBar")))->Set_Owner(m_pOwner);
		}

//		if (nullptr != m_pHP)
//			m_pHP->Tick(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minigame_PlayerInfo::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
//		if (nullptr != m_pHP)
//			m_pHP->LateTick(fTimeDelta);

		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return;
		CCharacter* pCurCharacter = pPlayer->Get_Character();
		if (nullptr == pCurCharacter)
			return;
		CCharacter::CHARACTER_STAT StatDesc = pCurCharacter->Get_Stat();

		// 전투력 외곽선
		CRenderer::TEXT_DESC  TextDesc;
		TextDesc.strText = L"전투력"; // Temp
		TextDesc.strFontTag = L"Default_Medium";
		TextDesc.vPosition = _float2(m_vTextPosition.x - 1.f, m_vTextPosition.y);
		TextDesc.vColor = _float4(0.f, 0.f, 0.f, 1.f);
		TextDesc.vScale = { 0.38f, 0.38f };
		m_pRendererCom->Add_Text(TextDesc);
		TextDesc.vPosition = _float2(m_vTextPosition.x + 1.f, m_vTextPosition.y);
		m_pRendererCom->Add_Text(TextDesc);
		TextDesc.vPosition = _float2(m_vTextPosition.x, m_vTextPosition.y - 1.f);
		m_pRendererCom->Add_Text(TextDesc);
		TextDesc.vPosition = _float2(m_vTextPosition.x, m_vTextPosition.y + 1.f);
		m_pRendererCom->Add_Text(TextDesc);

		// 전투력
		TextDesc.vPosition = m_vTextPosition;
		TextDesc.vColor = { 1.f, 1.f, 1.f, 1.f };
		m_pRendererCom->Add_Text(TextDesc);

		// Todo : 전투력을 받아오게끔 구조 변경 필요함.
		// 전투력 숫자 외곽선
		CRenderer::TEXT_DESC Power;
		wstring strAttack = to_wstring(StatDesc.iAtt);
		_float iOffsetX = (strAttack.length() - 1) * 2.f;

		Power.strText = strAttack;
		Power.strFontTag = L"Default_Medium";
		Power.vPosition = _float2((m_vNumPosition.x + iOffsetX) - 2.f, m_vNumPosition.y);
		Power.vColor = _float4(0.f, 0.f, 0.f, 1.f);
		Power.vScale = { 0.38f, 0.38f };
		m_pRendererCom->Add_Text(Power);
		Power.vPosition = _float2((m_vNumPosition.x + iOffsetX) + 2.f, m_vNumPosition.y);
		m_pRendererCom->Add_Text(Power);
		Power.vPosition = _float2((m_vNumPosition.x + iOffsetX), m_vNumPosition.y - 2.f);
		m_pRendererCom->Add_Text(Power);
		Power.vPosition = _float2((m_vNumPosition.x + iOffsetX), m_vNumPosition.y + 2.f);
		m_pRendererCom->Add_Text(Power);

		//
		Power.vPosition = _float2((m_vNumPosition.x + iOffsetX), m_vNumPosition.y);
		Power.vColor = m_vPowerColor;
		m_pRendererCom->Add_Text(Power);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Minigame_PlayerInfo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

//	if (nullptr != m_pHP)
//		m_pHP->Render();

	__super::Render();

	return S_OK;
}

HRESULT CUI_Minigame_PlayerInfo::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_PlayerHP_Background"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_PlayerInfo::Ready_State()
{
	m_tInfo.fCX = 670.f * 0.55f;
	m_tInfo.fCY = 175.f * 0.52;
	m_tInfo.fX = m_tInfo.fCX * 0.5f;
	m_tInfo.fY = m_tInfo.fCY * 0.5f;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_PlayerInfo::Bind_ShaderResources()
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

CUI_Minigame_PlayerInfo* CUI_Minigame_PlayerInfo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_PlayerInfo* pInstance = new CUI_Minigame_PlayerInfo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_PlayerInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_PlayerInfo::Clone(void* pArg)
{
	CUI_Minigame_PlayerInfo* pInstance = new CUI_Minigame_PlayerInfo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_PlayerInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_PlayerInfo::Free()
{
	__super::Free();

	Safe_Release(m_pHP);
	Safe_Release(m_pTextureCom);
}
