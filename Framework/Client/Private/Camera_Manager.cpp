#include "stdafx.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

#include "Camera_Free.h"
#include "Camera_Follow.h"
#include "Camera_CutScene.h"


IMPLEMENT_SINGLETON(CCamera_Manager)

CCamera_Manager::CCamera_Manager()
{

}

HRESULT CCamera_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (nullptr == pDevice || nullptr == pContext)
		return E_FAIL;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	if (FAILED(Ready_Cameras()))
		return E_FAIL;

	return S_OK;
}

void CCamera_Manager::Tick(_float fTimeDelta)
{
	if (nullptr == m_pCurCamera)
		return;

	m_pCurCamera->Tick(fTimeDelta);
}

void CCamera_Manager::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pCurCamera)
		return;

	m_pCurCamera->LateTick(fTimeDelta);

	/* 아직 보간 처리 X 기본 싱글 카메라만 파이프라인에 전달 */

	/* V(WI) */
	{
		GI->Set_Transform(CPipeLine::D3DTS_VIEW, m_pCurCamera->Get_Transform()->Get_WorldMatrixInverse());
	}

	/* P */
	{
		const CCamera::PROJ_DESC& tDesc = m_pCurCamera->Get_ProjDesc();
		GI->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(tDesc.tLerpFov.fCurValue, tDesc.fAspect, tDesc.fNear, tDesc.fFar));
	}
}

CCamera* CCamera_Manager::Get_Camera(const CAMERA_TYPE& eType)
{
	if (CAMERA_TYPE::CAMERA_TYPE_END <= eType || 0 > CAMERA_TYPE::FREE)
		return nullptr;

	return m_Cameras[eType];
}

HRESULT CCamera_Manager::Set_CurCamera(const CAMERA_TYPE& eType)
{
	if (CAMERA_TYPE::CAMERA_TYPE_END <= eType || 0 > CAMERA_TYPE::FREE)
		return E_FAIL;

	if (nullptr == m_Cameras[eType])
		return E_FAIL;

	if (nullptr == m_pCurCamera) /* 최초 카메라 Set */
	{
		m_pCurCamera = m_Cameras[eType];
		m_pCurCamera->Set_Active(true);
	}
	else
	{
		m_pCurCamera->Set_Active(false);

		m_pCurCamera = m_Cameras[eType];

		m_pCurCamera->Set_Active(true);
	}

	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_Shake(const CAMERA_EVENT_DESC& tDesc)
{
	if (nullptr == m_pCurCamera)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_Lerp_Fov(const CAMERA_EVENT_DESC& tDesc)
{
	if (nullptr == m_pCurCamera)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_Lerp_Dist(const CAMERA_EVENT_DESC& tDesc)
{
	if (nullptr == m_pCurCamera)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_Shake_Default()
{
	if (nullptr == m_pCurCamera)
		return E_FAIL;

	m_pCurCamera->Start_Shake(0.1f, 17.f, 0.3f);
	
	return S_OK;
}

HRESULT CCamera_Manager::Start_Action_CutScene(const string& strCutSceneName)
{
	return S_OK;
}

string CCamera_Manager::Get_Action_CutSceneEvent_Name(const _int& iIndex)
{
	return Find_Action_CutSceneEvent_Name(iIndex);
}

vector<CAMERA_CUT_SCENE_EVENT_DESC> CCamera_Manager::Get_Action_CutSceneEvent(const string& strTag)
{
	return Find_Action_CutSceneEvent(strTag);
}

void CCamera_Manager::Add_Action_CutSceneEvent(const string& strTag)
{
	m_CutSceneEvents.emplace(strTag, vector<CAMERA_CUT_SCENE_EVENT_DESC>());
}

string CCamera_Manager::Find_Action_CutSceneEvent_Name(const _int& iIndex)
{
	_int iCount = 0;

	for (auto& iter : m_CutSceneEvents)
	{
		if (iCount == iIndex)
			return iter.first;
	}

	return string();
}

vector<CAMERA_CUT_SCENE_EVENT_DESC> CCamera_Manager::Find_Action_CutSceneEvent(const string& strTag)
{
	auto iterEvent = m_CutSceneEvents.find(strTag);

	if(iterEvent == m_CutSceneEvents.end())
		return vector<CAMERA_CUT_SCENE_EVENT_DESC>();

	return iterEvent->second;
}

HRESULT CCamera_Manager::Ready_Cameras()
{
	/* 카메라는 오브젝트 매니저에 추가 되지 않고 카메라 매니저가 플로우를 돌린다. */

	CAMERA_TYPE eType = CAMERA_TYPE::CAMERA_TYPE_END;

	/* Free */
	eType = CAMERA_TYPE::FREE;
	{
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Free_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			m_Cameras[eType] = CCamera_Free::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == m_Cameras[eType])
				return E_FAIL;

			if (FAILED(m_Cameras[eType]->Initialize(&tDesc)))
			{
				Safe_Release(m_Cameras[eType]);
				return E_FAIL;
			}
		}
		m_Cameras[eType]->Set_Type(eType);

		m_Cameras[eType]->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		m_Cameras[eType]->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* Follow */
	eType = CAMERA_TYPE::FOLLOW;
	{
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Follow_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			m_Cameras[eType] = CCamera_Follow::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == m_Cameras[eType])
				return E_FAIL;

			if (FAILED(m_Cameras[eType]->Initialize(&tDesc)))
			{
				Safe_Release(m_Cameras[eType]);
				return E_FAIL;
			}
		}
		m_Cameras[eType]->Set_Type(eType);

		m_Cameras[eType]->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		m_Cameras[eType]->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* CutScene */
	eType = CAMERA_TYPE::CUTSCENE;
	{
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_CurScene_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			m_Cameras[eType] = CCamera_Follow::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == m_Cameras[eType])
				return E_FAIL;

			if (FAILED(m_Cameras[eType]->Initialize(&tDesc)))
			{
				Safe_Release(m_Cameras[eType]);
				return E_FAIL;
			}
		}
		m_Cameras[eType]->Set_Type(eType);

		m_Cameras[eType]->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		m_Cameras[eType]->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	return S_OK;
}

void CCamera_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	
	for (_uint i = 0; i < CAMERA_TYPE::CAMERA_TYPE_END; ++i)
	{
		Safe_Release(m_Cameras[i]);
		m_Cameras[i] = nullptr;
	}
}



