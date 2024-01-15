#include "stdafx.h"
#include "UI_Setting_Slider.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Setting_Slider::CUI_Setting_Slider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_SLIDERTYPE eType)
	: CUI(pDevice, pContext, L"UI_Setting_Slider")
	, m_eType(eType)
{
}

CUI_Setting_Slider::CUI_Setting_Slider(const CUI_Setting_Slider& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Setting_Slider::Set_Active(_bool bActive)
{
	if (bActive)
	{
		if (m_fMaxX == 0 || m_fMinX == 0)
			return;

		m_fLength = fabs(m_fMaxX - m_fMinX);
		_int iVolume = 0;

		switch (m_eType)
		{
		case FIRST_SLIDER:
			iVolume = GI->Get_AllChannelVolume();
			m_tInfo.fX = m_fMinX + (m_fLength * iVolume);
			m_iPercent = iVolume * 100.f;
			break;

		case SECOND_SLIDER:
			iVolume = GI->Get_ChannelVolume(CHANNELID::SOUND_BGM);
			m_tInfo.fX = m_fMinX + (m_fLength * iVolume);
			m_iPercent = iVolume * 100.f;
			break;

		case THIRD_SLIDER:
			iVolume = GI->Get_ChannelVolume(CHANNELID::SOUND_UI);
			m_tInfo.fX = m_fMinX + (m_fLength * iVolume);
			m_iPercent = iVolume * 100.f;
			break;
		}
	}

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	m_bActive = bActive;
}

HRESULT CUI_Setting_Slider::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Setting_Slider::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	Set_SliderRange();

	m_bActive = true;
	m_iPercent = 100;

	m_bUseMouse = true;
	
	return S_OK;
}

void CUI_Setting_Slider::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (m_eType >= UI_SETTING_SLIDERTYPE::SLIDERTYPE_END)
			return;

		// 타입별로 Slider BG의 총Width를 1로두고,
		// 슬라이더 커서의 현재 위치의 값을 SoundManager의 변수에 보내준다.

		// 슬라이더 ratio로 percent를 계산한다
		m_fLength = fabs(m_fMaxX - m_fMinX);

		_float fCurPosX = m_fLength - fabs(m_fMaxX - m_tInfo.fX); // 슬라이더 시작 기준 현재 내가 있는 상대적 위치
		m_iPercent = _int((fCurPosX / m_fLength) * 100.f);

		switch (m_eType)
		{
		case UI_SETTING_SLIDERTYPE::FIRST_SLIDER:
			GI->Set_AllChannelVolume(fCurPosX / m_fLength);
			break;

		case UI_SETTING_SLIDERTYPE::SECOND_SLIDER:
			GI->Set_ChannelVolume(CHANNELID::SOUND_BGM, fCurPosX / m_fLength);
			break;

		case UI_SETTING_SLIDERTYPE::THIRD_SLIDER:
			for (_uint i = 0; i < CHANNELID::MAXCHANNEL; ++i)
			{
				if (i == CHANNELID::SOUND_BGM)
					continue;

				GI->Set_ChannelVolume(CHANNELID(i), fCurPosX / m_fLength);
			}
			break;
		}
		//

		__super::Tick(fTimeDelta);
	}
}

void CUI_Setting_Slider::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (0 > m_iPercent || 100 < m_iPercent)
			return;

		if (SLIDERTYPE_END == m_eType)
			return;

		CRenderer::TEXT_DESC PercentDesc = {};
		
		PercentDesc.strText = to_wstring(m_iPercent);
		PercentDesc.strFontTag = L"Default_Bold";
		PercentDesc.vScale = { 0.5f, 0.5f };

		_int iNumstr = PercentDesc.strText.size();
		_float fOffsetX = (3 - iNumstr) * 10.f;
		PercentDesc.vPosition = _float2(m_vTextPos.x + fOffsetX, m_vTextPos.y);

		PercentDesc.vColor = { 1.f, 1.f, 1.f, 1.f };

		m_pRendererCom->Add_Text(PercentDesc);

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
	}
}

HRESULT CUI_Setting_Slider::Render()
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

void CUI_Setting_Slider::On_MouseEnter(_float fTimeDelta)
{
}

void CUI_Setting_Slider::On_Mouse(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_Mouse(fTimeDelta);

		// Type에 따라서 증감되는 것이 다르다.
		Key_Input(fTimeDelta);
	}
}

void CUI_Setting_Slider::On_MouseExit(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::On_MouseExit(fTimeDelta);
	}
}

void CUI_Setting_Slider::On_MouseDragEnter(_float fTimeDelta)
{
}

void CUI_Setting_Slider::On_MouseDrag(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (KEY_HOLD(KEY::LBTN))
		{
			m_tInfo.fX += GI->Get_DIMMoveState(DIMM::DIMM_X);

			if (m_tInfo.fX >= m_fMaxX)
				m_tInfo.fX = m_fMaxX;

			if (m_tInfo.fX <= m_fMinX)
				m_tInfo.fX = m_fMinX;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));
		}
	}
}

void CUI_Setting_Slider::On_MouseDragExit(_float fTimeDelta)
{
}

HRESULT CUI_Setting_Slider::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Slider"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Setting_Slider::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Setting_Slider::Bind_ShaderResources()
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

void CUI_Setting_Slider::Set_SliderRange()
{
	// 슬라이더가 만들어질때 Range를 설정한다.

	// Setting창에서의 설정 -> 필요시 switch문 추가
	m_fMinX = m_tInfo.fX;
	m_fMaxX = m_tInfo.fX + 150.f;

	_float fOffset = 56.f;

	if (FIRST_SLIDER == m_eType)
		m_vTextPos = _float2(880.f, 385.f);
	else if (SECOND_SLIDER == m_eType)
		m_vTextPos = _float2(880.f, 385.f + fOffset);
	else
		m_vTextPos = _float2(880.f, 385.f + (fOffset * 2.f - 12.f));
}

void CUI_Setting_Slider::Key_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LBTN))
	{
	}
}

CUI_Setting_Slider* CUI_Setting_Slider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_SETTING_SLIDERTYPE eType)
{
	CUI_Setting_Slider* pInstance = new CUI_Setting_Slider(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Setting_Slider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Setting_Slider::Clone(void* pArg)
{
	CUI_Setting_Slider* pInstance = new CUI_Setting_Slider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Setting_Slider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Setting_Slider::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
