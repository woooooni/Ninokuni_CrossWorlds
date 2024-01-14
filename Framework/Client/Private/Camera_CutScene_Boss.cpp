#include "stdafx.h"
#include "Camera_CutScene_Boss.h"
#include "GameInstance.h"

#include "Camera_Manager.h"
#include "Camera_Follow.h"
#include "Camera_CutScene_Map.h"

#include "UI_Manager.h"

#include "Glanix.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"

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

	switch (m_iCurBossType)
	{
	case BOSS_TYPE::GLANIX :
		Tick_CutScene_Granix(fTimeDelta);
	break;
	case BOSS_TYPE::WITCH:
	{

	}
	break;
	case BOSS_TYPE::STELLIA:
	{

	}
	break;
	default:
		break;
	}
}

void CCamera_CutScene_Boss::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_CutScene_Boss::Ready_Components()
{
	return S_OK;
}

HRESULT CCamera_CutScene_Boss::Start_CutScene_Granix(const _uint& iCutSceneType, CGameObject* pBoss)
{
	m_pTargetObj = m_pLookAtObj = pBoss;
	m_iCurCutSceneType = iCutSceneType;

	switch (m_iCurCutSceneType)
	{
	case GLANIX_CUTSCENE_TYPE::APPEAR :
	{
		m_iCurProgressType = GLANIX_CUTSCENE_APPEAR_PROGRESS::IDEL;
		
		memcpy(&m_tTargetOffset.vCurVec, &m_tGlanixCutSceneAppearDesc.Offsets[m_iCurProgressType].first, sizeof(Vec4));
		memcpy(&m_tLookAtOffset.vCurVec, &m_tGlanixCutSceneAppearDesc.Offsets[m_iCurProgressType].second, sizeof(Vec4));

		CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

		/* Cam Position */
		{
			Vec4 vCamPos = (Vec4)pTargetTransform->Get_Position() +
							pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos.OneW());
		}

		/* Look At Position */
		{
			Vec4 vLookPos = Vec4::UnitW;

			Matrix matLookWorld = m_pLookAtObj->Get_Component<CModel>(L"Com_Model")->Get_SocketLocalMatrix(0)
				* pTargetTransform->Get_WorldMatrix();

			memcpy(&vLookPos, &matLookWorld.m[3], sizeof(Vec4));

			vLookPos += pTargetTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec);

			m_pTransformCom->LookAt(vLookPos.OneW());
		}

		/* Player Input Off */
		{
			CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
		}
	}
	break;
	case GLANIX_CUTSCENE_TYPE::PAGE:
	{
		
	}
	break;
	case GLANIX_CUTSCENE_TYPE::DEAD:
	{
		
	}
	break;
	default:
		break;
	}
	
	return S_OK;
}

void CCamera_CutScene_Boss::Tick_CutScene_Granix(const _float fDeltaTime)
{
	switch (m_iCurCutSceneType)
	{
	case GLANIX_CUTSCENE_TYPE::APPEAR:
	{
		CTransform* pTargetTransform = m_pTargetObj->Get_Component<CTransform>(L"Com_Transform");

		if (m_bSignal)
		{
			m_bSignal = false;

			if(m_iCurProgressType < GLANIX_CUTSCENE_APPEAR_PROGRESS::GLANIX_CUTSCENE_APPEAR_PROGRESSEND - 1)
				m_iCurProgressType++;

			memcpy(&m_tTargetOffset.vCurVec, &m_tGlanixCutSceneAppearDesc.Offsets[m_iCurProgressType].first, sizeof(Vec4));
			memcpy(&m_tLookAtOffset.vCurVec, &m_tGlanixCutSceneAppearDesc.Offsets[m_iCurProgressType].second, sizeof(Vec4));

			/* Cam Position */
			{
				if (GLANIX_CUTSCENE_APPEAR_PROGRESS::IDEL == m_iCurProgressType)
					m_tTargetOffset.vCurVec.z += fDeltaTime; /* 줌인 */

				Vec4 vCamPos = (Vec4)pTargetTransform->Get_Position() +
								pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec);

				m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos.OneW());
			}
		}

		/* Look At Position */
		Vec4 vLookPos = Vec4::UnitW;
		{
			Matrix matLookWorld = m_pLookAtObj->Get_Component<CModel>(L"Com_Model")->Get_SocketLocalMatrix(0)
				* pTargetTransform->Get_WorldMatrix();

			memcpy(&vLookPos, &matLookWorld.m[3], sizeof(Vec4));

			vLookPos += pTargetTransform->Get_RelativeOffset(m_tLookAtOffset.vCurVec);

			if (Is_Shake())
				vLookPos += Vec4(Get_ShakeLocalPos());

			m_pTransformCom->LookAt(vLookPos.OneW());
		}

		/* 아이들 상태일 때 줌인 효과 구현 */
		if (GLANIX_CUTSCENE_APPEAR_PROGRESS::IDEL == m_iCurProgressType)
		{
			m_tTargetOffset.vCurVec.z += fDeltaTime; 

			Vec4 vCamPos =
				(Vec4)pTargetTransform->Get_Position() +
				pTargetTransform->Get_RelativeOffset(m_tTargetOffset.vCurVec);

			m_pTransformCom->Set_State(CTransform::STATE_POSITION, vCamPos.OneW());
		}
	}
	break;
	case GLANIX_CUTSCENE_TYPE::PAGE:
	{

	}
	break;
	case GLANIX_CUTSCENE_TYPE::DEAD:
	{

	}
	break;
	default:
		break;
	}
}

void CCamera_CutScene_Boss::Clear_Data()
{
	m_iCurBossType		= -1;
	m_iCurCutSceneType	= -1;
	m_iCurProgressType	= -1;

	m_pTargetObj = nullptr;
	m_pLookAtObj = nullptr;
}

HRESULT CCamera_CutScene_Boss::Start_CutScene(const _uint& iBossType, const _uint& iCutSceneType, CGameObject* pBoss)
{
	if (nullptr == pBoss)
		return E_FAIL;

	m_iCurBossType = iBossType;

	switch (m_iCurBossType)
	{
	case BOSS_TYPE::GLANIX :
	{
		if (FAILED(Start_CutScene_Granix(iCutSceneType, pBoss)))
			return E_FAIL;

	}
	break;
	case BOSS_TYPE::WITCH:
	{

	}
	break;
	case BOSS_TYPE::STELLIA:
	{

	}
	break;
	default:
		break;
	}
	return S_OK;
}

HRESULT CCamera_CutScene_Boss::Finish_CutScene()
{
	switch (m_iCurBossType)
	{
	case BOSS_TYPE::GLANIX:
	{
		switch (m_iCurCutSceneType)
		{
		case GLANIX_CUTSCENE_TYPE::APPEAR :
		{
			CCamera_Follow* pFollowCam = dynamic_cast<CCamera_Follow*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::FOLLOW));
			if (nullptr != pFollowCam)
			{
				pFollowCam->Set_Default_Position();
				CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);

				CGameObject* pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), LAYER_MONSTER, L"Glanix");
				if (nullptr != pTarget)
					pFollowCam->Start_LockOn(pTarget, Cam_Target_Offset_LockOn_Glanix, Cam_LookAt_Offset_LockOn_Glanix);

				/* Player Input On */
				CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
			}
		}
		break;
		case GLANIX_CUTSCENE_TYPE::PAGE :
		{

		}
		break;
		case GLANIX_CUTSCENE_TYPE::DEAD :
		{

		}
		break;
		default:
			break;
		}
	}
	break;
	case BOSS_TYPE::WITCH:
	{

	}
	break;
	case BOSS_TYPE::STELLIA:
	{

	}
	break;
	default:
		break;
	}

	Clear_Data();

	return S_OK;
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
