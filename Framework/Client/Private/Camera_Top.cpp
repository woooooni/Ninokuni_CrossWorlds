#include "stdafx.h"
#include "Camera_Top.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

#include "Game_Manager.h"
#include "Player.h"

CCamera_Top::CCamera_Top(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_Top::CCamera_Top(const CCamera_Top& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_Top::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Top::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Top::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	if (m_bBlending)
	{
		Tick_Blending(fTimeDelta);
		return;
	}

	switch(m_eCurType)
	{
	case VIEW_TYPE::GLANIX_PILLAR_PATTERN:
	{
		Tick_Glanix_PillarPattern(fTimeDelta);
	}
	break;
	default:
		break;
	}

}

void CCamera_Top::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_Top::Start_TopView(const VIEW_TYPE& eType)
{
	switch (eType)
	{
	case VIEW_TYPE::GLANIX_PILLAR_PATTERN :
	{
		CGameObject* pPlayer = CGame_Manager::GetInstance()->Get_Player()->Get_Character();

		if (nullptr == pPlayer)
			return E_FAIL;

		Set_TargetObj(pPlayer);
		Set_LookAtObj(pPlayer);

		m_tTargetOffset.vCurVec = Cam_TargetOffset_Top_Glanix;
		m_tLookAtOffset.vCurVec = Cam_LookAtOffset_Top_Glanix;

		/* Cam Position*/
		m_vCurPos = m_tTargetOffset.vCurVec +
			Vec4(m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).ZeroW();

		/* Cam Look */
		m_vPrevLookAt = m_tLookAtOffset.vCurVec +
			Vec4(m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).ZeroW();

		/* Set Transform */
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vCurPos);
		m_pTransformCom->LookAt(m_vPrevLookAt);

		/* Etc */
		m_fPosOriginHeight = m_vCurPos.y;
		m_fLookOriginHeight = m_vPrevLookAt.y;
	}
	break;
	default:
		break;
	}

	m_eCurType = eType;

	return S_OK;
}

Vec4 CCamera_Top::Get_LookAt()
{
	switch (m_eCurType)
	{
	case VIEW_TYPE::GLANIX_PILLAR_PATTERN:
	{
		return m_vPrevLookAt;
	}
	break;

	default:
		break;
	}


	return Vec4::UnitW;
}

void CCamera_Top::Tick_Blending(const _float fDeltaTime)
{
	const Vec4 vCamPosition = CCamera_Manager::GetInstance()->Get_BlendingPosition();

	const Vec4 vLook = CCamera_Manager::GetInstance()->Get_BlendingLookAt();

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, vCamPosition);

	m_pTransformCom->LookAt(vLook);
}

void CCamera_Top::Set_Blending(const _bool& bBlending)
{
	__super::Set_Blending(bBlending);

	if (!bBlending && CAMERA_TYPE::FOLLOW == CCamera_Manager::GetInstance()->Get_PrevCamera()->Get_Key()) /* 팔로우 -> 탑 */
	{
		/* 무브 인풋만 열어준다.*/
		CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_Move_Input(true);
	}
}

void CCamera_Top::Lerp_TargetOffset(const Vec4& vStartValue, const Vec4& vTargetValue, const _float& fTime, const LERP_MODE& eMode)
{
	__super::Lerp_TargetOffset(vStartValue, vTargetValue, fTime, eMode);

	if (!m_bInitLerp)
		m_bInitLerp = true;
	else if (m_bInitLerp)
		m_tLerpHeight.Start(m_fPosOriginHeight, m_fPosOriginHeight - 7.f, fTime, eMode);
}

HRESULT CCamera_Top::Ready_Components()
{
	return S_OK;
}

void CCamera_Top::Tick_Glanix_PillarPattern(_float fTimeDelta)
{
	/* Cam Position (Damping) */
	{
		Vec4 vGoalPos = m_tTargetOffset.vCurVec +
							Vec4(m_pTargetObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).ZeroW();

		if (m_tLerpHeight.bActive)
		{
			m_tLerpHeight.Update(fTimeDelta);

			m_fPosOriginHeight = m_tLerpHeight.fCurValue;
		}

		vGoalPos.y = m_fPosOriginHeight;

		const Vec4 vDist = (vGoalPos.ZeroW() - m_vCurPos.ZeroW()) * m_fDampingCoefficient;

		m_vCurPos += vDist;
		m_vCurPos.y = m_fPosOriginHeight;
	
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_vCurPos);
	}

	/* Cam Look */
	{
		m_vPrevLookAt = m_tLookAtOffset.vCurVec +
							Vec4(m_pLookAtObj->Get_Component<CTransform>(L"Com_Transform")->Get_Position()).ZeroW();

		if (Is_Shake())
			m_vPrevLookAt += Vec4(Get_ShakeLocalPos()).ZeroW();

		m_vPrevLookAt.y = m_fLookOriginHeight;

		m_pTransformCom->LookAt(m_vPrevLookAt);
	}
}

CCamera_Top* CCamera_Top::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_Top* pInstance = new CCamera_Top(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_Top");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_Top::Clone(void* pArg)
{
	CCamera_Top* pInstance = new CCamera_Top(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_Top");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Top::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
