#include "stdafx.h"
#include "UI_InGame_Setting_Slider.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "Camera_Manager.h"
#include "Camera_Group.h"

CUI_InGame_Setting_Slider::CUI_InGame_Setting_Slider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_SLIDERTYPE eType)
	: CUI(pDevice, pContext, L"UI_InGame_Setting_Slider")
	, m_eType(eType)
{
}

CUI_InGame_Setting_Slider::CUI_InGame_Setting_Slider(const CUI_InGame_Setting_Slider& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_InGame_Setting_Slider::Set_Active(_bool bActive)
{
	if (bActive)
	{
		if (m_fMaxX == 0 || m_fMinX == 0)
			return;

		m_fLength = fabs(m_fMaxX - m_fMinX);
		_float fValue = 0.f;
		_float fX = 0.f;
		_float fY = 0.f;

		CCamera_Follow* pFollowCamera =
			dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

		switch (m_eType)
		{
		case FIRST_SLIDER: // 쉐이킹
			fValue = pFollowCamera->Get_ShakeAmplitudeMag();
			m_tInfo.fX = m_fMinX + (m_fLength * fValue);
			m_iPercent = _int(fValue * 100.f);
			break;

		case SECOND_SLIDER: // 민감도
			fX = pFollowCamera->Get_MouseSensitivity().x;
			fY = pFollowCamera->Get_MouseSensitivity().y;

			m_tInfo.fX = m_fMinX + (m_fLength * fX); // 일단 X적용
			m_iPercent = _int(fX * 100.f);
			break;

		case THIRD_SLIDER: // 댐핑
			fValue = pFollowCamera->Get_DampingCoefficient();

			m_tInfo.fX = m_fMinX + (m_fLength * fValue);
			m_iPercent = _int(fValue * 100.f);
			break;
		}

		m_bDrag = false;
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	m_bActive = bActive;
}

void CUI_InGame_Setting_Slider::Set_DefaultSetting()
{
	if (m_eType == SLIDERTYPE_END)
		return;

	CCamera_Follow* pFollowCamera =
		dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

	pFollowCamera->Set_Defualt_Setting();
	_float fValue = 0.f;
	_float fX = 0.f;
	_float fY = 0.f;

	switch (m_eType)
	{
	case FIRST_SLIDER:
		fValue = pFollowCamera->Get_ShakeAmplitudeMag();
		m_tInfo.fX = m_fMinX + (m_fLength * fValue);
		m_iPercent = _int(fValue * 100.f);
		break;

	case SECOND_SLIDER: // 민감도
		fX = pFollowCamera->Get_MouseSensitivity().x;
		fY = pFollowCamera->Get_MouseSensitivity().y;

		m_tInfo.fX = m_fMinX + (m_fLength * fX); // 일단 X적용
		m_iPercent = _int(fX * 100.f);
		break;

	case THIRD_SLIDER: // 댐핑
		fValue = pFollowCamera->Get_DampingCoefficient();

		m_tInfo.fX = m_fMinX + (m_fLength * fValue);
		m_iPercent = _int(fValue * 100.f);
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
}

HRESULT CUI_InGame_Setting_Slider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slider::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Set_SliderRange();

	m_bActive = true;
	m_bUseMouse = true;
	
	return S_OK;
}

void CUI_InGame_Setting_Slider::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType >= UI_SETTING_SLIDERTYPE::SLIDERTYPE_END)
			return;

		// 타입별로 Slider BG의 총Width를 1로두고,
		// 슬라이더 ratio로 percent를 계산한다

		m_fLength = fabs(m_fMaxX - m_fMinX);

		_float fCurPosX = m_fLength - fabs(m_fMaxX - m_tInfo.fX); // 슬라이더 시작 기준 현재 내가 있는 상대적 위치
		m_iPercent = _int((fCurPosX / m_fLength) * 100.f);

		CCamera_Follow* pFollowCamera =
			dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));

		switch (m_eType)
		{
		case UI_SETTING_SLIDERTYPE::FIRST_SLIDER: // 쉐이킹
			pFollowCamera->Set_ShakeAmplitudeMag(fCurPosX / m_fLength);
			break;

		case UI_SETTING_SLIDERTYPE::SECOND_SLIDER: // 민감도
			pFollowCamera->Set_MouseSensitivity_X(fCurPosX / m_fLength);
			//pFollowCamera->Set_MouseSensitivity_Y(fCurPosX / m_fLength);
			break;

		case UI_SETTING_SLIDERTYPE::THIRD_SLIDER: // 댐핑
			pFollowCamera->Set_DampingCoefficient(fCurPosX / m_fLength);
			break;

		default:
			break;
		}

		if (true == m_bDrag)
		{
			CUI::UI_INFO UIDesc = {}; // 부모 정보

			UIDesc.fCX = 461.f;
			UIDesc.fCY = 27.f;
			UIDesc.fX = g_iWinSizeX - UIDesc.fCX * 0.5f - 15.f;
			UIDesc.fY = 120.f;

			POINT ptMouse = GI->GetMousePos();

			_float fPositionX = ptMouse.x - UIDesc.fX; // 부모기준으로 변환된 XPosition

			_float fMinPosX = UIDesc.fX + 60.f; // 부모를 고려하지 않은 최소 포지션
			_float fMaxPosX = UIDesc.fX + 200.f; // 부모를 고려하지 않은 최대 포지션

			m_tInfo.fX = fPositionX;

			if (ptMouse.x >= fMaxPosX)
				m_tInfo.fX = 200.f;
			if (ptMouse.x <= fMinPosX)
				m_tInfo.fX = 60.f;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_InGame_Setting_Slider::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0 > m_iPercent || 100 < m_iPercent)
			return;

		if (SLIDERTYPE_END == m_eType)
			return;

		if (true == m_bIsArrived)
		{
			CRenderer::TEXT_DESC PercentDesc = {};

			PercentDesc.strText = to_wstring(m_iPercent);
			PercentDesc.strFontTag = L"Default_Bold";
			PercentDesc.vScale = { 0.5f, 0.5f };

			_int iNumstr = PercentDesc.strText.size();
			_float fOffsetX = (3 - iNumstr) * 10.f;
			PercentDesc.vPosition = _float2(m_vTextPos.x + fOffsetX, m_vTextPos.y);

			PercentDesc.vColor = { 1.f, 1.f, 1.f, 1.f };

			m_pRendererCom->Add_Text(PercentDesc);
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_InGame_Setting_Slider::Render()
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

void CUI_InGame_Setting_Slider::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_InGame_Setting_Slider::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_AWAY(KEY::LBTN))
		{
			if (true == m_bDrag)
				m_bDrag = false;
		}

		__super::On_Mouse(fTimeDelta);
	}
}

void CUI_InGame_Setting_Slider::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

void CUI_InGame_Setting_Slider::On_MouseDragEnter(_float fTimeDelta)
{
}

void CUI_InGame_Setting_Slider::On_MouseDrag(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			if (false == m_bDrag)
			{
				if (0 != GI->Get_DIMMoveState(DIMM::DIMM_X))
					m_bDrag = true;
			}
		}

		if (KEY_AWAY(KEY::LBTN))
		{
			if (true == m_bDrag)
				m_bDrag = false;
		}
	}
}

void CUI_InGame_Setting_Slider::On_MouseDragExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		m_bDrag = false;
	}
}

HRESULT CUI_InGame_Setting_Slider::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Slider"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_InGame_Setting_Slider::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_InGame_Setting_Slider::Bind_ShaderResources()
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

void CUI_InGame_Setting_Slider::Set_SliderRange()
{
	// 슬라이더가 만들어질때 Range를 설정한다.

	// Setting창에서의 설정 -> 필요시 switch문 추가
	m_fMinX = m_tInfo.fX;
	m_fMaxX = m_tInfo.fX + 140.f;

	_float fOffset = 63.f;

	if (FIRST_SLIDER == m_eType)
		m_vTextPos = _float2(1310.f, 405.f);
	else if (SECOND_SLIDER == m_eType)
		m_vTextPos = _float2(1310.f, 405.f + fOffset);
	else
		m_vTextPos = _float2(1310.f, 405.f + (fOffset * 2.f + 2.f));
}

CUI_InGame_Setting_Slider* CUI_InGame_Setting_Slider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_SLIDERTYPE eType)
{
	CUI_InGame_Setting_Slider* pInstance = new CUI_InGame_Setting_Slider(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_InGame_Setting_Slider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_InGame_Setting_Slider::Clone(void* pArg)
{
	CUI_InGame_Setting_Slider* pInstance = new CUI_InGame_Setting_Slider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_InGame_Setting_Slider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_InGame_Setting_Slider::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
