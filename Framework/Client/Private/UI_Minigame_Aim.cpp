#include "stdafx.h"
#include "UI_Minigame_Aim.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "Camera.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Vehicle_Flying.h"
#include "UIMinigame_Manager.h"

CUI_Minigame_Aim::CUI_Minigame_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minigame_Aim")
{
}

CUI_Minigame_Aim::CUI_Minigame_Aim(const CUI_Minigame_Aim& rhs)
	: CUI(rhs)
{
}

void CUI_Minigame_Aim::Set_Owner(CVehicle_Flying* pOwner)
{
	if (nullptr == pOwner)
		return;

	m_pOwner = pOwner;

	CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());

	if (TEXT("Vehicle_EnemyBiplane") == m_pOwner->Get_ObjectTag())
		m_iTextureIndex = 1;
}

HRESULT CUI_Minigame_Aim::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minigame_Aim::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMinSize = _float2(m_vOriginSize.x * 0.8f, m_vOriginSize.y * 0.8f);

	m_bActive = false;
	m_bResize = false;

	return S_OK;
}

void CUI_Minigame_Aim::Tick(_float fTimeDelta)
{
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
//		if (!CUI_Manager::GetInstance()->Is_DefaultSettingOn())
//			m_bActive = false;
//		else
//			m_bActive = true;

		// 만약에 플레이어가 Aim을 잡고있는 상태면 Set_Active = true;
		if (true == CUIMinigame_Manager::GetInstance()->Is_AimActive())
		{
			if (false == m_bActive)
				m_bActive = true;
		}
		else
		{
			if (true == m_bActive)
				m_bActive = false;
		}

		if (m_bActive)
		{
			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

			_float4 Temp;
			XMStoreFloat4(&Temp, pTransform->Get_Position());
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());

			if (!m_bResize)
			{
				m_tInfo.fCX -= fTimeDelta * 20.f;
				m_tInfo.fCY -= fTimeDelta * 20.f;

				if (m_tInfo.fCX <= m_vMinSize.x)
				{
					m_bResize = true;
					m_tInfo.fCX = m_vMinSize.x;
					m_tInfo.fCY = m_vMinSize.y;
				}

				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));

			}
			else
			{
				m_tInfo.fCX += fTimeDelta * 20.f;
				m_tInfo.fCY += fTimeDelta * 20.f;

				if (m_tInfo.fCX >= m_vOriginSize.x)
				{
					m_bResize = false;
					m_tInfo.fCX = m_vOriginSize.x;
					m_tInfo.fCY = m_vOriginSize.y;
				}

				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			}

			Update_Distance();

			__super::Tick(fTimeDelta);
		}
	}
	
}

void CUI_Minigame_Aim::LateTick(_float fTimeDelta)
{
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (m_bActive)
		{
			_float4 vCamPos = GI->Get_CamPosition();
			_vector vTempForDistance = m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos);
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));
			CTransform* pPlayerTransform = CUI_Manager::GetInstance()->Get_Character()->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pPlayerTransform)
				return;
			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

			_vector vTemp = (pTransform->Get_Position()) - (pPlayerTransform->Get_Position());
			_float fTotarget = XMVectorGetX(XMVector3Length(vTemp));

			if (fTotarget > 0.001f)
			{
				_float4x4 matTargetWorld = pTransform->Get_WorldFloat4x4();
 				matTargetWorld._42 += 1.f;

				_float4x4 matWorld;
				matWorld = matTargetWorld;
				_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
				_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

				_float4x4 matWindow;
				XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

				_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];
				vWindowPos.x /= vWindowPos.z;
				vWindowPos.y /= vWindowPos.z;

				m_tInfo.fX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
				m_tInfo.fY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION,
					XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

				if ((0 <= m_tInfo.fX && g_iWinSizeX >= m_tInfo.fX) &&
					(0 <= m_tInfo.fY && g_iWinSizeY >= m_tInfo.fY))
				{
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
						if (CUI_Manager::GetInstance()->Is_FadeFinished())
						{
							// AddText
							wstring strDistance = to_wstring(_uint(m_fDistance)) + TEXT("M");
							_int iLength = strDistance.length() - 1;
							_float2 vFontPos = _float2(m_tInfo.fX - 6.8f - (iLength * (6.8f - iLength)), m_tInfo.fY + 7.f);

							CRenderer::TEXT_DESC TextDesc = {};
							TextDesc.strText = strDistance;
							TextDesc.strFontTag = L"Default_Bold";
							TextDesc.vScale = { 0.3f, 0.3f };
							TextDesc.vColor = _float4(1.f, 1.f, 1.f, 1.f);
							TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y);
							m_pRendererCom->Add_Text(TextDesc);

							m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
						}

						__super::LateTick(fTimeDelta);
					}
				}
			}
		}
	}
}

HRESULT CUI_Minigame_Aim::Render()
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

HRESULT CUI_Minigame_Aim::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_AimCursor"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Minigame_Aim::Ready_State()
{
	_float fBtnSize = 64.f;

	m_tInfo.fCX = fBtnSize;
	m_tInfo.fCY = fBtnSize;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Minigame_Aim::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex)))
		return E_FAIL;

	return S_OK;
}

void CUI_Minigame_Aim::Update_Distance()
{
	CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
	CCharacter* pCharacter = pPlayer->Get_Character();
	if (nullptr == pCharacter)
		return;

	Vec4 vPlayerPos = pCharacter->Get_CharacterPosition();
	Vec4 vOwnerPos = m_pOwner->Get_Component<CTransform>(L"Com_Transform")->Get_Position();

	m_fDistance = XMVectorGetX(XMVector3Length(vPlayerPos - vOwnerPos));
}

CUI_Minigame_Aim* CUI_Minigame_Aim::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minigame_Aim* pInstance = new CUI_Minigame_Aim(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minigame_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minigame_Aim::Clone(void* pArg)
{
	CUI_Minigame_Aim* pInstance = new CUI_Minigame_Aim(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minigame_Aim");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minigame_Aim::Free()
{
	__super::Free();
	
	Safe_Release(m_pTextureCom_Target);
	Safe_Release(m_pTextureCom);
}
