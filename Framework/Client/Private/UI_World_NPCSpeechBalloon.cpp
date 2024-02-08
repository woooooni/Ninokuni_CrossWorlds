#include "stdafx.h"
#include "UI_World_NPCSpeechBalloon.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "GameNpc.h"
#include "HierarchyNode.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_World_NPCSpeechBalloon::CUI_World_NPCSpeechBalloon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_World_NPCSpeechBalloon")
{
}

CUI_World_NPCSpeechBalloon::CUI_World_NPCSpeechBalloon(const CUI_World_NPCSpeechBalloon& rhs)
	: CUI(rhs)
{
}

void CUI_World_NPCSpeechBalloon::Set_Active(_bool bActive)
{
	if (bActive)
	{

	}
	else
	{
		m_bHide = true;
		m_fHideTimeAcc = 0.f;
		m_bResizeDone = false;

		m_tInfo.fCX = m_vMinSize.x;
		m_tInfo.fCY = m_vMinSize.y;

		m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	}

	m_bActive = bActive;
}

void CUI_World_NPCSpeechBalloon::Set_Owner(CGameObject* pOwner, _float fOffsetY)
{
	if (nullptr == pOwner)
		return;

	m_pOwner = dynamic_cast<CGameNpc*>(pOwner);
	m_fOffsetY = fOffsetY;

	if (FAILED(Ready_State()))
		return;

	Set_Active(false);
}

void CUI_World_NPCSpeechBalloon::Set_Balloon(const wstring& pText)
{
	m_strContents = pText;
}

HRESULT CUI_World_NPCSpeechBalloon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_World_NPCSpeechBalloon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_bActive = false;
	m_bResizeDone = false;

	return S_OK;
}

void CUI_World_NPCSpeechBalloon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pOwner)
			return;

		if (true == CUI_Manager::GetInstance()->Is_Dialog_Active())
			return;

		m_fActiveTimeAcc += fTimeDelta;

		if (5.f < m_fActiveTimeAcc)
		{
			m_fActiveTimeAcc = 0.f;
			Set_Active(false);
		}
		else
		{
			if (false == m_bResizeDone)
			{
				if (m_vOriginSize.y <= m_tInfo.fCY)
				{
					m_bResizeDone = true;
					m_tInfo.fCX = m_vOriginSize.x;
					m_tInfo.fCY = m_vOriginSize.y;
				}
				else
				{
					m_tInfo.fCX += fTimeDelta * m_fSpeed.x;
					m_tInfo.fCY += fTimeDelta * m_fSpeed.y;
				}

				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			}
		}

		__super::Tick(fTimeDelta);
	}
	else
	{
		if (true == m_bHide)
		{
			m_fHideTimeAcc += fTimeDelta;

			if (5.f < m_fHideTimeAcc)
			{
				m_bHide = false;
				m_fHideTimeAcc = 0.f;
			}
		}
	}

}

void CUI_World_NPCSpeechBalloon::LateTick(_float fTimeDelta)
{
	//if (m_bActive)
	{
		if (nullptr != m_pOwner && m_bHide == false)
		{
			if (true == CUI_Manager::GetInstance()->Is_Dialog_Active())
				return;

			if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
				return;

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
			_vector vNPCToCamera = pTransform->Get_Position() - vCameraPos;

			vNPCToCamera = XMVector3Normalize(vNPCToCamera);
			CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
			if (nullptr == pCurCamera)
				return;

			CTransform* pCameraTrans = pCurCamera->Get_Transform();
			if (nullptr == pCameraTrans)
				return;

			_vector vCameraForward = pCameraTrans->Get_State(CTransform::STATE_LOOK);
			vCameraForward = XMVector3Normalize(vCameraForward);
			_float fAngle = XMVectorGetX(XMVector3Dot(vNPCToCamera, vCameraForward));

			// 플레이어와의 거리를 구한다.
			CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
			if (nullptr == pPlayer)
				return;
			CCharacter* pCharacter = pPlayer->Get_Character();
			if (nullptr == pCharacter)
				return;

			CTransform* pPlayerTransform = pCharacter->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pPlayerTransform)
				return;
			_vector vTemp = (pTransform->Get_Position()) - (pPlayerTransform->Get_Position());
			_float fToTarget = XMVectorGetX(XMVector3Length(vTemp));

			if (CUI_Manager::GetInstance()->Is_FadeFinished())
			{
				if ((0.001 < fToTarget) && (fToTarget < 5.f))
				{
					if(false == m_bActive)
						Set_Active(true);
				}
			}

			// Active == true면
			if (true == m_bActive)
			{
				if ((fAngle >= XMConvertToRadians(0.f) && fAngle <= XMConvertToRadians(180.f)))
				{
					if (m_bResizeDone)
					{
						if (true == CUI_Manager::GetInstance()->Is_Background_Active())
							return;

						_int iLength = m_strContents.length() - 1;
						_float2 vFontPos = _float2(m_vTextPos.x - (iLength * 6.f), m_vTextPos.y - 10.f);

						CRenderer::TEXT_DESC TextDesc = {};
						TextDesc.strText = m_strContents;
						TextDesc.strFontTag = L"Default_Bold";
						TextDesc.vScale = { 0.35f, 0.35f };
						TextDesc.vColor = _float4(0.047f, 0.024f, 0.004f, 1.f);
						TextDesc.vPosition = vFontPos;
						m_pRendererCom->Add_Text(TextDesc);
					}

					m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
				}
			}
		}
	}
}

HRESULT CUI_World_NPCSpeechBalloon::Render()
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

HRESULT CUI_World_NPCSpeechBalloon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_NPC_Default_SpeechBalloon"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_World_NPCSpeechBalloon::Ready_State()
{
	m_tInfo.fCX = 400.f * 0.5f;
	m_tInfo.fCY = 108.f * 0.5f;

	m_fSpeed.x = m_tInfo.fCX * 5.f;
	m_fSpeed.y = m_tInfo.fCY * 5.f;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMinSize = _float2(m_tInfo.fCX * 0.4f, m_tInfo.fCY * 0.4f);
	
	m_tInfo.fCX = m_vMinSize.x * 100.f;
	m_tInfo.fCY = m_vMinSize.y * 100.f;
	
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_World_NPCSpeechBalloon::Bind_ShaderResources()
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

CUI_World_NPCSpeechBalloon* CUI_World_NPCSpeechBalloon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_World_NPCSpeechBalloon* pInstance = new CUI_World_NPCSpeechBalloon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_World_NPCSpeechBalloon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_World_NPCSpeechBalloon::Clone(void* pArg)
{
	CUI_World_NPCSpeechBalloon* pInstance = new CUI_World_NPCSpeechBalloon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_World_NPCSpeechBalloon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_World_NPCSpeechBalloon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
