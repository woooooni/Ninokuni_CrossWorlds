#include "stdafx.h"
#include "UI_Milepost.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Camera_Manager.h"
#include "Camera.h"

CUI_Milepost::CUI_Milepost(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType)
	: CUI(pDevice, pContext, L"UI_Milepost")
	, m_eType(eType)
{
}

CUI_Milepost::CUI_Milepost(const CUI_Milepost& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Milepost::Set_Active(_bool bActive)
{
	// Set_TargetPosition을 먼저 하고 Set_Active해야함.

	if (!m_bGoal)
		return;

	m_bActive = bActive;
}

HRESULT CUI_Milepost::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Milepost::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_bActive = true;

	if (nullptr == m_pPlayer)
	{
		CPlayer* pPlayer = CGame_Manager::GetInstance()->Get_Player();
		if (nullptr == pPlayer)
			return E_FAIL;

		CCharacter* pCharacter = pPlayer->Get_Character();
		if (nullptr == pCharacter)
			return E_FAIL;

		m_pPlayer = pCharacter;
	}

	Set_TargetPosition(_float4(-69.5f, -2.7f, -10.f, 1.f));

	return S_OK;
}

void CUI_Milepost::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		__super::Tick(fTimeDelta);
	}
}

void CUI_Milepost::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr != m_pPlayer)
		{
			if (m_bGoal)
			{
				// 카메라 범위
				_float4 vCamPos = GI->Get_CamPosition();
				_vector vCameraPos = XMLoadFloat4(&vCamPos);
				_vector vTargetToCamera = XMLoadFloat4(&m_vTargetPos) - vCameraPos;
				_float fDistance = XMVectorGetX(XMVector3Length(vTargetToCamera));

				vTargetToCamera = XMVector3Normalize(vTargetToCamera);
				CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
				if (nullptr == pCurCamera)
					return;

				CTransform* pCameraTrans = pCurCamera->Get_Transform();
				if (nullptr == pCameraTrans)
					return;

				_vector vCameraForward = pCameraTrans->Get_State(CTransform::STATE_LOOK);
				vCameraForward = XMVector3Normalize(vCameraForward);

				_float fAngle = XMVectorGetX(XMVector3Dot(vTargetToCamera, vCameraForward));
				if (5.f < fDistance)
				{
					// Degree는 -180 ~ 180
					if (fAngle >= XMConvertToRadians(40.f) && fAngle <= XMConvertToRadians(140.f) ||
						fAngle <= XMConvertToRadians(-40.f) && fAngle >= XMConvertToRadians(-140.f))
					{
						_matrix matTemp = XMMatrixIdentity();
						matTemp.r[3] = XMLoadFloat4(&m_vTargetPos);
						_float4x4 matTargetWorld;
						XMStoreFloat4x4(&matTargetWorld, matTemp);

						_float4x4 matWorld;
						matWorld = matTargetWorld;
						_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
						_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

						_float4x4 matWindow;
						XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

						_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];

						vWindowPos.x /= vWindowPos.z;
						vWindowPos.y /= vWindowPos.z;

						m_vCurrentPos.x = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
						m_vCurrentPos.y = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

						// 0 ~ 900 . 200 ~ 700
						if (m_vCurrentPos.y <= 200.f)
							m_vCurrentPos.y = 200.f;

						if (m_vCurrentPos.y >= 700)
							m_vCurrentPos.y = 700.f;

						m_tInfo.fX = m_vCurrentPos.x;
						m_tInfo.fY = m_vCurrentPos.y;

						m_pTransformCom->Set_State(CTransform::STATE_POSITION,
							XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
					}
					else
					{
						if (m_vCurrentPos.x > g_iWinSizeX)
							m_vCurrentPos.x = (g_iWinSizeX - m_tInfo.fCX);

						if (m_vCurrentPos.x < 0)
							m_vCurrentPos.x = (m_tInfo.fCX);

						if (m_vCurrentPos.y > g_iWinSizeY)
							m_vCurrentPos.y = (g_iWinSizeY - m_tInfo.fCY);
						
						if (m_vCurrentPos.y < 0)
							m_vCurrentPos.y = (m_tInfo.fCY);

						m_tInfo.fX = m_vCurrentPos.x;
						m_tInfo.fY = m_vCurrentPos.y;

						m_pTransformCom->Set_State(CTransform::STATE_POSITION,
							XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));
					}

					if (m_eType == UI_MILEPOST::MILEPOST_FLAG)
					{
						CTransform* pPlayerTransform = m_pPlayer->Get_Component<CTransform>(L"Com_Transform");
						if (nullptr == pPlayerTransform)
							return;
						_vector vTemp = XMLoadFloat4(&m_vTargetPos) - (pPlayerTransform->Get_Position());
						_float fTotarget = XMVectorGetX(XMVector3Length(vTemp));

						wstring strDistance = to_wstring(_uint(fTotarget));
						_int iLength = strDistance.length() - 1;
						_float2 vFontPos = _float2(m_vCurrentPos.x - 6.8f - (iLength * (6.8f - iLength)), m_vCurrentPos.y + 4.f);

						CRenderer::TEXT_DESC TextDesc = {};
						TextDesc.strText = strDistance;
						TextDesc.strFontTag = L"Default_Bold";
						TextDesc.vScale = { 0.35f, 0.35f };
						TextDesc.vColor = _float4(0.655f, 0.475f, 0.325f, 1.f);
						TextDesc.vPosition = _float2(vFontPos.x - 1.f, vFontPos.y);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x + 1.f, vFontPos.y);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y - 1.f);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y + 1.f);
						m_pRendererCom->Add_Text(TextDesc);

						TextDesc.vColor = _float4(1.f, 0.973f, 0.588f, 1.f);
						TextDesc.vPosition = _float2(m_vCurrentPos.x - 7.f - (iLength * (6.8f - iLength)), m_vCurrentPos.y + 4.f);
						m_pRendererCom->Add_Text(TextDesc);
					}
				}
				else
				{
					m_bGoal = false;
					m_bActive = false;
				}
			}
		}
		
		if (m_bGoal)
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

		__super::LateTick(fTimeDelta);
	}

}

HRESULT CUI_Milepost::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(1);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_Milepost::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	switch (m_eType)
	{
	case UI_MILEPOST::MILEPOST_FLAG:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Flag"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;

	case UI_MILEPOST::MILEPOST_ARROW:
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Arrow"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

HRESULT CUI_Milepost::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Milepost::Bind_ShaderResources()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4())))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture")))
		return E_FAIL;

	return S_OK;
}

void CUI_Milepost::Calculate_Distance()
{
	// m_fDistance를 채워준다. 거리계산 -> Add_Text하게될 변수
}

CUI_Milepost* CUI_Milepost::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MILEPOST eType)
{
	CUI_Milepost* pInstance = new CUI_Milepost(pDevice, pContext, eType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Milepost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Milepost::Clone(void* pArg)
{
	CUI_Milepost* pInstance = new CUI_Milepost(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Milepost");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Milepost::Free()
{
	__super::Free();

	Safe_Release(m_pPlayer);
	Safe_Release(m_pTextureCom);
}
