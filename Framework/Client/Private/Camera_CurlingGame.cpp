#include "stdafx.h"
#include "Camera_CurlingGame.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

CCamera_CurlingGame::CCamera_CurlingGame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_CurlingGame::CCamera_CurlingGame(const CCamera_CurlingGame& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_CurlingGame::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_CurlingGame::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	/* Set Camera */
	{
		Set_TargetOffSet(Cam_TargetOffset_CurlingGame_Default);

		Set_LookAtOffSet(Cam_LookAtOffset_CurlingGame_Default);
	
		Set_Fov(Cam_Fov_CurlingGame);
	}

	return S_OK;
}

void CCamera_CurlingGame::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (m_bBlending)
	{
		Tick_Blending(fTimeDelta);
		return;
	}

	if (m_tTargetDesc.bChangingTarget)
	{
		Tick_ChangeTarget(fTimeDelta);
	}

	if (!m_tTargetDesc.bChangingTarget && nullptr != m_pTargetObj)
	{
		const Vec4 vCamPos = Calculate_Position(m_pTargetObj->Get_Component_Transform()->Get_Position());

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos);
	}
}

void CCamera_CurlingGame::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

void CCamera_CurlingGame::Tick_Blending(const _float fDeltaTime)
{
	const Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	const Vec4 vLook = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition);

	m_pTransformCom->LookAt(vLook);
}

void CCamera_CurlingGame::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);
}

HRESULT CCamera_CurlingGame::Change_Target(CGameObject* pTarget, const _float& fLerpTime)
{
	/* Check Exception */
	{
		if (nullptr == pTarget)
			return E_FAIL;

		if (nullptr != m_pTargetObj && pTarget == m_pTargetObj)
			return S_OK;
	}

	m_tTargetDesc.bChangingTarget = true;

	Set_TargetObj(pTarget);

	m_tTargetDesc.tLerpPos.Start(m_pTransformCom->Get_Position(), Calculate_Position(pTarget->Get_Component_Transform()->Get_Position()), fLerpTime, LERP_MODE::SMOOTHER_STEP);
	
	return S_OK;
}

Vec4 CCamera_CurlingGame::Calculate_Position(const Vec4 vTargetPosition)
{
	const Vec4 vCamPos = Vec4(vTargetPosition + m_pTransformCom->Get_RelativeOffset(m_tTargetOffset.vCurVec)).OneW();
	
	if (m_tDampingDesc.bActive)
	{

	}
	else
	{

	}

	return vCamPos;
}

Vec4 CCamera_CurlingGame::Calculate_LookAt(const Vec4 vTargetPostion)
{
	m_vPrevLookAt = Vec4(vTargetPostion + m_tLookAtOffset.vCurVec).OneW();

	return m_vPrevLookAt;
}

HRESULT CCamera_CurlingGame::Start_Damping()
{
	m_tDampingDesc.Ready(m_pTransformCom->Get_Position());

	Start_Lerp_Fov(m_tDampingDesc.fDampingFov, m_tDampingDesc.fFovLerpTime);

	return S_OK;
}

HRESULT CCamera_CurlingGame::Finish_Damping()
{
	m_tDampingDesc.bActive = false;

	Start_Lerp_Fov(Cam_Fov_CurlingGame, m_tDampingDesc.fFovLerpTime);

	return S_OK;
}

HRESULT CCamera_CurlingGame::Ready_Components()
{
	return S_OK;
}

void CCamera_CurlingGame::Tick_ChangeTarget(const _float& fTimeDelta)
{
	m_tTargetDesc.tLerpPos.Update_Lerp(fTimeDelta);
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tTargetDesc.tLerpPos.vCurVec.OneW());

	//m_pTransformCom->LookAt(m_tLerpLookAt.vCurVec.OneW());
	
	if (!m_tTargetDesc.tLerpPos.bActive)
		m_tTargetDesc.bChangingTarget = false;
}

CCamera_CurlingGame* CCamera_CurlingGame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_CurlingGame* pInstance = new CCamera_CurlingGame(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_CurlingGame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_CurlingGame::Clone(void* pArg)
{
	CCamera_CurlingGame* pInstance = new CCamera_CurlingGame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_CurlingGame");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_CurlingGame::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
