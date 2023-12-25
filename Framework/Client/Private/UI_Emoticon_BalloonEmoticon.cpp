#include "stdafx.h"
#include "UI_Emoticon_BalloonEmoticon.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Emoticon_BalloonEmoticon::CUI_Emoticon_BalloonEmoticon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Emoticon_BalloonEmoticon")
{
}

CUI_Emoticon_BalloonEmoticon::CUI_Emoticon_BalloonEmoticon(const CUI_Emoticon_BalloonEmoticon& rhs)
	: CUI(rhs)
{
}

void CUI_Emoticon_BalloonEmoticon::Set_TextureIndex(_uint iIndex)
{
	m_iTextureIndex = iIndex;

	switch (m_iTextureIndex)
	{
	case 0:
		m_iMaxIndex = 15;
		break;
	case 1:
		m_iMaxIndex = 13;
		break;
	case 2:
		m_iMaxIndex = 13;
		break;
	case 3:
		m_iMaxIndex = 15;
		break;
	case 4:
		m_iMaxIndex = 15;
		break;
	case 5:
		m_iMaxIndex = 15;
		break;
	case 6:
		m_iMaxIndex = 15;
		break;
	case 7:
		m_iMaxIndex = 15;
		break;
	case 8:
		m_iMaxIndex = 15;
		break;
	}

	m_fFrame = 0.f;
	m_bFinished = false;
	m_fTimeAcc = 0.f;
	m_bActive = true;
	m_fAlpha = 0.9f;
}

HRESULT CUI_Emoticon_BalloonEmoticon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Emoticon_BalloonEmoticon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;
	m_fAlpha = 0.9f;

	return S_OK;
}

void CUI_Emoticon_BalloonEmoticon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (!m_bFinished)
		{
			m_fFrame += 10.f * fTimeDelta;

			if (_uint(m_fFrame) >= m_iMaxIndex)
			{
				m_fFrame = _float(m_iMaxIndex);
				m_bFinished = true;
			}
		}

//		__super::Tick(fTimeDelta);
	}
}

void CUI_Emoticon_BalloonEmoticon::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (_uint(m_fFrame) > m_iMaxIndex)
			return;

		if (m_bFinished)
		{
			m_fTimeAcc += fTimeDelta;

			if (m_fTimeAcc > 1.5f)
			{
				CUI_Manager::GetInstance()->OnOff_EmoticonBalloon(false);
				m_fTimeAcc = 0.f;
			}
		}

		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		//__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Emoticon_BalloonEmoticon::Render()
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

HRESULT CUI_Emoticon_BalloonEmoticon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_1"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_2"),
		TEXT("Com_TextureSecond"), (CComponent**)&m_pTextureSecond)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_3"),
		TEXT("Com_TextureThird"), (CComponent**)&m_pTextureThird)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_4"),
		TEXT("Com_TextureFourth"), (CComponent**)&m_pTextureFourth)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_5"),
		TEXT("Com_TextureFifth"), (CComponent**)&m_pTextureFifth)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_6"),
		TEXT("Com_TextureSixth"), (CComponent**)&m_pTextureSixth)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_7"),
		TEXT("Com_TextureSeventh"), (CComponent**)&m_pTextureSeventh)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_8"),
		TEXT("Com_TextureEighth"), (CComponent**)&m_pTextureEighth)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Emoticon_BalloonEmoticon::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Emoticon_BalloonEmoticon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	switch (m_iTextureIndex)
	{
	case 0:
		if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 1:
		if (FAILED(m_pTextureSecond->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 2:
		if (FAILED(m_pTextureThird->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 3:
		if (FAILED(m_pTextureFourth->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 4:
		if (FAILED(m_pTextureFifth->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 5:
		if (FAILED(m_pTextureSixth->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 6:
		if (FAILED(m_pTextureSeventh->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	case 7:
		if (FAILED(m_pTextureEighth->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_fFrame))))
			return E_FAIL;
		break;
	}

	return S_OK;
}

void CUI_Emoticon_BalloonEmoticon::Calculate_AnimUV(_uint iIndex)
{
	_int iRow = iIndex / 4;
	_int iCol = iIndex / 4;
	_float2 vUVPosition = _float2(iCol / 4.f, iRow / 4.f);

//	float frameDuration = 0.25f; // 한 프레임의 지속 시간
//	int totalFrames = 16; // 전체 프레임 수
//
//	// 시간에 따라 현재 프레임 계산
//	int currentFrame = int(time / frameDuration) % totalFrames;
//
//	// 현재 프레임의 텍스처 좌표로 설정
//	uv.x += currentFrame / 4.0f;
//
//
//	return uv;
}

CUI_Emoticon_BalloonEmoticon* CUI_Emoticon_BalloonEmoticon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Emoticon_BalloonEmoticon* pInstance = new CUI_Emoticon_BalloonEmoticon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Emoticon_BalloonEmoticon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Emoticon_BalloonEmoticon::Clone(void* pArg)
{
	CUI_Emoticon_BalloonEmoticon* pInstance = new CUI_Emoticon_BalloonEmoticon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Emoticon_BalloonEmoticon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Emoticon_BalloonEmoticon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureEighth);
	Safe_Release(m_pTextureSeventh);
	Safe_Release(m_pTextureSixth);
	Safe_Release(m_pTextureFifth);
	Safe_Release(m_pTextureFourth);
	Safe_Release(m_pTextureThird);
	Safe_Release(m_pTextureSecond);
	Safe_Release(m_pTextureCom);
}
