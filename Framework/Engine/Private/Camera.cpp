#include "..\Public\Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag, _uint iObjectID)
	: CGameObject(pDevice, pContext, strObjTag, iObjectID)
{
}

CCamera::CCamera(const CCamera & rhs)
	: CGameObject(rhs)
{

}

HRESULT CCamera::Initialize_Prototype()
{
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);
	
	if (nullptr == m_pTransformCom)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera::Initialize(void * pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	memcpy(&m_tProjDesc, pArg, sizeof(PROJ_DESC));

	return S_OK;
}

void CCamera::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

void CCamera::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;
}

HRESULT CCamera::Render()
{
	if (!m_bActive)
		return S_OK;

	return S_OK;
}

//void CCamera::Set_Transform_To_Pipeline()
//{
//	GI->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInverse());
//
//	GI->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_tProjDesc.tLerpFov.fCurValue, m_tProjDesc.fAspect, m_tProjDesc.fNear, m_tProjDesc.fFar));
//}

HRESULT CCamera::Ready_Components()
{
	return S_OK;
}

void CCamera::Free()
{
	__super::Free();
}
