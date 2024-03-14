#include "stdafx.h"
#include "UI_Grandprix_Target.h"
#include "GameInstance.h"

#include "UI_Manager.h"
#include "UIMinigame_Manager.h"

#include "Camera_Manager.h"
#include "Camera.h"

#include "Vehicle_Flying.h"
#include "Character.h"

CUI_Grandprix_Target::CUI_Grandprix_Target(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Grandprix_Target")
{
}

CUI_Grandprix_Target::CUI_Grandprix_Target(const CUI_Grandprix_Target& rhs)
	: CUI(rhs)
{
}

void CUI_Grandprix_Target::Set_Target(CVehicle_Flying* pOwner)
{
	if (nullptr == pOwner)
	{
		m_pOwner = nullptr;
		return;
	}

	_float2 vScreenPos = CUI_Manager::GetInstance()->Get_ProjectionPosition(pOwner->Get_Component<CTransform>(L"Com_Transform"));
	if (vScreenPos.x == -1.f && vScreenPos.y == -1.f)
	{
		m_pOwner = nullptr;
		return;
	}

	m_pOwner = pOwner;
	m_bActive = true;
}

HRESULT CUI_Grandprix_Target::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_Target::Initialize(void* pArg)
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

void CUI_Grandprix_Target::Tick(_float fTimeDelta)
{
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (true == m_bActive)
		{
			if (nullptr == m_pOwner || true == m_pOwner->Is_Dead())
			{
				Set_Active(false);
				return;
			}

			// Rotation
			m_pTransformCom->Rotation_Acc(XMVectorSet(0.f, 0.f, 1.f, 0.f), fTimeDelta);

			__super::Tick(fTimeDelta);
		}
	}
	
}

void CUI_Grandprix_Target::LateTick(_float fTimeDelta)
{
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (true == m_bActive)
		{
			if (nullptr == m_pOwner || true == m_pOwner->Is_Dead())
			{
				Set_Active(false);
				return;
			}

			// Error»óÅÂ¸é return
			if (true == CUIMinigame_Manager::GetInstance()->Is_RaderError())
				return;

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
							m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);

						__super::LateTick(fTimeDelta);
					}
				}
			}
		}
	}
}

HRESULT CUI_Grandprix_Target::Render()
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

HRESULT CUI_Grandprix_Target::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Minigame_Grandprix_Target"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_Grandprix_Target::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_Target::Bind_ShaderResources()
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

CUI_Grandprix_Target* CUI_Grandprix_Target::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Grandprix_Target* pInstance = new CUI_Grandprix_Target(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_Target::Clone(void* pArg)
{
	CUI_Grandprix_Target* pInstance = new CUI_Grandprix_Target(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_Target");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_Target::Free()
{
	__super::Free();
	
	Safe_Release(m_pTextureCom);
}
