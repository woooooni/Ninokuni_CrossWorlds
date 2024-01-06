#include "stdafx.h"
#include "Camera_CutScene_Boss.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

CCamera_CutScene_Boss::CCamera_CutScene_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_CutScene_Boss::CCamera_CutScene_Boss(const CCamera_CutScene_Boss& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_CutScene_Boss::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_CutScene_Boss::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_CutScene_Boss::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	/* Check Blending */
	if (m_bBlending)
	{
		Tick_Blending(fTimeDelta);
		return;
	}
}

void CCamera_CutScene_Boss::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_CutScene_Boss::Render()
{
	return S_OK;
}

HRESULT CCamera_CutScene_Boss::Start_CutScene(const _uint& iBossType, const _uint& iProgressType)
{
	return E_NOTIMPL;
}

Vec4 CCamera_CutScene_Boss::Get_LookAt()
{
	return Vec4();
}

HRESULT CCamera_CutScene_Boss::Ready_Components()
{
	return S_OK;
}

void CCamera_CutScene_Boss::Tick_Blending(const _float fDeltaTime)
{
}

CCamera_CutScene_Boss* CCamera_CutScene_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_CutScene_Boss* pInstance = new CCamera_CutScene_Boss(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_CutScene_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_CutScene_Boss::Clone(void* pArg)
{
	CCamera_CutScene_Boss* pInstance = new CCamera_CutScene_Boss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_CutScene_Boss");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_CutScene_Boss::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
