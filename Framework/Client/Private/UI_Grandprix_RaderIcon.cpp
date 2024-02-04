#include "stdafx.h"
#include "UI_Grandprix_RaderIcon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UIMinigame_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"

CUI_Grandprix_RaderIcon::CUI_Grandprix_RaderIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Grandprix_RaderIcon")
{
	m_eType = UI_RADERICON::RADERICON_END;
}

CUI_Grandprix_RaderIcon::CUI_Grandprix_RaderIcon(const CUI_Grandprix_RaderIcon& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Grandprix_RaderIcon::Set_Owner(CGameObject* pOwner, _bool bIsCamera)
{
	if (nullptr == pOwner)
		return;

	if (false == bIsCamera)
	{
		if (pOwner->Get_ObjectTag() == TEXT("Vehicle_Flying_PlayerBiplane"))
		{
			m_eType = ICON_SWORDMAN;
		}
		else if (pOwner->Get_ObjectTag() == TEXT("Vehicle_Flying_EnemyBiplane"))
		{
			m_eType = ICON_ENGINEER;
		}
		else if (pOwner->Get_ObjectTag() == TEXT("Vehicle_Flying_EnemyBoto"))
		{
			m_eType = ICON_GHOST;
		}
		else
			return;
	}
	else
	{
		m_eType = UI_RADERICON::ICON_CAMERA;
	}

	m_pOwner = pOwner;

	if (FAILED(Ready_State()))
		return;
}

HRESULT CUI_Grandprix_RaderIcon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Grandprix_RaderIcon::Initialize(void* pArg)
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

void CUI_Grandprix_RaderIcon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pOwner || true == m_pOwner->Is_Dead() || true == Is_Dead() || m_eType == RADERICON_END)
			return;

		if (UI_RADERICON::ICON_SWORDMAN == m_eType || UI_RADERICON::ICON_CAMERA == m_eType)
			Rotation_Icon(fTimeDelta);

		__super::Tick(fTimeDelta);
	}
}

void CUI_Grandprix_RaderIcon::LateTick(_float fTimeDelta)
{
	if (LEVEL_TOOL == GI->Get_CurrentLevel())
		return;

	if (m_bActive)
	{
		if (nullptr == m_pOwner || true == m_pOwner->Is_Dead() || true == Is_Dead() || m_eType == RADERICON_END)
			return;

		_float2 vWindowPos = CUI_Manager::GetInstance()->Calculate_PositionForMinimap(m_pOwner->Get_Component<CTransform>(L"Com_Transform"));

		if (vWindowPos.x == 99999.f && vWindowPos.y == 99999.f)
		{
			Set_Active(false);
			return;
		}

		m_tInfo.fX = vWindowPos.x;
		m_tInfo.fY = vWindowPos.y;
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

		// Rader가 켜져있으면 렌더그룹에 추가한다.
		if (true == CUIMinigame_Manager::GetInstance()->Is_BiplaneFlying())
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_MINIMAP_ICON, this);
		}
	}
}

HRESULT CUI_Grandprix_RaderIcon::Render()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(20);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Grandprix_RaderIcon::Render_Minimap()
{
	if (m_bActive)
	{
		if (FAILED(Bind_ShaderResources()))
			return E_FAIL;

		m_pShaderCom->Begin(20);

		m_pVIBufferCom->Render();
	}

	return S_OK;
}

HRESULT CUI_Grandprix_RaderIcon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_EVERMORE, TEXT("Prototype_Component_Texture_UI_Grandprix_Rader_Icons"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Grandprix_RaderIcon::Ready_State()
{
	switch (m_eType)
	{
	case UI_RADERICON::ICON_SWORDMAN:
		m_tInfo.fCX = 64.f * 1.7f;
		m_tInfo.fCY = m_tInfo.fCX;
		break;

	case UI_RADERICON::ICON_ENGINEER:
		m_tInfo.fCX = 64.f * 1.7f;
		m_tInfo.fCY = m_tInfo.fCX;
		break;

	case UI_RADERICON::ICON_GHOST:
		m_tInfo.fCX = 64.f * 1.5f;
		m_tInfo.fCY = m_tInfo.fCX;
		break;

	case UI_RADERICON::ICON_CAMERA:
		m_tInfo.fCX = 128.f * 6.f;
		m_tInfo.fCY = 256.f * 6.f;
		m_fAlpha = 0.75f;
		break;

	default:
		m_tInfo.fCX = 64.f; 
		m_tInfo.fCY = m_tInfo.fCX;
		break;
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), 0.f, 1.f));

	return S_OK;
}

HRESULT CUI_Grandprix_RaderIcon::Bind_ShaderResources()
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

void CUI_Grandprix_RaderIcon::Rotation_Icon(_float fTimeDelta)
{
	_vector vStandard = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	_vector vLook;

	if (UI_RADERICON::ICON_SWORDMAN == m_eType)
	{
		CTransform* pPlayerTransform = m_pOwner->Get_Component<CTransform>(L"Com_Transform");
		if (nullptr == pPlayerTransform)
			return;

		vLook = pPlayerTransform->Get_Look();
		vLook = XMVector3Normalize(vLook);
	}
	else
	{
		CCamera* pCurCamera = CCamera_Manager::GetInstance()->Get_CurCamera();
		if (nullptr == pCurCamera)
			return;
		CTransform* pCameraTrans = pCurCamera->Get_Transform();
		if (nullptr == pCameraTrans)
			return;

		vLook = pCameraTrans->Get_Look();
		vLook = XMVector3Normalize(vLook);
	}

	// 두 벡터 사이의 각도 계산
	_float fDot = XMVectorGetX(XMVector3Dot(vStandard, vLook));
	_float fDegree = XMConvertToDegrees(acos(fDot));

	// 두 벡터의 외적을 통해 회전 방향 결정
	if (XMVectorGetY(XMVector3Cross(vStandard, vLook)) > 0.f)
		fDegree *= -1.f;

	m_pTransformCom->Rotation(XMVectorSet(0.f, 0.f, 1.f, 0.f), XMConvertToRadians(fDegree));
}

CUI_Grandprix_RaderIcon* CUI_Grandprix_RaderIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Grandprix_RaderIcon* pInstance = new CUI_Grandprix_RaderIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Grandprix_RaderIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Grandprix_RaderIcon::Clone(void* pArg)
{
	CUI_Grandprix_RaderIcon* pInstance = new CUI_Grandprix_RaderIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Grandprix_RaderIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Grandprix_RaderIcon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
