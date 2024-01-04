#include "stdafx.h"
#include "UI_World_NameTag.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "Character.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"

CUI_World_NameTag::CUI_World_NameTag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_NAMETAG eType)
	: CUI(pDevice, pContext, L"UI_World_NameTag")
	, m_eType(eType)
{
}

CUI_World_NameTag::CUI_World_NameTag(const CUI_World_NameTag& rhs)
	: CUI(rhs)
	,  m_eType(rhs.m_eType)
{
}

void CUI_World_NameTag::Set_Owner(CGameObject* pOwner)
{
	if (nullptr == pOwner)
		return;

	m_pOwner = dynamic_cast<CCharacter*>(pOwner);

	switch (m_pOwner->Get_CharacterType())
	{
	case CHARACTER_TYPE::SWORD_MAN:
		m_fOffsetY = 2.f;
		break;

	case CHARACTER_TYPE::DESTROYER:
		break;

	case CHARACTER_TYPE::ENGINEER:
		break;
	}
}

HRESULT CUI_World_NameTag::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_World_NameTag::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	return S_OK;
}

void CUI_World_NameTag::Tick(_float fTimeDelta)
{
	/*if (UI_NAMETAG::NAMETAG_GAMEPLAY == m_eType)
	{
		if (!CUI_Manager::GetInstance()->Is_DefaultSettingOn())
			m_bActive = false;
		else
			m_bActive = true;
	}

	if (m_bActive)
	{
		if (nullptr == m_pOwner)
			return;

		CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

		_float4 Temp;
		XMStoreFloat4(&Temp, pTransform->Get_Position());
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());

		__super::Tick(fTimeDelta);
	}*/
}

void CUI_World_NameTag::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pOwner)
		{
			_float4 vCamPos = GI->Get_CamPosition();
			_vector vTempForDistance = m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos);
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

			_float4x4 matTargetWorld = pTransform->Get_WorldFloat4x4();
			matTargetWorld._42 += m_fOffsetY;

			_float4x4 matWorld;
			matWorld = matTargetWorld;
			_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
			_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

			_float4x4 matWindow;
			XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

			_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];
			vWindowPos.x /= vWindowPos.z;
			vWindowPos.y /= vWindowPos.z;

			m_vTextPos.x = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
			m_vTextPos.y = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

			m_tInfo.fX = m_vTextPos.x;
			m_tInfo.fY = m_vTextPos.y;

			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

			_vector vCameraPos = XMLoadFloat4(&vCamPos);
			_vector vMonsterToCamera = pTransform->Get_Position() - vCameraPos;

			vMonsterToCamera = XMVector3Normalize(vMonsterToCamera);
			CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
			if (nullptr == pCurCamera)
				return;

			CTransform* pCameraTrans = pCurCamera->Get_Transform();
			if (nullptr == pCameraTrans)
				return;

			_vector vCameraForward = pCameraTrans->Get_State(CTransform::STATE_LOOK);
			vCameraForward = XMVector3Normalize(vCameraForward);

			_float fAngle = XMVectorGetX(XMVector3Dot(vMonsterToCamera, vCameraForward));

			if (fAngle >= XMConvertToRadians(0.f) && fAngle <= XMConvertToRadians(180.f))
			{
				wstring strName = CGame_Manager::GetInstance()->Get_UserName();
				_int iLength = strName.length() - 1;
				_float2 vFontPos = _float2(m_vTextPos.x - (iLength * 8.f), m_vTextPos.y);

				CRenderer::TEXT_DESC TextDesc = {};
				TextDesc.strText = strName;
				TextDesc.strFontTag = L"Default_Bold";
				TextDesc.vScale = { 0.4f, 0.4f };
				TextDesc.vColor = _float4(0.22f, 0.427f, 0.301f, 1.f);
				TextDesc.vPosition = _float2(vFontPos.x - 1.f, vFontPos.y);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(vFontPos.x + 1.f, vFontPos.y);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y - 1.f);
				m_pRendererCom->Add_Text(TextDesc);
				TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y + 1.f);
				m_pRendererCom->Add_Text(TextDesc);

				TextDesc.vColor = _float4(0.788f, 0.839f, 0.741f, 1.f);
				TextDesc.vPosition = vFontPos;
				m_pRendererCom->Add_Text(TextDesc);
			}
		}

		if (UI_NAMETAG::NAMETAG_LOBBY == m_eType)
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
		}
	}
}

HRESULT CUI_World_NameTag::Render()
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

HRESULT CUI_World_NameTag::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_NAMETAG::NAMETAG_LOBBY:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Lobby_NameTag"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Lobby_NameTag_FX"),
			TEXT("Com_FXTexture"), (CComponent**)&m_pFXTextureCom)))
			return E_FAIL;
		break;

	case UI_NAMETAG::NAMETAG_GAMEPLAY:
		break;
	}
	
	return S_OK;
}

HRESULT CUI_World_NameTag::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_World_NameTag::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", _uint(m_eType))))
		return E_FAIL;

	return S_OK;
}

CUI_World_NameTag* CUI_World_NameTag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_NAMETAG eType)
{
	CUI_World_NameTag* pInstance = new CUI_World_NameTag(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_World_NameTag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_World_NameTag::Clone(void* pArg)
{
	CUI_World_NameTag* pInstance = new CUI_World_NameTag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_World_NameTag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_World_NameTag::Free()
{
	__super::Free();

	if (NAMETAG_LOBBY == m_eType)
	{
		Safe_Release(m_pFXTextureCom);
		Safe_Release(m_pTextureCom);
	}
}
