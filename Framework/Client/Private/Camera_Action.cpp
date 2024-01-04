#include "stdafx.h"
#include "Camera_Action.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

CCamera_Action::CCamera_Action(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Action::CCamera_Action(const CCamera_Action& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Action::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Action::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Action::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (m_bAction)
	{
		switch (m_eCurActionType)
		{
		case CCamera_Action::DOOR:
		{
			CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

			/* Rotation */

			const _float fRotPerFrame = XMConvertToRadians(1.f) * fTimeDelta;

			m_pTransformCom->RevolutionRotation(pTargetTransform->Get_Position(), Vec3::Up, fRotPerFrame);
			
			
			Vec4 vPos = m_pTransformCom->Get_Position();
			vPos.y += 0.1f * fTimeDelta;
			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos.OneW());
			
		}
			break;
		case CCamera_Action::TALK:
		{

		}
			break;
		
		default:
			break;
		}
	}

}

void CCamera_Action::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Action::Render()
{
	return S_OK;
}

HRESULT CCamera_Action::Start_Action(const CAMERA_ACTION_TYPE& eType, CGameObject* pTarget, const _uint& iTag)
{
	switch (eType)
	{
	case CAMERA_ACTION_TYPE::DOOR:
	{
		if (FAILED(Start_Action_Door()))
			return E_FAIL;

		m_eCurActionType = CAMERA_ACTION_TYPE::DOOR;
	}
	break;
	case CAMERA_ACTION_TYPE::TALK:
	{
		if (nullptr == pTarget || FAILED(Start_Action_Talk(pTarget, iTag)))
			return E_FAIL;

		m_eCurActionType = CAMERA_ACTION_TYPE::TALK;
	}
	break;
	default:
		break;
	}

	m_bAction = true;

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Door()
{
	CGameObject* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

	if (nullptr == pPlayer)
		return E_FAIL;
	
	m_pTargetObj = m_pLookAtObj = pPlayer;
	
	CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

	/* Cam Positon */
	m_tTargetOffset.vCurVec = { 0.f, 1.f, 4.f, 1.f };

	const Vec4 vCamTargetPosition = pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec).ZeroW()
								+ (Vec4)pTargetTransform->Get_Position();

	/* Cam LookAt */
	m_tLookAtOffset.vCurVec = { 0.f, 1.f, 0.f, 1.f };

	const Vec4 vCamLookAt = pTargetTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec).ZeroW()
							+ (Vec4)pTargetTransform->Get_Position();

	/* Set */
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamTargetPosition);
	m_pTransformCom->LookAt(vCamLookAt);

	return S_OK;
}

HRESULT CCamera_Action::Start_Action_Talk(CGameObject* pTarget, const _uint& iTag)
{
	return S_OK;
}

HRESULT CCamera_Action::Ready_Components()
{
	return S_OK;
}

CCamera_Action* CCamera_Action::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Action* pInstance = new CCamera_Action(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Action");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Action::Clone(void* pArg)
{
	CCamera_Action* pInstance = new CCamera_Action(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Action");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Action::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
