#include "stdafx.h"
#include "UI_Minimap_Icon.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UIMinimap_Manager.h"
#include "Camera_Manager.h"
#include "Camera.h"

CUI_Minimap_Icon::CUI_Minimap_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext, L"UI_Minimap_Icon")
{
	m_eType = UI_MINIMAPICON::MINIMAPICON_END;
}

CUI_Minimap_Icon::CUI_Minimap_Icon(const CUI_Minimap_Icon& rhs)
	: CUI(rhs)
	, m_eType(rhs.m_eType)
{
}

void CUI_Minimap_Icon::Set_Owner(CGameObject* pOwner, _bool bIsCamera)
{
	if (nullptr == pOwner)
		return;

	OBJ_TYPE eObjType = OBJ_TYPE::OBJ_END;

	if (false == bIsCamera)
	{
		switch (pOwner->Get_ObjectType())
		{
		case OBJ_TYPE::OBJ_CHARACTER:
			eObjType = OBJ_TYPE::OBJ_CHARACTER;
			m_eType = ICON_PLAYER;
			break;

		case OBJ_TYPE::OBJ_MONSTER:
			if (pOwner->Get_ObjectTag() == TEXT("Quest_DestSpot"))
			{
				eObjType = OBJ_TYPE::OBJ_NPC;
				m_eType = ICON_NPC;
			}
			else
			{
				eObjType = OBJ_TYPE::OBJ_MONSTER;
				m_eType = ICON_MONSTER;
			}
			break;

		case OBJ_TYPE::OBJ_PORTAL:
			eObjType = OBJ_TYPE::OBJ_PORTAL;
			m_eType = ICON_PORTAL;
			break;

		case OBJ_TYPE::OBJ_NPC:
			eObjType = OBJ_TYPE::OBJ_NPC;
			m_eType = ICON_NPC;
			break;

		default:
			break;
		}
	}
	else
	{
		eObjType = OBJ_TYPE::OBJ_CAMERA;
		m_eType = ICON_CAMERA;
	}

	if (OBJ_TYPE::OBJ_END == eObjType)
		return;

	m_pOwner = pOwner;

	if (FAILED(Ready_State()))
		return;
}

//void CUI_Minimap_Icon::Set_Position(_float2 vPos)
//{
//	if (vPos.x == 99999.f && vPos.y == 99999.f)
//		return;
//
//	m_tInfo.fX = vPos.x;
//	m_tInfo.fY = vPos.y;
//}

HRESULT CUI_Minimap_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Minimap_Icon::Initialize(void* pArg)
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

void CUI_Minimap_Icon::Tick(_float fTimeDelta)
{
	if (m_bActive)
	{
		if (nullptr == m_pOwner || true == Is_Dead())
			return;

		if (true == m_pOwner->Is_Dead())
		{
			Safe_Release(m_pOwner);
			Set_Dead(true);
			return;
		}

		if (m_eType == MINIMAPICON_END)
			return;

		if (ICON_PLAYER == m_eType || ICON_CAMERA == m_eType)
		{
			Rotation_Icon(fTimeDelta);
		}

		__super::Tick(fTimeDelta);
	}
}

void CUI_Minimap_Icon::LateTick(_float fTimeDelta)
{
	if (LEVEL_TOOL == GI->Get_CurrentLevel())
		return;

	if (m_bActive)
	{
		if (nullptr == m_pOwner || true == Is_Dead())
			return;

		if (true == m_pOwner->Is_Dead())
		{
			Safe_Release(m_pOwner);
			Set_Dead(true);
			return;
		}

		if (m_eType == MINIMAPICON_END)
			return;

		_float2 vWindowPos = CUIMinimap_Manager::GetInstance()->Calculate_PositionForMinimap(m_pOwner->Get_Component<CTransform>(L"Com_Transform"));

		if (vWindowPos.x == 99999.f && vWindowPos.y == 99999.f)
		{
			Set_Active(false);
			return;
		}

		if (ICON_CAMERA == m_eType)
		{
			m_tInfo.fX = vWindowPos.x;
			m_tInfo.fY = vWindowPos.y; //-100.f;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), m_fZ, 1.f));
		}
		else
		{
			m_tInfo.fX = vWindowPos.x;
			m_tInfo.fY = vWindowPos.y;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), m_fZ, 1.f));
			// Z값이 필요가없음. 수정하자
		}

		// Minimap이 켜져있으면 렌더그룹에 추가한다.
		if (true == CUI_Manager::GetInstance()->Is_MinimapOn())
		{
			m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI_MINIMAP_ICON, this);
		}

		__super::LateTick(fTimeDelta);
	}
}

HRESULT CUI_Minimap_Icon::Render()
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

HRESULT CUI_Minimap_Icon::Render_Minimap()
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

HRESULT CUI_Minimap_Icon::Ready_Components()
{
	
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_Icons"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CUI_Minimap_Icon::Ready_State()
{
	// 만약 아이콘 별 사이즈 조정이 필요하다면 여기서 Set
	
	//m_tInfo.fCX = 64.f * 0.4f;
	switch (m_eType)
	{
	case UI_MINIMAPICON::ICON_PLAYER:
		m_tInfo.fCX = 64.f * 1.5f;
		m_tInfo.fCY = m_tInfo.fCX;
		m_fZ = 0.f;
		break;

	case UI_MINIMAPICON::ICON_MONSTER:
		m_tInfo.fCX = 64.f * 1.5f;
		m_tInfo.fCY = m_tInfo.fCX;
		m_fZ = 0.5f;
		break;

	case UI_MINIMAPICON::ICON_PORTAL:
		m_tInfo.fCX = 64.f * 1.5f;
		m_tInfo.fCY = m_tInfo.fCX;
		m_fZ = 0.5f;
		break;

	case UI_MINIMAPICON::ICON_NPC: // 정확히는 Quest DestSpot
		m_tInfo.fCX = 64.f * 1.5f;
		m_tInfo.fCY = m_tInfo.fCX;
		m_fZ = 0.5f;
		break;

	case UI_MINIMAPICON::ICON_CAMERA:
		m_fAlpha = 0.5f;
		m_tInfo.fCX = 128.f * 2.f;
		m_tInfo.fCY = 256.f * 2.f;
		m_fZ = 1.f;
		m_iPass = 21;
		break;

	default:
		m_tInfo.fCX = 64.f;
		m_tInfo.fCY = m_tInfo.fCX;
		break;
	}

	m_pTransformCom->Set_Scale(XMVectorSet(m_tInfo.fCX, m_tInfo.fCY, 1.f, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -(m_tInfo.fY - g_iWinSizeY * 0.5f), m_fZ, 1.f));

	return S_OK;
}

HRESULT CUI_Minimap_Icon::Bind_ShaderResources()
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

void CUI_Minimap_Icon::Rotation_Icon(_float fTimeDelta)
{
	_vector vStandard = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	_vector vLook;

	if (ICON_PLAYER == m_eType)
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

CUI_Minimap_Icon* CUI_Minimap_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Minimap_Icon* pInstance = new CUI_Minimap_Icon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Create : CUI_Minimap_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Minimap_Icon::Clone(void* pArg)
{
	CUI_Minimap_Icon* pInstance = new CUI_Minimap_Icon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Clone : CUI_Minimap_Icon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Minimap_Icon::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
}
