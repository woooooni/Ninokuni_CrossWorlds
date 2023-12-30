#include "stdafx.h"
#include "Camera_CutScene.h"
#include "GameInstance.h"

#include "Camera_Manager.h"

CCamera_CutScene::CCamera_CutScene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
	: CCamera(pDevice, pContext, strObjTag, OBJ_TYPE::OBJ_CAMERA)
{
}

CCamera_CutScene::CCamera_CutScene(const CCamera_CutScene& rhs)
	: CCamera(rhs)
{

}

HRESULT CCamera_CutScene::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_CutScene::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CCamera_CutScene::Tick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::Tick(fTimeDelta);

	m_CurrCutSceneDesc.Update(fTimeDelta);

	if (!m_CurrCutSceneDesc.bPlay)
		CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);
}

void CCamera_CutScene::LateTick(_float fTimeDelta)
{
	if (!m_bActive)
		return;

	__super::LateTick(fTimeDelta);
}

HRESULT CCamera_CutScene::Render()
{
	return S_OK;
}

HRESULT CCamera_CutScene::Add_CutSceneDesc(string strCutSceneName)
{
	if (Has_CutSceneDesc(strCutSceneName))
		return E_FAIL;

	CAMERA_CUTSCENE_DESC CutSceneDesc;

	m_CutSceneDescs.emplace(strCutSceneName, CutSceneDesc);
	
	if (FAILED(Add_PathDesc(strCutSceneName)))
		return E_FAIL;

	m_CurrCutSceneDesc = CutSceneDesc;

	return S_OK;
}

HRESULT CCamera_CutScene::Del_CutSceneDesc(string strCutSceneName)
{
	if (Has_CutSceneDesc(strCutSceneName))
		return E_FAIL;

	auto& iter = m_CutSceneDescs.find(strCutSceneName);

	if (iter == m_CutSceneDescs.end())
		return E_FAIL;

	m_CutSceneDescs.erase(iter);

	return S_OK;
}

const _bool CCamera_CutScene::Has_CutSceneDesc(string strCutSceneName)
{
	auto& iter = m_CutSceneDescs.find(strCutSceneName);

	if (iter == m_CutSceneDescs.end())
		return false;

	return true;
}

HRESULT CCamera_CutScene::Change_CutSceneName(string strOriginName, string strChangedName)
{
	auto& iter = m_CutSceneDescs.find(strOriginName);

	if (iter == m_CutSceneDescs.end())
		return E_FAIL;

	m_CutSceneDescs[strChangedName] = move(iter->second);

	m_CutSceneDescs.erase(iter);

	return S_OK;
}

HRESULT CCamera_CutScene::Add_PathDesc(string strCutSceneName)
{
	auto& iter = m_CutSceneDescs.find(strCutSceneName);

	if (iter == m_CutSceneDescs.end())
		return E_FAIL;

	CAMERA_CUSTCENE_PATH_DESC CutScenePathDesc;
	{
		CutScenePathDesc.m_tTimeDesc.fEndTime = 1.f;

		/* Postion */
		Vec3 vCurCamLook = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Look();

		Vec3 vCreateStartPos = CCamera_Manager::GetInstance()->Get_CurCamera()->Get_Transform()->Get_Position();

		vCreateStartPos += vCurCamLook * 30.f;

		Vec3 vPosDelta = Vec3(10.f, 0.f, 0.f);

		for (size_t i = 0; i < 4; i++)
		{
			Vec3 vCreatePos = vCreateStartPos + vPosDelta + (vCurCamLook.ZeroY() * 30.f * i);

			CutScenePathDesc.vCamPositions[i] = vCreatePos;
		}

		/* LookAt */
		Vec3 vLookDelta = Vec3(-10.f, 0.f, 0.f);

		for (size_t i = 0; i < 4; i++)
		{
			Vec3 vCreatePos = vCreateStartPos + vPosDelta + (vCurCamLook.ZeroY() * 30.f * i);

			CutScenePathDesc.vCamLookAts[i] = vCreatePos;
		}
	}

	/* 만약 이전에 추가된 경로가 있다면 현재 추가될 위치의 첫 포지션을 앞 경로의 마지막 경로로 잡아준다.*/
	if (!iter->second.tPaths.empty())
	{
		CutScenePathDesc.vCamPositions[0] = iter->second.tPaths.back().vCamPositions[0];

		CutScenePathDesc.vCamLookAts[0] = iter->second.tPaths.back().vCamLookAts[0];
	}

	iter->second.tPaths.push_back(CutScenePathDesc);

	return S_OK;
}

HRESULT CCamera_CutScene::Del_PathDesc(string strCutSceneName, const _uint iIndex)
{
	return S_OK;
}

HRESULT CCamera_CutScene::Start_CutScene(string strCutSceneName)
{
	auto& iter = m_CutSceneDescs.find(strCutSceneName);

	if (iter == m_CutSceneDescs.end())
		return E_FAIL;

	m_CurrCutSceneDesc = iter->second;

	m_CurrCutSceneDesc.Start();

	return S_OK;
}

Vec3 CCamera_CutScene::Get_Bezier_CamPosition(string strCutSceneName)
{
	if (!m_CurrCutSceneDesc.bPlay)
		return Vec3::Zero;

	CAMERA_CUSTCENE_PATH_DESC path = m_CurrCutSceneDesc.tPaths[m_CurrCutSceneDesc.iCurPathIndex];

	return Calculate_Bezier_Position(path.vCamPositions, path.m_tTimeDesc.fCurTime / path.m_tTimeDesc.fEndTime );
}

Vec3 CCamera_CutScene::Get_Bezier_CamLookAt(string strCutSceneName)
{
	if (!m_CurrCutSceneDesc.bPlay)
		return Vec3::Zero;

	CAMERA_CUSTCENE_PATH_DESC path = m_CurrCutSceneDesc.tPaths[m_CurrCutSceneDesc.iCurPathIndex];

	return Calculate_Bezier_Position(path.vCamLookAts, path.m_tTimeDesc.fCurTime / path.m_tTimeDesc.fEndTime);
}

HRESULT CCamera_CutScene::Ready_Components()
{
	return S_OK;
}

Vec3 CCamera_CutScene::Calculate_Bezier_Position(Vec3 vPoint[MAX_BEZIER_POINT], const _float& fRatio)
{
	/* 4개의 베지어 포인트와 시간에 대한 곡선상 위치를 찾는다. */

	const Vec3 v0 = Vec3::Lerp(vPoint[0], vPoint[1], fRatio);
	const Vec3 v1 = Vec3::Lerp(vPoint[1], vPoint[2], fRatio);
	const Vec3 v2 = Vec3::Lerp(vPoint[2], vPoint[3], fRatio);

	const Vec3 r0 = Vec3::Lerp(v0, v1, fRatio);
	const Vec3 r1 = Vec3::Lerp(v1, v2, fRatio);

	return Vec3::Lerp(r0, r1, fRatio);
}

CCamera_CutScene* CCamera_CutScene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjTag)
{
	CCamera_CutScene* pInstance = new CCamera_CutScene(pDevice, pContext, strObjTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CCamera_CutScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera_CutScene::Clone(void* pArg)
{
	CCamera_CutScene* pInstance = new CCamera_CutScene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCamera_CutScene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_CutScene::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
