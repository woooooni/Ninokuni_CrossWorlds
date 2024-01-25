#include "stdafx.h"
#include "UIMinimap_Manager.h"
#include "GameInstance.h"
#include "Transform.h"

#include "UI_Manager.h"

IMPLEMENT_SINGLETON(CUIMinimap_Manager)

CUIMinimap_Manager::CUIMinimap_Manager()
{

}

HRESULT CUIMinimap_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	CComponent* pRenderer = nullptr;
	pRenderer = GI->Clone_Component(LEVELID::LEVEL_STATIC, TEXT("Prototype_Component_Renderer"));
	if (nullptr == pRenderer)
		return E_FAIL;

	m_pRendererCom = dynamic_cast<CRenderer*>(pRenderer);
	if (nullptr == m_pRendererCom)
		return E_FAIL;

	return S_OK;
}

void CUIMinimap_Manager::Tick(_float fTimeDelta)
{

}

_bool CUIMinimap_Manager::Is_InMinimap(CTransform* pTransform)
{
	if (nullptr == pTransform)
		return false;

	_float2 vScreenPos = Calculate_PositionForMinimap(pTransform);
	if (vScreenPos.x == 99999.f && vScreenPos.y == 99999.f)
		return false;

	return true;
}

_float2 CUIMinimap_Manager::Calculate_PositionForMinimap(CTransform* pTransform)
{
	if (nullptr == pTransform)
		return _float2(99999.f, 99999.f);

	_vector vPosition = pTransform->Get_State(CTransform::STATE_POSITION);

	_matrix matWorld = XMLoadFloat4x4(&pTransform->Get_WorldFloat4x4());
	_matrix matView = XMLoadFloat4x4(&m_pRendererCom->Get_MinimapView());
	_matrix matProj = XMLoadFloat4x4(&m_pRendererCom->Get_MinimapProj());

	_float4x4 matWindow;
	XMStoreFloat4x4(&matWindow, matWorld * matView * matProj);

	_float3 vWindowPos = *(_float3*)&matWindow.m[3][0];

	vWindowPos.x /= vWindowPos.z;
	vWindowPos.y /= vWindowPos.z;
	_float fScreenX = vWindowPos.x * g_iWinSizeX * 0.5f + (g_iWinSizeX * 0.5f);
	_float fScreenY = vWindowPos.y * -(g_iWinSizeY * 0.5f) + (g_iWinSizeY * 0.5f);

	if (0.f > fScreenX || _float(g_iWinSizeX) < fScreenX)
		return _float2(99999.f, 99999.f);

	if (0.f > fScreenY || _float(g_iWinSizeY) < fScreenY)
		return _float2(99999.f, 99999.f);

	// 만약에 스크린좌표에 모두 있다면 그려야할 대상이다.
	return _float2(fScreenX, fScreenY);
}

void CUIMinimap_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
