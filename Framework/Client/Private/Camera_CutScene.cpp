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

HRESULT CCamera_CutScene::Start_CutScene(const string& strCutSceneName)
{
	m_pCurCutSceneDesc = Find_CutSceneDesc(strCutSceneName);
	if (nullptr == m_pCurCutSceneDesc)
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_CutScene::Save_CutSceneDescs()
{
	/* json */

	return S_OK;
}

HRESULT CCamera_CutScene::Load_CutSceneDescs()
{
	/* json */

	return S_OK;
}

Vec4 CCamera_CutScene::Get_Point_In_Bezier(Vec3 vPoints[MAX_BEZIER_POINT], const _float& fRatio)
{
	if (nullptr == vPoints)
		return Vec4::UnitW;

	if (sizeof(vPoints) / sizeof(Vec3) != MAX_BEZIER_POINT)
		return Vec4::UnitW;

	const _float fNormalizedRatio = min(1.0f, max(0.0f, fRatio));

	const Vec3 v0 = Vec3::Lerp(vPoints[0], vPoints[1], fNormalizedRatio);
	const Vec3 v1 = Vec3::Lerp(vPoints[1], vPoints[2], fNormalizedRatio);
	const Vec3 v2 = Vec3::Lerp(vPoints[2], vPoints[3], fNormalizedRatio);

	const Vec3 r0 = Vec3::Lerp(v0, v1, fNormalizedRatio);
	const Vec3 r1 = Vec3::Lerp(v1, v2, fNormalizedRatio);

	return Vec3::Lerp(r0, r1, fNormalizedRatio);
}

HRESULT CCamera_CutScene::Add_CutSceneDesc(const CAMERA_CUTSCENE_DESC& desc)
{
	if (nullptr != Find_CutSceneDesc(desc.strCutSceneName))
		return E_FAIL;

	m_CutSceneDescs.push_back(desc);

	return S_OK;
}

HRESULT CCamera_CutScene::Del_CutSceneDesc(const string& strCutSceneName)
{
	if (nullptr == Find_CutSceneDesc(strCutSceneName))
		return E_FAIL;

	for (vector<CAMERA_CUTSCENE_DESC>::iterator iter = m_CutSceneDescs.begin(); iter != m_CutSceneDescs.end(); ++iter)
	{
		if (strCutSceneName == (*iter).strCutSceneName)
		{
			m_CutSceneDescs.erase(iter);
			return S_OK;
		}
	}

	return E_FAIL;
}

HRESULT CCamera_CutScene::Change_CutSceneDesc(const _int& iIndex, const CAMERA_CUTSCENE_DESC& desc)
{
	if (0 > iIndex || m_CutSceneDescs.size() <= iIndex)
		return E_FAIL;

	m_CutSceneDescs[iIndex] = desc;

	return S_OK;
}

CAMERA_CUTSCENE_DESC* CCamera_CutScene::Find_CutSceneDesc(const string& strCutSceneName)
{
	for (auto& iter : m_CutSceneDescs)
	{
		if (strCutSceneName == iter.strCutSceneName)
			return &iter;
	}

	return nullptr;
}

HRESULT CCamera_CutScene::Ready_Components()
{
	return S_OK;
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
