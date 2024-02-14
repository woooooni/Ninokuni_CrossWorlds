#include "stdafx.h"
#include "UI_InGame_Setting_Slot.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Character.h"
#include "UI_InGame_Setting_RadioBtn.h"
#include "UI_InGame_Setting_Button.h"
#include "Camera_Manager.h"
#include "Camera_Group.h"
#include "UI_InGame_Setting_Slider.h"

CUI_InGame_Setting_Slot::CUI_InGame_Setting_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SETTING_SECTION eSection, UI_SLOT_ORDER eType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_Slot")
	, m_eType(eType)
	, m_eSectionType(eSection)
{
}

CUI_InGame_Setting_Slot::CUI_InGame_Setting_Slot(const CUI_InGame_Setting_Slot& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
	, m_eSectionType(rhs.m_eSectionType)
{
}

void CUI_InGame_Setting_Slot::Set_Active(_bool bActive)
{
	if (SLOTORDER_END == m_eType)
		return;

	if (bActive)
	{
		m_fTimeAcc = 0.f;
		m_bArrived = false;
		m_bClicked = false;
		m_tInfo.fX = m_vStartPosition.x;
		m_tInfo.fY = m_vStartPosition.y;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		if ((SETTING_GRAPHIC == m_eSectionType &&
			SLOT_THIRD == m_eType) ||
			(SETTING_CAMERA == m_eSectionType &&
				SLOT_THIRD == m_eType))
			Ready_RadioState();

		if ((SETTING_GRAPHIC == m_eSectionType && SLOT_SECOND == m_eType) ||
			(SETTING_CAMERA == m_eSectionType && SLOT_SECOND == m_eType))
			Ready_ButtonState();

//		if ((SETTING_AUDIO == m_eSectionType &&
//			SLOT_SECOND == m_eType || SLOT_THIRD == m_eType || SLOT_FOURTH == m_eType))
//			Ready_SliderState();
//
		Set_ChildActive(true);
	}
	else
	{
		if (m_bEvent)
			m_bEvent = false;

		if ((SETTING_CAMERA == m_eSectionType) &&
			(SLOT_FOURTH == m_eType || SLOT_FIFTH == m_eType || SLOT_SIXTH == m_eType))
		{
			for (auto& iter : m_pChild)
			{
				if (nullptr != dynamic_cast<CUI_InGame_Setting_Slider*>(iter))
					dynamic_cast<CUI_InGame_Setting_Slider*>(iter)->Set_IsArrived(false);
			}
		}

		if ((SETTING_AUDIO == m_eSectionType) &&
			(SLOT_SECOND == m_eType || SLOT_THIRD == m_eType || SLOT_FOURTH == m_eType))
		{
			for (auto& iter : m_pChild)
			{
				if (nullptr != dynamic_cast<CUI_InGame_Setting_Slider*>(iter))
					dynamic_cast<CUI_InGame_Setting_Slider*>(iter)->Set_IsArrived(false);
			}
		}

		Set_ChildActive(false);
	}

	m_bActive = bActive;
}

HRESULT CUI_InGame_Setting_Slot::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;
	
	Ready_Buttons();
	Ready_RadioGroup();

	Ready_ButtonState();

	Ready_Slider();
	Ready_Icons();

	Ready_SliderState();

	m_bActive = false;

	switch (m_eType)
	{
	case SLOT_FIRST:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 100.f, m_vArrivedPosition.y);
		break;

	case SLOT_SECOND:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 150.f, m_vArrivedPosition.y);
		break;

	case SLOT_THIRD:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 200.f, m_vArrivedPosition.y);
		break;

	case SLOT_FOURTH:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 250.f, m_vArrivedPosition.y);
		break;

	case SLOT_FIFTH:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 300.f, m_vArrivedPosition.y);
		break;

	case SLOT_SIXTH:
		m_vArrivedPosition = _float2(m_tInfo.fX, m_tInfo.fY);
		m_vStartPosition = _float2(m_vArrivedPosition.x + 350.f, m_vArrivedPosition.y);
		break;
	}

	m_fSpeed = 500.f;
	m_fLimit = m_tInfo.fX + (m_tInfo.fCX * 0.5f);

	m_tInfo.fX = m_vStartPosition.x;
	m_tInfo.fY = m_vStartPosition.y;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

void CUI_InGame_Setting_Slot::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (SLOTORDER_END == m_eType)
			return;

		if (!m_bArrived)
		{
			m_tInfo.fX -= fTimeDelta * m_fSpeed;

			if (m_tInfo.fX < m_vArrivedPosition.x)
			{
				m_bArrived = true;
				Set_ChildActive(true);
				m_tInfo.fX = m_vArrivedPosition.x;

				if ((SETTING_CAMERA == m_eSectionType) &&
					(SLOT_FOURTH == m_eType || SLOT_FIFTH == m_eType || SLOT_SIXTH == m_eType))
				{
					for (auto& iter : m_pChild)
					{
						if (nullptr != dynamic_cast<CUI_InGame_Setting_Slider*>(iter))
							dynamic_cast<CUI_InGame_Setting_Slider*>(iter)->Set_IsArrived(true);
					}
				}

				if ((SETTING_AUDIO == m_eSectionType) &&
					(SLOT_SECOND == m_eType || SLOT_THIRD == m_eType || SLOT_FOURTH == m_eType))
				{
					for (auto& iter : m_pChild)
					{
						if (nullptr != dynamic_cast<CUI_InGame_Setting_Slider*>(iter))
							dynamic_cast<CUI_InGame_Setting_Slider*>(iter)->Set_IsArrived(true);
					}
				}
			}
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_Slot::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (SLOTORDER_END == m_eType)
			return;

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_InGame_Setting_Slot::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(9);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

void CUI_InGame_Setting_Slot::Update_ButtonState(_uint iGroupType)
{
	CUI* pCustomBtn = nullptr;
	CUI* pDefaultBtn = nullptr;

	CUI_InGame_Setting_Button* pCustom = nullptr;
	CUI_InGame_Setting_Button* pDefault = nullptr;

	CUI* pBackBtn = nullptr;
	CUI* pShoulderBtn = nullptr;

	CUI_InGame_Setting_Button* pBack = nullptr;
	CUI_InGame_Setting_Button* pShoulder = nullptr;

	CCamera_Follow* pFollowCamera =
		dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr == pFollowCamera)
		return;

	switch (iGroupType)
	{
	case CUI_InGame_Setting_Button::UI_SETTINGBUTTON::SETBUTTON_GRAPHIC_CUSTOM:
		pCustomBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Custom"));
		if (nullptr == pCustomBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pCustomBtn))
			return;
		pDefaultBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Default"));
		if (nullptr == pDefaultBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pDefaultBtn))
			return;

		pCustom = dynamic_cast<CUI_InGame_Setting_Button*>(pCustomBtn);
		pDefault = dynamic_cast<CUI_InGame_Setting_Button*>(pDefaultBtn);

		pCustom->Set_Click(true);
		pDefault->Set_Click(false);

		// 버튼을 누를 수 있게 한다.
		CUI_Manager::GetInstance()->Set_AbleToControlRadio(false);
		break;

	case CUI_InGame_Setting_Button::UI_SETTINGBUTTON::SETBUTTON_GRAPHIC_DEFAULT:
		pCustomBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Custom"));
		if (nullptr == pCustomBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pCustomBtn))
			return;
		pDefaultBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Default"));
		if (nullptr == pDefaultBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pDefaultBtn))
			return;

		pCustom = dynamic_cast<CUI_InGame_Setting_Button*>(pCustomBtn);
		pDefault = dynamic_cast<CUI_InGame_Setting_Button*>(pDefaultBtn);

		pCustom->Set_Click(false);
		pDefault->Set_Click(true);

		// 버튼을 누르지 못하게 통제한다.
		CUI_Manager::GetInstance()->Set_AbleToControlRadio(true);
		// 기본세팅으로 만든다.
		CUI_Manager::GetInstance()->Set_DefaultGraphicSetting();
		break;

	case CUI_InGame_Setting_Button::UI_SETTINGBUTTON::SETBUTTON_CAMERA_SHOULDER:
		pShoulderBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_ShoulderView"));
		if (nullptr == pShoulderBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pShoulderBtn))
			return;
		pBackBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_BackView"));
		if (nullptr == pBackBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pBackBtn))
			return;

		pShoulder = dynamic_cast<CUI_InGame_Setting_Button*>(pShoulderBtn);
		pBack = dynamic_cast<CUI_InGame_Setting_Button*>(pBackBtn);

		pShoulder->Set_Click(true);
		pBack->Set_Click(false);

		if (CAMERA_VIEW_TYPE::SHOLDER != pFollowCamera->Get_ViewType())
			pFollowCamera->Set_ViewType(CAMERA_VIEW_TYPE::SHOLDER);
		break;

	case CUI_InGame_Setting_Button::UI_SETTINGBUTTON::SETBUTTON_CAMERA_BACK:
		pShoulderBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_ShoulderView"));
		if (nullptr == pShoulderBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pShoulderBtn))
			return;
		pBackBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_BackView"));
		if (nullptr == pBackBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pBackBtn))
			return;

		pShoulder = dynamic_cast<CUI_InGame_Setting_Button*>(pShoulderBtn);
		pBack = dynamic_cast<CUI_InGame_Setting_Button*>(pBackBtn);

		pShoulder->Set_Click(false);
		pBack->Set_Click(true);

		if (CAMERA_VIEW_TYPE::BACK != pFollowCamera->Get_ViewType())
			pFollowCamera->Set_ViewType(CAMERA_VIEW_TYPE::BACK);
		break;

	default:
		break;
	}
}

void CUI_InGame_Setting_Slot::Update_GraphicRadioBtnState(_uint iGroupType, _uint iBtnType)
{
	/*
	enum UI_RADIOGROUP
	{ GRAPHIC_NATURAL, GRAPHIC_SHADOW, GRAPHIC_OUTLINE, GRAPHIC_BLOOM, GRAPHIC_BLUR, GRAPHIC_SSAO, GRAPHIC_PBR, RADIOGROUP_END };
	enum UI_RADIOTYPE{ RADIO_ONBTN, RADIO_OFFBTN, RADIO_END };
	*/
	if (m_bControl == false)
	{
		CUI* pOnBtn = nullptr;
		CUI* pOffBtn = nullptr;

		CUI_InGame_Setting_RadioBtn* pOn = nullptr;
		CUI_InGame_Setting_RadioBtn* pOff = nullptr;

		switch (iGroupType)
		{
		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_NATURAL:
			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Natural"));
			if (nullptr == pOnBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
				return;
			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Natural"));
			if (nullptr == pOffBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
				return;

			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
			{
				pOn->Set_Click(true);
				pOff->Set_Click(false);

				if (false == m_pRendererCom->Get_NatrualDraw())
					m_pRendererCom->Set_NaturalDraw(true);

			}
			else
			{
				pOn->Set_Click(false);
				pOff->Set_Click(true);

				if (true == m_pRendererCom->Get_NatrualDraw())
					m_pRendererCom->Set_NaturalDraw(false);
			}
			break;

		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_SHADOW:
			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Shadow"));
			if (nullptr == pOnBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
				return;
			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Shadow"));
			if (nullptr == pOffBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
				return;

			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
			{
				pOn->Set_Click(true);
				pOff->Set_Click(false);

				if (false == m_pRendererCom->Get_ShadowDraw())
					m_pRendererCom->Set_ShadowDraw(true);
			}
			else
			{
				pOn->Set_Click(false);
				pOff->Set_Click(true);

				if (true == m_pRendererCom->Get_ShadowDraw())
					m_pRendererCom->Set_ShadowDraw(false);
			}
			break;

		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_OUTLINE:
			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Outline"));
			if (nullptr == pOnBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
				return;
			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Outline"));
			if (nullptr == pOffBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
				return;

			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
			{
				pOn->Set_Click(true);
				pOff->Set_Click(false);

				if (false == m_pRendererCom->Get_OutlineDraw())
					m_pRendererCom->Set_OutlineDraw(true);
			}
			else
			{
				pOn->Set_Click(false);
				pOff->Set_Click(true);

				if (true == m_pRendererCom->Get_OutlineDraw())
					m_pRendererCom->Set_OutlineDraw(false);
			}
			break;

		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_BLOOM:
			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Bloom"));
			if (nullptr == pOnBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
				return;
			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Bloom"));
			if (nullptr == pOffBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
				return;

			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
			{
				pOn->Set_Click(true);
				pOff->Set_Click(false);

				if (false == m_pRendererCom->Get_BloomDraw())
					m_pRendererCom->Set_BloomDraw(true);
			}
			else
			{
				pOn->Set_Click(false);
				pOff->Set_Click(true);

				if (true == m_pRendererCom->Get_BloomDraw())
					m_pRendererCom->Set_BloomDraw(false);
			}
			break;

		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_BLUR:
			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Blur"));
			if (nullptr == pOnBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
				return;
			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Blur"));
			if (nullptr == pOffBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
				return;

			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
			{
				pOn->Set_Click(true);
				pOff->Set_Click(false);

				if (false == m_pRendererCom->Get_BlurDraw())
					m_pRendererCom->Set_BlurDraw(true);
			}
			else
			{
				pOn->Set_Click(false);
				pOff->Set_Click(true);

				if (true == m_pRendererCom->Get_BlurDraw())
					m_pRendererCom->Set_BlurDraw(false);
			}
			break;

		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_SSAO:
			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_SSAO"));
			if (nullptr == pOnBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
				return;
			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_SSAO"));
			if (nullptr == pOffBtn)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
				return;

			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
			{
				pOn->Set_Click(true);
				pOff->Set_Click(false);

				if (false == m_pRendererCom->Get_SsaoDraw())
					m_pRendererCom->Set_SsaoDraw(true);
			}
			else
			{
				pOn->Set_Click(false);
				pOff->Set_Click(true);

				if (true == m_pRendererCom->Get_SsaoDraw())
					m_pRendererCom->Set_SsaoDraw(false);
			}
			break;

		case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::GRAPHIC_PBR:
//			pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_PBR"));
//			if (nullptr == pOnBtn)
//				return;
//			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
//				return;
//			pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_PBR"));
//			if (nullptr == pOffBtn)
//				return;
//			if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
//				return;
//
//			pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
//			pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);
//
//			if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
//			{
//				pOn->Set_Click(true);
//				pOff->Set_Click(false);
//
//				if (false == m_pRendererCom->Get_PbrDraw())
//					m_pRendererCom->Set_PbrDraw(true);
//			}
//			else
//			{
//				pOn->Set_Click(false);
//				pOff->Set_Click(true);
//
//				if (true == m_pRendererCom->Get_PbrDraw())
//					m_pRendererCom->Set_PbrDraw(false);
//			}
			break;
		}
	}
}

void CUI_InGame_Setting_Slot::Update_CameraRadioBtnState(_uint iGroupType, _uint iBtnType)
{
	CUI* pOnBtn = nullptr;
	CUI* pOffBtn = nullptr;

	CUI_InGame_Setting_RadioBtn* pOn = nullptr;
	CUI_InGame_Setting_RadioBtn* pOff = nullptr;

	CCamera_Follow* pFollowCamera =
		dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr == pFollowCamera)
		return;

	switch (iGroupType)
	{
	case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::CAMERA_AXISX:
		pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Camera_AxisX"));
		if (nullptr == pOnBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
			return;
		pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Camera_AxisX"));
		if (nullptr == pOffBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
			return;

		pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
		pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

		if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
		{
			pOn->Set_Click(true);
			pOff->Set_Click(false);

			pFollowCamera->Set_MouseInputInvert_X(false);
		}
		else
		{
			pOn->Set_Click(false);
			pOff->Set_Click(true);

			pFollowCamera->Set_MouseInputInvert_X(true);
		}
		break;

	case CUI_InGame_Setting_RadioBtn::UI_RADIOGROUP::CAMERA_AXISY:
		pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Camera_AxisY"));
		if (nullptr == pOnBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
			return;
		pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Camera_AxisY"));
		if (nullptr == pOffBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
			return;

		pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
		pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

		if (CUI_InGame_Setting_RadioBtn::UI_RADIOTYPE::RADIO_ONBTN == iBtnType) // 0일때
		{
			pOn->Set_Click(true);
			pOff->Set_Click(false);

			pFollowCamera->Set_MouseInputInvert_Y(false);
		}
		else
		{
			pOn->Set_Click(false);
			pOff->Set_Click(true);

			pFollowCamera->Set_MouseInputInvert_Y(true);
		}
		break;
	}
}

HRESULT CUI_InGame_Setting_Slot::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Graphic_Slots"),
		TEXT("Com_Texture1"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Camera_Slots"),
		TEXT("Com_Texture2"), (CComponent**)&m_pTextureCamera)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Sound_Slots"),
		TEXT("Com_Texture3"), (CComponent**)&m_pTextureSound)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slot::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (!m_bArrived)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_LimitX", &m_fLimit, sizeof(_float))))
			return E_FAIL;
	}

	switch(m_eSectionType)
	{
	case UI_SETTING_SECTION::SETTING_GRAPHIC:
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
		break;

	case UI_SETTING_SECTION::SETTING_CAMERA:
		if (FAILED(m_pTextureCamera->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
		break;

	case UI_SETTING_SECTION::SETTING_AUDIO:
		if (FAILED(m_pTextureSound->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
			return E_FAIL;
		break;
	}

	return S_OK;
}

void CUI_InGame_Setting_Slot::Ready_Buttons()
{
	if (m_eSectionType == SETTINGSECTION_END)
		return;

	if (m_eType == SLOTORDER_END)
		return;

	_float2 vSize = _float2(124.f, 40.f);

	switch (m_eSectionType)
	{
	case UI_SETTING_SECTION::SETTING_GRAPHIC:
		if (m_eType != SLOT_SECOND)
			return;

		if (FAILED(Make_Child(0.f, 0.f, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Custom"))))
			return;
		if (FAILED(Make_Child(150.f, 0.f, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Default"))))
			return;
		break;

	case UI_SETTING_SECTION::SETTING_CAMERA:

		if (m_eType != SLOT_SECOND)
			return;

		if (FAILED(Make_Child(0.f, 0.f, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_ShoulderView"))))
			return;
		if (FAILED(Make_Child(150.f, 0.f, vSize.x, vSize.y, TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_BackView"))))
			return;
		break;

	case UI_SETTING_SECTION::SETTING_AUDIO:
		break;
	}
}

void CUI_InGame_Setting_Slot::Ready_RadioGroup()
{
	if (m_eSectionType == SETTINGSECTION_END)
		return;

	if (m_eType == SLOTORDER_END)
		return;

	_float fSize = 64.f * 0.5f;

	switch (m_eSectionType)
	{
	case UI_SETTING_SECTION::SETTING_GRAPHIC:
		if (m_eType != SLOT_THIRD)
			return;

		if (FAILED(Make_Child(-55.f, -105.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Natural"))))
			return;
		if (FAILED(Make_Child(95.f, -105.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Natural"))))
			return;

		if (FAILED(Make_Child(-55.f, -60.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Shadow"))))
			return;
		if (FAILED(Make_Child(95.f, -60.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Shadow"))))
			return;

		if (FAILED(Make_Child(-55.f, -15.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Outline"))))
			return;
		if (FAILED(Make_Child(95.f, -15.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Outline"))))
			return;
		
		if (FAILED(Make_Child(-55.f, 30.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Bloom"))))
			return;
		if (FAILED(Make_Child(95.f, 30.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Bloom"))))
			return;

		if (FAILED(Make_Child(-55.f, 72.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Blur"))))
			return;
		if (FAILED(Make_Child(95.f, 72.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Blur"))))
			return;

		if (FAILED(Make_Child(-55.f, 114.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_SSAO"))))
			return;
		if (FAILED(Make_Child(95.f, 114.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_SSAO"))))
			return;

//		if (FAILED(Make_Child(-55.f, 156.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_PBR"))))
//			return;
//		if (FAILED(Make_Child(95.f, 156.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_PBR"))))
//			return;
		break;

	case UI_SETTING_SECTION::SETTING_CAMERA:
		if (m_eType != SLOT_THIRD)
			return;

		if (FAILED(Make_Child(-55.f, 5.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Camera_AxisX"))))
			return;
		if (FAILED(Make_Child(95.f, 5.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Camera_AxisX"))))
			return;

		if (FAILED(Make_Child(-55.f, 50.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Camera_AxisY"))))
			return;
		if (FAILED(Make_Child(95.f, 50.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Camera_AxisY"))))
			return;
		break;

	case UI_SETTING_SECTION::SETTING_AUDIO:
		break;
	}
}

void CUI_InGame_Setting_Slot::Ready_RadioState()
{
	CUI* pOnBtn = nullptr;
	CUI* pOffBtn = nullptr;

	CUI_InGame_Setting_RadioBtn* pOn = nullptr;
	CUI_InGame_Setting_RadioBtn* pOff = nullptr;

	CCamera_Follow* pFollowCamera =
		dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
	if (nullptr == pFollowCamera)
		return;

	if (true == CUI_Manager::GetInstance()->Is_SettingFirst())
	{
		if (SETTING_GRAPHIC == m_eSectionType)
		{
			if (true == m_pRendererCom->Get_NatrualDraw())
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Natural"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Natural"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(true);
				pOff->Set_Click(false);
			}
			else
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Natural"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Natural"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(false);
				pOff->Set_Click(true);
			}

			if (true == m_pRendererCom->Get_ShadowDraw())
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Shadow"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Shadow"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(true);
				pOff->Set_Click(false);
			}
			else
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Shadow"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Shadow"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(false);
				pOff->Set_Click(true);
			}

			if (true == m_pRendererCom->Get_OutlineDraw())
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Outline"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Outline"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(true);
				pOff->Set_Click(false);
			}
			else
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Outline"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Outline"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(false);
				pOff->Set_Click(true);
			}

			if (true == m_pRendererCom->Get_BloomDraw())
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Bloom"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Bloom"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(true);
				pOff->Set_Click(false);
			}
			else
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Bloom"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Bloom"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(false);
				pOff->Set_Click(true);
			}

			if (true == m_pRendererCom->Get_BlurDraw())
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Blur"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Blur"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(true);
				pOff->Set_Click(false);
			}
			else
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Blur"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Blur"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(false);
				pOff->Set_Click(true);
			}

			if (true == m_pRendererCom->Get_SsaoDraw())
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_SSAO"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_SSAO"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(true);
				pOff->Set_Click(false);
			}
			else
			{
				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_SSAO"));
				if (nullptr == pOnBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
					return;
				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_SSAO"));
				if (nullptr == pOffBtn)
					return;
				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
					return;

				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

				pOn->Set_Click(false);
				pOff->Set_Click(true);
			}

//			if (true == m_pRendererCom->Get_PbrDraw())
//			{
//				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_PBR"));
//				if (nullptr == pOnBtn)
//					return;
//				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
//					return;
//				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_PBR"));
//				if (nullptr == pOffBtn)
//					return;
//				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
//					return;
//
//				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
//				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);
//
//				pOn->Set_Click(true);
//				pOff->Set_Click(false);
//			}
//			else
//			{
//				pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_PBR"));
//				if (nullptr == pOnBtn)
//					return;
//				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
//					return;
//				pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_PBR"));
//				if (nullptr == pOffBtn)
//					return;
//				if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
//					return;
//
//				pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
//				pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);
//
//				pOn->Set_Click(false);
//				pOff->Set_Click(true);
//			}
		}
	}

	if (SETTING_CAMERA == m_eSectionType)
	{
		_int iX = pFollowCamera->Get_MouseInputInvert().x;
		_int iY = pFollowCamera->Get_MouseInputInvert().y;

		pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Camera_AxisX"));
		if (nullptr == pOnBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
			return;
		pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Camera_AxisX"));
		if (nullptr == pOffBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
			return;

		pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
		pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

		if (-1 == iX)
		{
			pOn->Set_Click(true);
			pOff->Set_Click(false);
		}
		else
		{
			pOn->Set_Click(false);
			pOff->Set_Click(true);
		}

		pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Camera_AxisY"));
		if (nullptr == pOnBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
			return;
		pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Camera_AxisY"));
		if (nullptr == pOffBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
			return;

		pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
		pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

		if (-1 == iY)
		{
			pOn->Set_Click(true);
			pOff->Set_Click(false);
		}
		else
		{
			pOn->Set_Click(false);
			pOff->Set_Click(true);
		}
	}
}

void CUI_InGame_Setting_Slot::Ready_ButtonState()
{
	if (SETTING_CAMERA == m_eSectionType)
	{
		CCamera_Follow* pFollowCamera =
			dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

		CAMERA_VIEW_TYPE eView = pFollowCamera->Get_ViewType();

		CUI* pShoulderBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_ShoulderView"));
		if (nullptr == pShoulderBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pShoulderBtn))
			return;
		CUI* pBackBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Camera_BackView"));
		if (nullptr == pBackBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pBackBtn))
			return;

		CUI_InGame_Setting_Button* pShoulder = dynamic_cast<CUI_InGame_Setting_Button*>(pShoulderBtn);
		CUI_InGame_Setting_Button* pBack = dynamic_cast<CUI_InGame_Setting_Button*>(pBackBtn);

		if (CAMERA_VIEW_TYPE::BACK == eView)
		{
			pShoulder->Set_Click(false);
			pBack->Set_Click(true);
		}
		else
		{
			pShoulder->Set_Click(true);
			pBack->Set_Click(false);
		}
	}

	if (m_eSectionType == SETTING_GRAPHIC)
	{
		CUI* pCustomBtn = nullptr;
		CUI* pDefaultBtn = nullptr;

		CUI_InGame_Setting_Button* pCustom = nullptr;
		CUI_InGame_Setting_Button* pDefault = nullptr;

		pCustomBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Custom"));
		if (nullptr == pCustomBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pCustomBtn))
			return;
		pDefaultBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Button_Graphic_Default"));
		if (nullptr == pDefaultBtn)
			return;
		if (nullptr == dynamic_cast<CUI_InGame_Setting_Button*>(pDefaultBtn))
			return;

		pCustom = dynamic_cast<CUI_InGame_Setting_Button*>(pCustomBtn);
		pDefault = dynamic_cast<CUI_InGame_Setting_Button*>(pDefaultBtn);

		if (true == CUI_Manager::GetInstance()->Is_SettingFirst())
		{
			// 첫 세팅
			CUI_Manager::GetInstance()->Set_GraphicSettingState(false);

			pCustom->Set_Click(false);
			pDefault->Set_Click(true);

			CUI_Manager::GetInstance()->Set_DefaultGraphicSetting();
		}
	}
}

void CUI_InGame_Setting_Slot::Ready_SliderState()
{
	if (SETTING_CAMERA == m_eSectionType)
	{
		if (SLOT_FOURTH > m_eType || SLOTORDER_END == m_eType)
			return;

		CCamera_Follow* pFollowCamera =
			dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
		if (nullptr == pFollowCamera)
			return;

		CUI* pFirstSlider = nullptr;
		CUI* pSecondSlider = nullptr;
		CUI* pThirdSlider = nullptr;

		CUI_InGame_Setting_Slider* pFirst = nullptr;
		CUI_InGame_Setting_Slider* pSecond = nullptr;
		CUI_InGame_Setting_Slider* pThird = nullptr;

		switch (m_eType)
		{
		case SLOT_FOURTH:
			pFirstSlider = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_First"));
			if (nullptr == pFirstSlider)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_Slider*>(pFirstSlider))
				return;

			pFirst = dynamic_cast<CUI_InGame_Setting_Slider*>(pFirstSlider); // 흔들림
			pFirst->Set_CameraDefaultSetting();
			break;

		case SLOT_FIFTH:
			pSecondSlider = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Second"));
			if (nullptr == pSecondSlider)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_Slider*>(pSecondSlider))
				return;

			pSecond = dynamic_cast<CUI_InGame_Setting_Slider*>(pSecondSlider); // 마우스 민감도
			pSecond->Set_CameraDefaultSetting();
			break;

		case SLOT_SIXTH:
			pThirdSlider = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Third"));
			if (nullptr == pThirdSlider)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_Slider*>(pThirdSlider))
				return;

			pThird = dynamic_cast<CUI_InGame_Setting_Slider*>(pThirdSlider); // 댐핑 민감도
			pThird->Set_CameraDefaultSetting();
			break;
		}
	}
	if (SETTING_AUDIO == m_eSectionType)
	{
		CUI* pFirstSlider = nullptr;
		CUI* pSecondSlider = nullptr;
		CUI* pThirdSlider = nullptr;

		CUI_InGame_Setting_Slider* pFirst = nullptr;
		CUI_InGame_Setting_Slider* pSecond = nullptr;
		CUI_InGame_Setting_Slider* pThird = nullptr;

		switch (m_eType)
		{
		case SLOT_SECOND:
			pFirstSlider = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Audio_First"));
			if (nullptr == pFirstSlider)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_Slider*>(pFirstSlider))
				return;

			pFirst = dynamic_cast<CUI_InGame_Setting_Slider*>(pFirstSlider); // 전체 볼륨
			pFirst->Set_AudioDefaultSetting();
			break;

		case SLOT_THIRD:
			pSecondSlider = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Audio_Second"));
			if (nullptr == pSecondSlider)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_Slider*>(pSecondSlider))
				return;

			pSecond = dynamic_cast<CUI_InGame_Setting_Slider*>(pSecondSlider); // 배경음
			pSecond->Set_AudioDefaultSetting();
			break;

		case SLOT_FOURTH:
			pThirdSlider = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Audio_Third"));
			if (nullptr == pThirdSlider)
				return;
			if (nullptr == dynamic_cast<CUI_InGame_Setting_Slider*>(pThirdSlider))
				return;

			pThird = dynamic_cast<CUI_InGame_Setting_Slider*>(pThirdSlider); // 효과음
			pThird->Set_AudioDefaultSetting();
			break;
		}
	}

}

void CUI_InGame_Setting_Slot::Ready_Slider()
{
	_float fWidth = 17.f; // 12.f

	if (SETTING_CAMERA == m_eSectionType)
	{
		if (SLOT_FOURTH > m_eType || SLOTORDER_END == m_eType)
			return;

		switch (m_eType)
		{
		case SLOT_FOURTH:
			if (FAILED(Make_Child(60.f, 0.f, fWidth, 23.f, TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_First"))))
				return;
			break;

		case SLOT_FIFTH:
			if (FAILED(Make_Child(60.f, 0.f, fWidth, 23.f, TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Second"))))
				return;
			break;

		case SLOT_SIXTH: // 최대 200
			if (FAILED(Make_Child(60.f, 0.f, fWidth, 23.f, TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Third"))))
				return;
			break;
		}
	}
	else if (SETTING_AUDIO == m_eSectionType)
	{
		if (!(SLOT_FIRST < m_eType && SLOT_FIFTH > m_eType))
			return;

		switch (m_eType)
		{
		case SLOT_SECOND:
			if (FAILED(Make_Child(60.f, 2.f, fWidth, 23.f, TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Audio_First"))))
				return;
			break;

		case SLOT_THIRD:
			if (FAILED(Make_Child(60.f, 3.f, fWidth, 23.f, TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Audio_Second"))))
				return;
			break;

		case SLOT_FOURTH: // 최대 200
			if (FAILED(Make_Child(60.f, 3.f, fWidth, 23.f, TEXT("Prototype_GameObject_UI_Ingame_Setting_Slider_Audio_Third"))))
				return;
			break;
		}
	}
}

void CUI_InGame_Setting_Slot::Ready_Icons()
{
	if (SETTING_CAMERA != m_eSectionType)
		return;

	if (SLOT_FIRST != m_eType)
		return;

	_float fSize = 64.f * 0.65f;

	if (FAILED(Make_Child(210.f, 5.f, fSize, fSize, TEXT("Prototype_GameObject_UI_Ingame_Setting_Etc_Icon_Camera_DefaultSetting"))))
		return;
}

void CUI_InGame_Setting_Slot::Set_DefaultGraphicSetting()
{
	CUI* pOnBtn = nullptr;
	CUI* pOffBtn = nullptr;

	CUI_InGame_Setting_RadioBtn* pOn = nullptr;
	CUI_InGame_Setting_RadioBtn* pOff = nullptr;

	// Natural
	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Natural"));
	if (nullptr == pOnBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
		return;
	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Natural"));
	if (nullptr == pOffBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
		return;
	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

	pOn->Set_Click(true);
	pOff->Set_Click(false);
	m_pRendererCom->Set_NaturalDraw(true);


	// Shadow
	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Shadow"));
	if (nullptr == pOnBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
		return;
	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Shadow"));
	if (nullptr == pOffBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
		return;
	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

	pOn->Set_Click(true);
	pOff->Set_Click(false);
	m_pRendererCom->Set_ShadowDraw(true);


	// Outline
	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Outline"));
	if (nullptr == pOnBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
		return;
	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Outline"));
	if (nullptr == pOffBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
		return;
	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

	pOn->Set_Click(true);
	pOff->Set_Click(false);
	m_pRendererCom->Set_OutlineDraw(true);


	// Bloom
	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Bloom"));
	if (nullptr == pOnBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
		return;
	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Bloom"));
	if (nullptr == pOffBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
		return;
	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

	pOn->Set_Click(true);
	pOff->Set_Click(false);
	m_pRendererCom->Set_BloomDraw(true);


	// Blur
	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_Blur"));
	if (nullptr == pOnBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
		return;
	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_Blur"));
	if (nullptr == pOffBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
		return;

	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

	pOn->Set_Click(true);
	pOff->Set_Click(false);
	m_pRendererCom->Set_BlurDraw(true);


	// SSAO
	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_SSAO"));
	if (nullptr == pOnBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
		return;
	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_SSAO"));
	if (nullptr == pOffBtn)
		return;
	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
		return;
	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);

	pOn->Set_Click(true);
	pOff->Set_Click(false);
	m_pRendererCom->Set_SsaoDraw(true);


	// PBR
//	pOnBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OnButton_Graphic_PBR"));
//	if (nullptr == pOnBtn)
//		return;
//	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn))
//		return;
//	pOffBtn = Get_Child(TEXT("Prototype_GameObject_UI_Ingame_Setting_Radio_OffButton_Graphic_PBR"));
//	if (nullptr == pOffBtn)
//		return;
//	if (nullptr == dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn))
//		return;
//	pOn = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOnBtn);
//	pOff = dynamic_cast<CUI_InGame_Setting_RadioBtn*>(pOffBtn);
//
//	pOn->Set_Click(true);
//	pOff->Set_Click(false);
//	m_pRendererCom->Set_PbrDraw(true);

	//g_bControl = false;
}

void CUI_InGame_Setting_Slot::Set_DefaultCameraSetting()
{
	if (SETTING_CAMERA != m_eSectionType)
		return;

	Ready_RadioState();
	Ready_ButtonState();
	Ready_SliderState();
}

void CUI_InGame_Setting_Slot::Set_DefaultSoundSetting()
{
	if (SETTING_AUDIO != m_eSectionType)
		return;

	Ready_SliderState();
}


CUI_InGame_Setting_Slot* CUI_InGame_Setting_Slot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UI_SETTING_SECTION eSection, UI_SLOT_ORDER eType)
{
	CUI_InGame_Setting_Slot* pInstance = new CUI_InGame_Setting_Slot(pDevice, pContext, eSection, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_Slot::Clone(void* pArg)
{
	CUI_InGame_Setting_Slot* pInstance = new CUI_InGame_Setting_Slot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_Slot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_Slot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureSound);
	Safe_Release(m_pTextureCamera);
	Safe_Release(m_pTextureCom);
}
