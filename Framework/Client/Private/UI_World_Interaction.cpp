#include "stdafx.h"
#include "UI_World_Interaction.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "Animals.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"

CUI_World_Interaction::CUI_World_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_World_Interaction")
{
}

CUI_World_Interaction::CUI_World_Interaction(const CUI_World_Interaction& rhs)
	: CUI(rhs)
{
}

void CUI_World_Interaction::Set_Active(_bool bActive)
{
	if (nullptr == m_pOwner)
		return;

	if (bActive)
	{
		CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());
	}

	m_bActive = bActive;

}

void CUI_World_Interaction::Set_Owner(CGameObject* pOwner)
{
	if (nullptr == pOwner)
		return;

	m_pOwner = pOwner;

	if (TEXT("Animal_Cat") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_Dochi") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_PolarBear") == m_pOwner->Get_ObjectTag())
	{
		m_eType = UI_INTERACTION_TYPE::INTERACTION_PICKUP;
		m_vOffset = _float2(0.f, 0.8f);
		m_vOriginOffset = m_vOffset;
	}
	else if (TEXT("Animal_DuckGoo") == m_pOwner->Get_ObjectTag())
	{
		m_eType = UI_INTERACTION_TYPE::INTERACTION_PICKUP;
		m_vOffset = _float2(0.f, 1.2f);
		m_vOriginOffset = m_vOffset;
	}
	else
	{
		m_eType = UI_INTERACTION_TYPE::INTERACTION_PICKUP;
		m_vOffset = _float2(0.f, 0.8f);
		m_vOriginOffset = m_vOffset;
	}
}

HRESULT CUI_World_Interaction::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_World_Interaction::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_vOriginSize = _float2(m_tInfo.fCX, m_tInfo.fCY);
	m_vMinSize = _float2(m_vOriginSize.x * 0.8f, m_vOriginSize.y * 0.8f);

	m_bActive = true;
	m_bResize = false;

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

	return S_OK;
}

void CUI_World_Interaction::Tick(_float fTimeDelta)
{
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (!CUI_Manager::GetInstance()->Is_DefaultSettingOn())
			m_bActive = false;
		else
			m_bActive = true;

		if (m_bActive)
		{
			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

			_float4 Temp;
			XMStoreFloat4(&Temp, pTransform->Get_Position());
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());

			if (!m_bResize)
			{
				m_tInfo.fCX -= fTimeDelta * 10.f;
				m_tInfo.fCY -= fTimeDelta * 10.f;

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
				m_tInfo.fCX += fTimeDelta * 10.f;
				m_tInfo.fCY += fTimeDelta * 10.f;

				if (m_tInfo.fCX >= m_vOriginSize.x)
				{
					m_bResize = false;
					m_tInfo.fCX = m_vOriginSize.x;
					m_tInfo.fCY = m_vOriginSize.y;
				}

				m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
			}

			Update_ButtonIcon();
			__super::Tick(fTimeDelta);
		}
	}
	
}

void CUI_World_Interaction::LateTick(_float fTimeDelta)
{
	if (LEVELID::LEVEL_TOOL != GI->Get_CurrentLevel())
	{
		if (m_bActive)
		{
			if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
				return;

			_float4 vCamPos = GI->Get_CamPosition();
			_vector vTempForDistance = m_pTransformCom->Get_Position() - XMLoadFloat4(&vCamPos);
			_float fDistance = XMVectorGetX(XMVector3Length(vTempForDistance));

			CTransform* pPlayerTransform = m_pPlayer->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pPlayerTransform)
				return;
			CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

			_vector vTemp = (pTransform->Get_Position()) - (pPlayerTransform->Get_Position());
			_float fTotarget = XMVectorGetX(XMVector3Length(vTemp));

			if (fTotarget > 0.001f)
			{
				_float4x4 matTargetWorld = pTransform->Get_WorldFloat4x4();
 				matTargetWorld._42 += m_vOffset.y;

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

				if (fTotarget < 5.f)
				{
					CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");

					_float4x4 matTargetWorld = pTransform->Get_WorldFloat4x4();
					matTargetWorld._42 += m_vOffset.y;

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

					if (fDistance < 35.f)
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
	
}

HRESULT CUI_World_Interaction::Render()
{
	if (m_bActive)
	{
		if (UI_INTERACTION_TYPE::INTERACTION_END <= m_eType)
			return E_FAIL;

		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(1);

		m_pVIBufferCom->Render();

		__super::Render();
	}

	return S_OK;
}

HRESULT CUI_World_Interaction::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_InteractionWorld"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_World_Interaction::Ready_State()
{
	_float fBtnSize = 102.f * 0.55f;

	m_tInfo.fCX = fBtnSize;
	m_tInfo.fCY = fBtnSize;

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

HRESULT CUI_World_Interaction::Bind_ShaderResources()
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

void CUI_World_Interaction::Update_ButtonIcon()
{
	if (TEXT("Animal_Cat") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_Dochi") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_DuckGoo") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_PolarBear") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_Fox") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_Rabbit") == m_pOwner->Get_ObjectTag() ||
		TEXT("Animal_Ermine") == m_pOwner->Get_ObjectTag())
	{
		if (dynamic_cast<CAnimals*>(m_pOwner)->Lifting())
		{
			m_eType = UI_INTERACTION_TYPE::INTERACTION_PICKDOWN;
			m_vOffset = _float2(0.f, 0.f);
		}
		else
		{
			m_eType = UI_INTERACTION_TYPE::INTERACTION_PICKUP;
			m_vOffset = m_vOriginOffset;
		}
	}
}

CUI_World_Interaction* CUI_World_Interaction::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_World_Interaction* pInstance = new CUI_World_Interaction(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_World_interaction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_World_Interaction::Clone(void* pArg)
{
	CUI_World_Interaction* pInstance = new CUI_World_Interaction(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_World_interaction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_World_Interaction::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
