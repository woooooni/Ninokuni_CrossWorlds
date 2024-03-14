#include "stdafx.h"
#include "UI_World_NPCTag.h"
#include "GameInstance.h"
#include "UI_Manager.h"

#include "GameNpc.h"
#include "HierarchyNode.h"
#include "Camera.h"
#include "Camera_Manager.h"
#include "Game_Manager.h"
#include "Player.h"
#include "Quest_Manager.h"

CUI_World_NPCTag::CUI_World_NPCTag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_World_NPCTag")
{
}

CUI_World_NPCTag::CUI_World_NPCTag(const CUI_World_NPCTag& rhs)
	: CUI(rhs)
{
}

void CUI_World_NPCTag::Set_Owner(CGameObject* pOwner, const wstring& strNameTag, _float fOffsetY, _bool bIsUnique)
{
	if (nullptr == pOwner)
		return;

	m_pOwner = dynamic_cast<CGameNpc*>(pOwner);
	m_fOffsetY = fOffsetY;
	m_strName = strNameTag;
	m_bIsUnique = bIsUnique;

	if (false == m_bIsUnique)
		m_vColor = _float4(1.f, 0.969f, 0.6f, 1.f);
	else
		m_vColor = _float4(0.365f, 0.863f, 0.82f, 1.f);

	m_bActive = true;
}

HRESULT CUI_World_NPCTag::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_World_NPCTag::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_fTimeAcc = 0.f;

	return S_OK;
}

void CUI_World_NPCTag::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pOwner)
			return;

		CTransform* pTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, pTransform->Get_Position());

		__super::Tick(fTimeDelta);
	}
}

void CUI_World_NPCTag::LateTick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (TEXT("Kuu") == m_pOwner->Get_ObjectTag())
		{
			if (false == CUI_Manager::GetInstance()->Is_DefaultSettingOn())
				return;
		}

		if (nullptr != m_pOwner)
		{
			if (CAMERA_TYPE::CUTSCENE_MAP == CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Key())
				return;

			if (CQuest_Manager::QUESTEVENT_TYPE::QUESTEVENT_ENDING == CQuest_Manager::GetInstance()->Get_CurQuestEvent() ||
				CQuest_Manager::QUESTEVENT_TYPE::QUESTEVENT_INVASION == CQuest_Manager::GetInstance()->Get_CurQuestEvent())
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

			if ((fAngle >= XMConvertToRadians(0.f) && fAngle <= XMConvertToRadians(180.f))
				&& ((0.001 < fToTarget) && (fToTarget < 7.f)))
			{
				if (CUI_Manager::GetInstance()->Is_FadeFinished())
				{
					_int iLength = m_strName.length() - 1;
					_float2 vFontPos = _float2(m_vTextPos.x - (iLength * 8.f), m_vTextPos.y);

					CRenderer::TEXT_DESC TextDesc = {};
					TextDesc.strText = m_strName;
					TextDesc.strFontTag = L"Default_Bold";
					TextDesc.vScale = { 0.4f, 0.4f };
					TextDesc.vColor = _float4(0.216f, 0.373f, 0.408f, 1.f);
					if (true == m_bIsUnique)
					{
						TextDesc.vPosition = _float2(vFontPos.x - 1.f, vFontPos.y);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x + 1.f, vFontPos.y);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y - 1.f);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y + 1.f);
						m_pRendererCom->Add_Text(TextDesc);
					}
					else
					{
						TextDesc.vPosition = _float2(vFontPos.x - 0.5f, vFontPos.y);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x + 0.5f, vFontPos.y);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y - 0.5f);
						m_pRendererCom->Add_Text(TextDesc);
						TextDesc.vPosition = _float2(vFontPos.x, vFontPos.y + 0.5f);
						m_pRendererCom->Add_Text(TextDesc);
					}

					TextDesc.vColor = m_vColor;
					TextDesc.vPosition = vFontPos;
					m_pRendererCom->Add_Text(TextDesc);
				}
			}
		}
	}
}

HRESULT CUI_World_NPCTag::Render()
{
//	if (m_bActive)
//	{
//		m_pShaderCom->Begin(m_iPass);
//
//		m_pVIBufferCom->Render();
//	}

	return S_OK;
}

HRESULT CUI_World_NPCTag::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_World_NPCTag::Ready_State()
{
	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 1.f, 1.f));

	return S_OK;
}

CUI_World_NPCTag* CUI_World_NPCTag::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_World_NPCTag* pInstance = new CUI_World_NPCTag(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_World_NPCTag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_World_NPCTag::Clone(void* pArg)
{
	CUI_World_NPCTag* pInstance = new CUI_World_NPCTag(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_World_NPCTag");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_World_NPCTag::Free()
{
	__super::Free();
}
