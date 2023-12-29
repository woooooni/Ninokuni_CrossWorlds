#include "stdafx.h"
#include "UI.h"
#include "GameInstance.h"



CUI::CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_UI)
{
	ZeroMemory(&m_tInfo, sizeof(UI_INFO));
}

CUI::CUI(const CUI& rhs)
	: CGameObject(rhs)
	, m_tInfo(rhs.m_tInfo)
	, m_strText(rhs.m_strText)
{
}

void CUI::Set_ParentsAlpha(_float fAlpha)
{
	// 자식에게 부모의 알파값을 입힌다.
	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->Set_ChildAlpha(fAlpha);
	}
}

void CUI::Set_ChildActive(_bool bActive)
{
	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->Set_Active(bActive);
	}
}

void CUI::Set_ChildPosition(_float3 vPosition)
{
	CTransform* pChildTransform = nullptr;

	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
		{
			pChildTransform = pChildUI->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pChildTransform)
				continue;

			pChildTransform->Set_State(CTransform::STATE_POSITION, XMVectorSet(vPosition.x, vPosition.y, vPosition.z, 1.f));
		}
	}
}

void CUI::Set_ElementalTypeToChild(ELEMENTAL_TYPE eElementalType)
{
	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->Set_ElementalType(eElementalType);
	}
}

void CUI::Set_PlayerTypeToChild(CHARACTER_TYPE eCharacterType)
{
	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->Set_CharacterType(eCharacterType);
	}
}

_float2 CUI::Get_ProjectionPosition(CTransform* pTransfrom)
{
	if (nullptr == pTransfrom)
		return _float2(-1.f, -1.f);

//	_vector vPosition = pTransfrom->Get_State(CTransform::STATE_POSITION);
//
//	_matrix ViewMatrix = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
//	_matrix ProjMatrix = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
//
//	vPosition = XMVector3TransformCoord(vPosition, ViewMatrix);
//	vPosition = XMVector3TransformCoord(vPosition, ProjMatrix);
//
//	_float fScreenX = XMVectorGetX(vPosition) * (g_iWinSizeX * 0.5f) + (g_iWinSizeX * 0.5f);
//	_float fScreenY = -1.f * XMVectorGetY(vPosition) * (g_iWinSizeY * 0.5f) + (g_iWinSizeY* 0.5f);

	_vector vPosition = pTransfrom->Get_State(CTransform::STATE_POSITION);

	_float4x4 matWorld = pTransfrom->Get_WorldFloat4x4();
	_matrix matView = GI->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = GI->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	_float4x4 matWindow;
	XMStoreFloat4x4(&matWindow, XMLoadFloat4x4(&matWorld) * matView * matProj);

	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];
	// &matWindow.m[3][0] -> 포지션의 시작 주소를 얻고,
	// (_float3*) -> _float3 포인터로 캐스팅
	// * -> 그 값을 가져온다.

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;
	_float fScreenX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
	_float fScreenY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

	return _float2(fScreenX, fScreenY);
}

HRESULT CUI::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		m_tInfo = *((UI_INFO*)pArg);
	}
	else
	{
		//Arg를 넣어주지 않은 상황이라면 기본 세팅으로 생성한다.
		m_tInfo.fX = g_iWinSizeX * 0.5f;
		m_tInfo.fY = g_iWinSizeY * 0.5f;
		m_tInfo.fCX = _float(g_iWinSizeX);
		m_tInfo.fCY = _float(g_iWinSizeY);
	}

	// View행렬과 Proj행렬을 채워준다.
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI::Tick(_float fTimeDelta)
{
	POINT pt = GI->GetMousePos();
	RECT MouseRect;
	MouseRect.left = pt.x - 1.f;
	MouseRect.right = pt.x + 1.f;
	MouseRect.top = pt.y - 1.f;
	MouseRect.bottom = pt.y + 1.f;

	RECT UIRect;
	if (nullptr == m_tInfo.pParent) // 부모가 없는 경우
	{
		UIRect.left = m_tInfo.fX - m_tInfo.fCX / 2.f;
		UIRect.right = m_tInfo.fX + m_tInfo.fCX / 2.f;
		UIRect.top = m_tInfo.fY - m_tInfo.fCY / 2.f;
		UIRect.bottom = m_tInfo.fY + m_tInfo.fCY / 2.f;
	}
	else // 부모가 있는 경우
	{
		UIRect.left = m_tInfo.pParent->Get_UI_Info().fX + m_tInfo.fX - m_tInfo.fCX / 2.f;
		UIRect.right = m_tInfo.pParent->Get_UI_Info().fX + m_tInfo.fX + m_tInfo.fCX / 2.f;
		UIRect.top = m_tInfo.pParent->Get_UI_Info().fY + m_tInfo.fY - m_tInfo.fCY / 2.f;
		UIRect.bottom = m_tInfo.pParent->Get_UI_Info().fY + m_tInfo.fY + m_tInfo.fCY / 2.f;
	}

	RECT Result;
	if (IntersectRect(&Result, &MouseRect, &UIRect))
	{
		// 들어와 있다.
		switch(m_eMouseState)
		{
		case UI_MOUSESTATE::MOUSE_ENTER:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ON;
			On_Mouse(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_ON:
			On_Mouse(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_EXIT:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ENTER;
			On_MouseEnter(fTimeDelta);
			break;

		case UI_MOUSESTATE::MOUSE_DRAG_START:
			m_eMouseState = UI_MOUSESTATE::MOUSE_DRAG;
			On_MouseDrag(fTimeDelta);
			break;

		case UI_MOUSESTATE::MOUSE_DRAG:
			On_MouseDrag(fTimeDelta);
			break;

		case UI_MOUSESTATE::MOUSE_DRAG_END:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ON;
			On_Mouse(fTimeDelta);
			break;


		default:
			m_eMouseState = UI_MOUSESTATE::MOUSE_ENTER;
			On_MouseEnter(fTimeDelta);
			break;

		}
	}
	else
	{
		// 안들어와 있다.
		switch (m_eMouseState)
		{
		case UI_MOUSESTATE::MOUSE_ENTER:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseExit(fTimeDelta);
			break;
		case UI_MOUSESTATE::MOUSE_ON:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseExit(fTimeDelta);
			break;

		case UI_MOUSESTATE::MOUSE_EXIT:
			m_eMouseState = UI_MOUSESTATE::MOUSE_END;
			break;

		case UI_MOUSESTATE::MOUSE_DRAG_START:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseDragExit(fTimeDelta);
			break;

		case UI_MOUSESTATE::MOUSE_DRAG:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseDragExit(fTimeDelta);
			break;

		case UI_MOUSESTATE::MOUSE_DRAG_END:
			m_eMouseState = UI_MOUSESTATE::MOUSE_EXIT;
			On_MouseExit(fTimeDelta);
			break;

		default:
			break;
		}
	}

	if (nullptr != m_tInfo.pParent) // 부모가 있는 UI라면
	{
		//m_tInfo.fX = m_tInfo.pParent->Get_UI_Info().fX + m_tInfo.fX;
		//m_tInfo.fY = m_tInfo.pParent->Get_UI_Info().fY + m_tInfo.fY;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			XMVectorSet((m_tInfo.pParent->Get_UI_Info().fX + m_tInfo.fX) - (g_iWinSizeX * 0.5f),
				-(m_tInfo.pParent->Get_UI_Info().fY + m_tInfo.fY) + (g_iWinSizeY * 0.5f), 0.0f, 1.f));
	}

	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->Tick(fTimeDelta);
	}

}

void CUI::LateTick(_float fTimeDelta)
{
	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->LateTick(fTimeDelta);
	}
}

HRESULT CUI::Render()
{
	for (auto& pChildUI : m_pChild)
	{
		if (nullptr != pChildUI)
			pChildUI->Render();
	}

	return S_OK;
}

HRESULT CUI::Make_Child(_float fX, _float fY, _float fCX, _float fCY, const wstring& strPrototypeTag, const wstring& strObjectTag, void* pChildArg)
{
	UI_INFO	UIDesc;
	ZeroMemory(&UIDesc, sizeof(CUI::UI_INFO));

	UIDesc.fCX = fCX;
	UIDesc.fCY= fCY;
	UIDesc.fX = fX;
	UIDesc.fY = fY;

	UIDesc.pParent = this;
	if (nullptr != pChildArg)
		UIDesc.pDesc = pChildArg;

	CGameObject* pChild = nullptr;
	pChild = GI->Clone_GameObject(strPrototypeTag, LAYER_TYPE::LAYER_UI, &UIDesc);
	if (TEXT("") != strObjectTag)
		pChild->Set_ObjectTag(strObjectTag);

	m_pChild.push_back(dynamic_cast<CUI*>(pChild));

	return S_OK;
}

void CUI::Delete_AllChild()
{
	for (auto& pChildUI : m_pChild)
	{
		if (pChildUI != nullptr)
		{
			pChildUI->Set_Dead(true);
			Safe_Release(pChildUI);
		}
	}
	m_pChild.clear();
}

void CUI::On_Mouse(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::LBTN) || KEY_HOLD(KEY::RBTN))
	{
		if (GI->Get_DIMMoveState(DIMM_X) || GI->Get_DIMMoveState(DIMM_Y))
		{
			m_eMouseState = UI_MOUSESTATE::MOUSE_DRAG_START;
			return;
		}
	}
}

void CUI::Debug_Input(_float fTimeDelta)
{
	if (KEY_TAP(KEY::LEFT_ARROW))
		m_tInfo.fX -= 10.f;

	if(KEY_TAP(KEY::RIGHT_ARROW))
		m_tInfo.fX += 10.f;

	if (KEY_TAP(KEY::UP_ARROW))
		m_tInfo.fY += 10.f;

	if (KEY_TAP(KEY::DOWN_ARROW))
		m_tInfo.fY -= 10.f;

	if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET))
		m_tInfo.fCX -= 10.f;

	if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
		m_tInfo.fCX += 10.f;

	if(KEY_TAP(KEY::O))
		m_tInfo.fCY -= 10.f;

	if (KEY_TAP(KEY::P))
		m_tInfo.fCY += 10.f;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		XMVectorSet(m_tInfo.fX - g_iWinSizeX * 0.5f, -m_tInfo.fY + g_iWinSizeY * 0.5f, 1.f, 1.f));
	m_pTransformCom->Set_Scale(XMLoadFloat3(&_float3(m_tInfo.fCX, m_tInfo.fCY, 1.f)));
}

HRESULT CUI::Ready_Components()
{
	// Transform Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	// Renderer Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	// Shader Coomponent
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	// VIBuffer Component
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;

	return S_OK;
}

void CUI::Free()
{
	__super::Free();

	for (auto& pUI : m_pChild)
		Safe_Release(pUI);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}

//  m_pTransformCom->Set_State(CTransform::STATE_POSITION,
//	XMVectorSet(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f, 1.f));