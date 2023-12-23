#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "HierarchyNode.h"
#include "Animation.h"
#include "Shader.h"
#include "Utils.h"
#include "Transform.h"
#include <fstream>
#include <filesystem>
#include "VIBuffer_Instancing.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Model_Manager.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{


}

CModel::CModel(const CModel& rhs)
	: CComponent(rhs)
	, m_pAIScene(rhs.m_pAIScene)
	, m_iNumMeshes(rhs.m_iNumMeshes)
	, m_iNumMaterials(rhs.m_iNumMaterials)
	, m_Meshes(rhs.m_Meshes)
	, m_Materials(rhs.m_Materials)
	, m_eModelType(rhs.m_eModelType)
	, m_Animations(rhs.m_Animations)
	, m_PivotMatrix(rhs.m_PivotMatrix)
	, m_iNumAnimations(rhs.m_iNumAnimations)
	, m_pSRV(rhs.m_pSRV)
	, m_strName(rhs.m_strName)
	, m_strFileName(rhs.m_strFileName)
	, m_strFolderPath(rhs.m_strFolderPath)
	, m_bFromBinary(rhs.m_bFromBinary)

{
	for (auto& pMeshContainer : m_Meshes)
		Safe_AddRef(pMeshContainer);

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_AddRef(Material.pTexture[i]);
	}

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

	for (auto& pNode : rhs.m_HierarchyNodes)
	{
		CHierarchyNode* pNewNode = pNode->Clone();
		m_HierarchyNodes.push_back(pNewNode);
	}

	for (auto& pNode : m_HierarchyNodes)
		pNode->Initialize_Bin(this);

	Safe_AddRef(m_pSRV);
}


void CModel::Debug_Animation()
{
	cout << "Curr Anim : " << m_TweenDesc.cur.iAnimIndex <<
		"\tCurr Frame : "		<< m_TweenDesc.cur.iCurFrame <<
		"\tNext Frame : "		<< m_TweenDesc.cur.iNextFrame <<
		"\tFrame Ratio : "		<< m_TweenDesc.cur.fRatio <<
		"\tIs Finish : "		<< _bool(m_TweenDesc.cur.iFinish) <<
		"\tIs Fix : "		<< _bool(m_TweenDesc.cur.iFix) <<
		"\tIs Stop : "		<< _bool(m_TweenDesc.cur.iStop) << endl;

	cout << "Next Anim : "	<< m_TweenDesc.next.iAnimIndex <<
		"\tCurr Frame : " << m_TweenDesc.next.iCurFrame <<
		"\tNext Frame : " << m_TweenDesc.next.iNextFrame <<
		"\tFrame Ratio :" 		<< m_TweenDesc.next.fRatio << 
		"\tIs Finish : " << _bool(m_TweenDesc.cur.iFinish) <<
		"\tIs Fix : " << _bool(m_TweenDesc.cur.iFix) <<
		"\tIs Stop : " << _bool(m_TweenDesc.cur.iStop) << endl;

	cout << "Tween Ratio : " << m_TweenDesc.fTweenRatio << endl << endl;

}

HRESULT CModel::Initialize_Prototype(TYPE eType, const wstring& strModelFolderPath, const wstring& strModelFileName, _fmatrix PivotMatrix)
{
	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	_tchar		szFullPath[MAX_PATH] = L"";

	m_strFolderPath = strModelFolderPath;
	m_strFileName = strModelFileName;

	lstrcpy(szFullPath, m_strFolderPath.c_str());
	lstrcat(szFullPath, strModelFileName.c_str());

	_uint		iFlag = 0;

	m_eModelType = eType;

	if (TYPE_NONANIM == eType)
		iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	else
		iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;

	m_pAIScene = m_Importer.ReadFile(CUtils::ToString(szFullPath).c_str(), iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;

	if (FAILED(Ready_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0)))
		return E_FAIL;

	if (FAILED(Ready_MeshContainers(PivotMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(m_strFolderPath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize_Prototype_Part(const wstring& strModelFilePath, const wstring& strModelFileName, _fmatrix PivotMatrix, CModel* pCharacterModel)
{
	if (nullptr == pCharacterModel)
		return E_FAIL;

	XMStoreFloat4x4(&m_PivotMatrix, PivotMatrix);

	_tchar		szFullPath[MAX_PATH] = L"";

	m_strFolderPath = strModelFilePath;
	m_strFileName = strModelFileName;

	lstrcpy(szFullPath, m_strFolderPath.c_str());
	lstrcat(szFullPath, strModelFileName.c_str());

	_uint		iFlag = 0;

	m_eModelType = TYPE::TYPE_NONANIM;

	if (TYPE_NONANIM == m_eModelType)
		iFlag |= aiProcess_PreTransformVertices | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;
	else
		iFlag |= aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace;

	m_pAIScene = m_Importer.ReadFile(CUtils::ToString(szFullPath).c_str(), iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;

	//if (FAILED(Ready_HierarchyNodes(m_pAIScene->mRootNode, nullptr, 0)))
	//	return E_FAIL;

	if (FAILED(Ready_MeshContainers_Part(PivotMatrix, pCharacterModel)))
		return E_FAIL;

	if (FAILED(Ready_Materials(m_strFolderPath)))
		return E_FAIL;

	//if (FAILED(Ready_Animations()))
	//	return E_FAIL;

	return S_OK;
}


HRESULT CModel::Initialize(void* pArg)
{
	_uint		iNumMeshes = 0;

	vector<CMesh*>		MeshContainers;

	for (auto& pPrototype : m_Meshes)
	{
		CMesh* pMeshContainer = (CMesh*)pPrototype->Clone();
		if (nullptr == pMeshContainer)
			return E_FAIL;

		MeshContainers.push_back(pMeshContainer);

		Safe_Release(pPrototype);
	}

	m_Meshes.clear();

	m_Meshes = MeshContainers;

	if (TYPE_ANIM == m_eModelType)
	{
		for (auto& pMeshContainer : m_Meshes)
		{
			if (nullptr != pMeshContainer)
				pMeshContainer->SetUp_HierarchyNodes(this, m_pAIScene->mMeshes[iNumMeshes++]);
		}
	}

	vector<CAnimation*>		Animations;

	for (auto& pPrototype : m_Animations)
	{
		CAnimation* pAnimation = pPrototype->Clone(this);
		if (nullptr == pAnimation)
			return E_FAIL;

		Animations.push_back(pAnimation);

		Safe_Release(pPrototype);
	}

	m_Animations.clear();

	m_Animations = Animations;

	return S_OK;
}

HRESULT CModel::Initialize_Bin(void* pArg)
{
	_uint		iNumMeshes = 0;

	vector<CMesh*>		MeshContainers;
	MeshContainers.reserve(m_Meshes.size());

	for (auto& pPrototype : m_Meshes)
	{
		CMesh* pMeshContainer = (CMesh*)pPrototype->Clone();
		if (nullptr == pMeshContainer)
			return E_FAIL;

		MeshContainers.push_back(pMeshContainer);
		Safe_Release(pPrototype);
	}

	m_Meshes.clear();

	m_Meshes = MeshContainers;

	if (TYPE_ANIM == m_eModelType)
	{
		for (auto& pMeshContainer : m_Meshes)
		{
			if (nullptr != pMeshContainer)
				pMeshContainer->SetUp_HierarchyNodes(this);
		}
	}

	vector<CAnimation*>		Animations;
	Animations.reserve(m_Animations.size());

	for (auto& pPrototype : m_Animations)
	{
		CAnimation* pAnimation = pPrototype->Clone(this);
		if (nullptr == pAnimation)
			return E_FAIL;

		Animations.push_back(pAnimation);
		Safe_Release(pPrototype);
	}
	m_Animations.clear();
	m_Animations = Animations;

	return S_OK;
}

HRESULT CModel::LateTick(_float fTimeDelta)
{
	if (TYPE::TYPE_ANIM != m_eModelType 
		|| m_Animations.empty()
		|| m_TweenDesc.cur.iAnimIndex < 0 
		||m_Animations.size() <= m_TweenDesc.cur.iAnimIndex
		|| nullptr == m_pSRV 
		|| m_TweenDesc.cur.iStop)
		return S_OK;

	/* 현재 애니메이션 */
	CAnimation* pCurAnim = m_Animations[m_TweenDesc.cur.iAnimIndex];
	if (nullptr != pCurAnim)
	{
		m_TweenDesc.cur.fFrameAcc += fTimeDelta;

		const _float fTimePerFrame = 1 / (pCurAnim->Get_TickPerSecond() * pCurAnim->Get_LiveSpeed());

		if (fTimePerFrame <= m_TweenDesc.cur.fFrameAcc)
		{
			if (0 == m_TweenDesc.cur.iNextFrame) // 애니메이션 종료 여부 체크
			{
				m_TweenDesc.cur.iFinish = true;

				if (!pCurAnim->Is_Loop()) // 픽스 여부 체크
					m_TweenDesc.cur.iFix = true;
				else
				{
					pCurAnim->Clear_AnimationEvent();
					m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationSpeed();
				}
			}

			if (!m_TweenDesc.cur.iFix) // 픽스가 아닐때만 프레임 갱신 (픽스라면 현재 프레임은 마지막 프레임으로 고정)
			{
				if (0 != m_TweenDesc.cur.iNextFrame)
					m_TweenDesc.cur.iFinish = false;

				m_TweenDesc.cur.fFrameAcc = 0.f;
				m_TweenDesc.cur.iCurFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
				m_TweenDesc.cur.iNextFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
			}
		}

		m_TweenDesc.cur.fRatio = m_TweenDesc.cur.fFrameAcc / fTimePerFrame;
		std::clamp(m_TweenDesc.cur.fRatio, 0.f, 1.f);

		/* 픽스 혹은, 다음 프레임이 0인데 루프가 아니라면 마지막 프레임의 0비율로 고정한다. 즉 마지막 프레임을 0번째 프레임과 보간을 하지 않는다. */
		if (m_TweenDesc.cur.iFix || (!pCurAnim->Is_Loop() && 0 == m_TweenDesc.cur.iNextFrame))
			m_TweenDesc.cur.fRatio = 0.f;

		pCurAnim->Update_Animation_Data(fTimeDelta, m_TweenDesc);
	}

	/* 다음 애니메이션이 예약되어 있다면 */
	if (0 <= m_TweenDesc.next.iAnimIndex)
	{
		CAnimation* pNextAnim = m_Animations[m_TweenDesc.next.iAnimIndex];
		if (nullptr == pNextAnim) return E_FAIL;

		/* 트위닝 보간 비율 설정 */
		m_TweenDesc.fTweenAcc += fTimeDelta;
		m_TweenDesc.fTweenRatio = m_TweenDesc.fTweenAcc / m_TweenDesc.fTweenDuration;
		std::clamp(m_TweenDesc.fTweenRatio, 0.f, 1.f);

		/* 트위닝 종료*/
		if (1.f <= m_TweenDesc.fTweenRatio)
		{
			pCurAnim->Clear_AnimationEvent();
			m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationSpeed();
			m_TweenDesc.cur = m_TweenDesc.next;
			m_TweenDesc.ClearNextAnim();
		}
		else
		{
			m_TweenDesc.next.fFrameAcc += fTimeDelta;

			const _float fTimePerFrame = 1 / (pNextAnim->Get_TickPerSecond() * pNextAnim->Get_LiveSpeed());

			if (fTimePerFrame <= m_TweenDesc.next.fFrameAcc)
			{
				m_TweenDesc.next.fFrameAcc = 0.f;
				m_TweenDesc.next.iCurFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
				m_TweenDesc.next.iNextFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
			}

			m_TweenDesc.next.fRatio = m_TweenDesc.next.fFrameAcc / fTimePerFrame;
			std::clamp(m_TweenDesc.next.fRatio, 0.f, 1.f);

			pNextAnim->Update_Animation_Data(fTimeDelta, m_TweenDesc);
		}

	}

	return S_OK;
}

const _int CModel::Get_HierarchyNodeIndex(const char* szBonename)
{
	for (int32 i = 0; i < m_HierarchyNodes.size(); ++i)
	{
		if (!strcmp(CUtils::ToString(m_HierarchyNodes[i]->Get_Name()).c_str(), szBonename))
			return i;
	}

	return -1;
}

const _int CModel::Get_HierarchyNodeIndex(wstring strBoneName)
{
	for (int32 i = 0; i < m_HierarchyNodes.size(); ++i)
	{
		if (m_HierarchyNodes[i]->Get_Name() == strBoneName)
			return i;
	}

	return -1;
}

void CModel::Set_CustomSocketPivotRotation(const _uint iIndex, Vec3 vCustomSocket)
{
	if (m_SocketCustomPivotRotation.size() <= iIndex)
		return;

	m_SocketCustomPivotRotation[iIndex] = vCustomSocket;
}

void CModel::Set_CustomSocketPivotPosition(const _uint iIndex, Vec3 vCustomSocket)
{
	if (m_SocketCustomPivotPosition.size() <= iIndex)
		return;

	m_SocketCustomPivotPosition[iIndex] = vCustomSocket;
}

Vec3 CModel::Get_CustomSocketPivotRotation(const _uint iIndex)
{
	if (m_SocketCustomPivotRotation.size() <= iIndex)
		return Vec3();

	return m_SocketCustomPivotRotation[iIndex];
}

Vec3 CModel::Get_CustomSocketPivotPosition(const _uint iIndex)
{
	if (m_SocketCustomPivotPosition.size() <= iIndex)
		return Vec3();

	return m_SocketCustomPivotPosition[iIndex];
}

Matrix CModel::Get_SocketLocalMatrix(const _uint iSocketEnumIndex)
{
	/* 수정시 클라 쪽 툴 모델에서도 수정 필요 (같은 함수)*/

	enum STEP { CURR, NEXT, STEP_END };

	Matrix		matAnimLocal;
	Vec4        vAnimLocalPos;

	Matrix		matAnim[STEP_END][STEP_END];
	Vec3		vScaleAnim[STEP_END][STEP_END];
	Quaternion  qQuatAnim[STEP_END][STEP_END];
	Vec3		vPosAnim[STEP_END][STEP_END];

	Vec3		vScale[STEP_END];
	Vec4		vRotation[STEP_END];
	Vec3		vPos[STEP_END];

	/* 현재 애니메이션 */
	Matrix matCurAnim;
	Vec4  vCurAnimPos;
	{
		matAnim[CURR][CURR] = m_SocketTransforms[iSocketEnumIndex][m_TweenDesc.cur.iAnimIndex].transforms[m_TweenDesc.cur.iCurFrame][0];
		matAnim[CURR][NEXT] = m_SocketTransforms[iSocketEnumIndex][m_TweenDesc.cur.iAnimIndex].transforms[m_TweenDesc.cur.iNextFrame][0];

		matAnim[CURR][CURR].Decompose(vScaleAnim[CURR][CURR], qQuatAnim[CURR][CURR], vPosAnim[CURR][CURR]);
		matAnim[CURR][NEXT].Decompose(vScaleAnim[CURR][NEXT], qQuatAnim[CURR][NEXT], vPosAnim[CURR][NEXT]);

		XMStoreFloat3(&vScale[CURR], XMVectorLerp(XMLoadFloat3(&vScaleAnim[CURR][CURR]), XMLoadFloat3(&vScaleAnim[CURR][NEXT]), m_TweenDesc.cur.fRatio));
		XMStoreFloat4(&vRotation[CURR], XMQuaternionSlerp(XMLoadFloat4(&qQuatAnim[CURR][CURR]), XMLoadFloat4(&qQuatAnim[CURR][NEXT]), m_TweenDesc.cur.fRatio));
		XMStoreFloat3(&vPos[CURR], XMVectorLerp(XMLoadFloat3(&vPosAnim[CURR][CURR]), XMLoadFloat3(&vPosAnim[CURR][NEXT]), m_TweenDesc.cur.fRatio));

		matCurAnim = XMMatrixAffineTransformation(
			XMLoadFloat3(&vScale[CURR]), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vRotation[CURR]), XMVectorSetW(XMLoadFloat3(&vPos[CURR]), 1.f));

		memcpy(&vCurAnimPos, matCurAnim.m[3], sizeof(Vec4));
	}

	/* 다음 애니메이션 */
	if (0 <= m_TweenDesc.next.iAnimIndex)
	{
		/* 다음 애니메이션 데이터 계산 */

		matAnim[NEXT][CURR] = m_SocketTransforms[iSocketEnumIndex][m_TweenDesc.next.iAnimIndex].transforms[m_TweenDesc.next.iCurFrame][0];
		matAnim[NEXT][NEXT] = m_SocketTransforms[iSocketEnumIndex][m_TweenDesc.next.iAnimIndex].transforms[m_TweenDesc.next.iNextFrame][0];

		matAnim[NEXT][CURR].Decompose(vScaleAnim[NEXT][CURR], qQuatAnim[NEXT][CURR], vPosAnim[NEXT][CURR]);
		matAnim[NEXT][NEXT].Decompose(vScaleAnim[NEXT][NEXT], qQuatAnim[NEXT][NEXT], vPosAnim[NEXT][NEXT]);

		XMStoreFloat3(&vScale[NEXT], XMVectorLerp(XMLoadFloat3(&vScaleAnim[NEXT][CURR]), XMLoadFloat3(&vScaleAnim[NEXT][NEXT]), m_TweenDesc.next.fRatio));
		XMStoreFloat4(&vRotation[NEXT], XMQuaternionSlerp(XMLoadFloat4(&qQuatAnim[NEXT][CURR]), XMLoadFloat4(&qQuatAnim[NEXT][NEXT]), m_TweenDesc.next.fRatio));
		XMStoreFloat3(&vPos[NEXT], XMVectorLerp(XMLoadFloat3(&vPosAnim[NEXT][CURR]), XMLoadFloat3(&vPosAnim[NEXT][NEXT]), m_TweenDesc.next.fRatio));

		/* 현재 애니메이션과 다음 애니메이션 보간 */
		{
			Vec3 vLerpScale;
			Vec4 vLerpRot;
			Vec3 vLerpPos;

			XMStoreFloat3(&vLerpScale, XMVectorLerp(XMLoadFloat3(&vScale[CURR]), XMLoadFloat3(&vScale[NEXT]), m_TweenDesc.fTweenRatio));
			XMStoreFloat4(&vLerpRot, XMQuaternionSlerp(XMLoadFloat4(&vRotation[CURR]), XMLoadFloat4(&vRotation[NEXT]), m_TweenDesc.fTweenRatio));
			XMStoreFloat3(&vLerpPos, XMVectorLerp(XMLoadFloat3(&vPos[CURR]), XMLoadFloat3(&vPos[NEXT]), m_TweenDesc.fTweenRatio));

			matAnimLocal = XMMatrixAffineTransformation(
				XMLoadFloat3(&vLerpScale), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMLoadFloat4(&vLerpRot), XMVectorSetW(XMLoadFloat3(&vLerpPos), 1.f));

			memcpy(&vAnimLocalPos, matAnimLocal.m[3], sizeof(Vec4));
		}
	}
	else
	{
		memcpy(&matAnimLocal, &matCurAnim, sizeof(Matrix));
		memcpy(&vAnimLocalPos, &vCurAnimPos, sizeof(Vec4));
	}


	/* 커스텀 피벗*/
	if (iSocketEnumIndex < m_SocketCustomPivotRotation.size())
	{
		Matrix matCustomPivot = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(
			XMConvertToRadians(m_SocketCustomPivotRotation[iSocketEnumIndex].x),
			XMConvertToRadians(m_SocketCustomPivotRotation[iSocketEnumIndex].y),
			XMConvertToRadians(m_SocketCustomPivotRotation[iSocketEnumIndex].z)));

		matAnimLocal = matCustomPivot * matAnimLocal;
	}

	/* 정규화 */
	{
		Vec3 vRight, vUp, vLook;
		memcpy(&vRight, matAnimLocal.m[0], sizeof(Vec3));
		memcpy(&vUp, matAnimLocal.m[1], sizeof(Vec3));
		memcpy(&vLook, matAnimLocal.m[2], sizeof(Vec3));

		vRight.Normalize();
		vUp.Normalize();
		vLook.Normalize();

		matAnimLocal.Right(vRight);
		matAnimLocal.Up(vUp);
		matAnimLocal.Backward(vLook);
	}


	/* 회전 행렬 포지션 초기화 */
	Vec4 vOneW = { 0.f, 0.f, 0.f, 1.f };
	memcpy(&matAnimLocal.m[3], &vOneW, sizeof(Vec4));

	/* 회전 행렬에서 피벗 포지션 값 뽑음*/
	Vec3 vPivotPosition = m_SocketCustomPivotRotation[iSocketEnumIndex];
	vPivotPosition = XMVector3TransformCoord(vPivotPosition, matAnimLocal);

	/* 회전 행렬 포지션에 기존 로컬 포지션과 피벗 포지션 값을 더함 */
	vAnimLocalPos += Vec4(vPivotPosition.x, vPivotPosition.y, vPivotPosition.z, 0.f);

	/* 포지션 적용  */
	memcpy(&matAnimLocal.m[3], &vAnimLocalPos, sizeof(Vec4));

	return matAnimLocal;
}

wstring CModel::Get_HiearachyNodeName(const _uint iIndex)
{
	for (int32 i = 0; i < m_HierarchyNodes.size(); ++i)
	{
		if (i == iIndex)
			return m_HierarchyNodes[i]->Get_Name();
	}

	return L"None";
}

CHierarchyNode* CModel::Get_HierarchyNode(const wstring& strNodeName)
{
	auto	iter = find_if(m_HierarchyNodes.begin(), m_HierarchyNodes.end(), [&](CHierarchyNode* pNode)
		{
			return strNodeName == pNode->Get_Name();
		});

	if (iter == m_HierarchyNodes.end())
		return nullptr;

	return *iter;
}

CHierarchyNode* CModel::Get_HierarchyNode(const _uint iIndex)
{
	for (int32 i = 0; i < m_HierarchyNodes.size(); ++i)
	{
		if (i == iIndex)
			return m_HierarchyNodes[i];
	}

	return nullptr;
}

_uint CModel::Get_MaterialIndex(_uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Get_MaterialIndex();
}

CTexture* CModel::Get_MaterialTexture(_uint iMeshIndex, _uint iTextureType)
{
	return m_Materials[iMeshIndex].pTexture[iTextureType];
}

const _bool CModel::Has_Animation(const wstring& strAnimationName)
{
	for (auto& pAnim : m_Animations)
	{
		if (strAnimationName == pAnim->Get_AnimationName())
			return TRUE;
	}

	return FALSE;
}

HRESULT CModel::Set_VtfSrv(ID3D11ShaderResourceView* pSrv)
{
	if (nullptr == pSrv)
		return E_FAIL;

	if (nullptr != m_pSRV)
		Safe_Release(m_pSRV);

	m_pSRV = pSrv;
	Safe_AddRef(m_pSRV);

	return S_OK;
}

HRESULT CModel::Clear_NotUsedData()
{
	/* HierarchyNodes */
	{
		/*for (auto& pHierarchyNode : m_HierarchyNodes)
		{
			Safe_Release(pHierarchyNode);

			if(nullptr != pHierarchyNode)
				Safe_Release(pHierarchyNode);
		}

		m_HierarchyNodes.clear();*/
	}


	/* Animations */
	{
		for (auto& pAnimation : m_Animations)
			pAnimation->Clear_Channels();
	}
	return S_OK;
}

HRESULT CModel::Set_Animation(const _uint& iAnimationIndex, const _float& fTweenDuration)
{
	if (TYPE::TYPE_NONANIM == m_eModelType || m_Animations.empty())
		return E_FAIL;

	_uint iIndex = iAnimationIndex % m_Animations.size();

	if (m_Animations.size() <= iIndex)
		return E_FAIL;

	if (m_TweenDesc.cur.iAnimIndex < 0) // 최초 1회 실행 
	{
		if (0 <= m_TweenDesc.cur.iAnimIndex)
		{
			m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationEvent();
			m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationSpeed();
		}

		m_TweenDesc.cur.iAnimIndex = iAnimationIndex % m_Animations.size();
		return S_OK;
	}

	/* 일부러 보간 시간을 음수로 주는 경우, 그 즉시 보간 없이 바로 다음 애니메이션을 세팅한다. */
	if (fTweenDuration <= 0.f)
	{
		if (0 <= m_TweenDesc.cur.iAnimIndex)
		{
			m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationEvent();
			m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationSpeed();
		}
		m_TweenDesc.cur.iAnimIndex = iAnimationIndex % m_Animations.size();
		m_TweenDesc.ClearNextAnim();
		return S_OK;
	}

	m_TweenDesc.ClearNextAnim();
	m_TweenDesc.next.iAnimIndex = iAnimationIndex % m_Animations.size();
	m_TweenDesc.fTweenDuration = fTweenDuration;
	//m_Animations[m_TweenDesc.cur.iAnimIndex]->Clear_AnimationSpeed();

	return S_OK;
}

HRESULT CModel::Set_Animation(const wstring& strAnimationName, const _float& fTweenDuration)
{
	for (size_t i = 0; i < m_Animations.size(); ++i)
	{
		if (strAnimationName == m_Animations[i]->Get_AnimationName())
		{
			Set_Animation(i, fTweenDuration);
			return S_OK;
		}
	}
	return E_FAIL;
}

CAnimation* CModel::Get_Animation(const _uint& iIndex)
{
	if (m_Animations.size() <= iIndex)
		return nullptr;

	return m_Animations[iIndex];
}

CAnimation* CModel::Get_Animation(const string strName)
{
	for (auto& pAnim : m_Animations)
	{
		if (pAnim->Get_AnimationName() == CUtils::ToWString(strName))
		{
			return pAnim;
		}
	}
	return nullptr;
}

const _int CModel::Get_AnimationIndex(const wstring& strName)
{
	for (size_t i = 0; i < m_Animations.size(); i++)
	{
		if (m_Animations[i]->Get_AnimationName() == strName)
			return i;
	}

	return -1;
}

void CModel::Set_KeyFrame_By_Progress(_float fProgress)
{
	std::clamp(fProgress, 0.f, 1.f);

	_uint iKeyFrame = fProgress * (m_Animations[m_TweenDesc.cur.iAnimIndex]->Get_MaxFrameCount() - 1.f) - m_TweenDesc.cur.fRatio;

	m_TweenDesc.cur.iCurFrame = iKeyFrame % m_Animations[m_TweenDesc.cur.iAnimIndex]->Get_MaxFrameCount();
	m_TweenDesc.cur.iNextFrame = iKeyFrame % m_Animations[m_TweenDesc.cur.iAnimIndex]->Get_MaxFrameCount();
	m_TweenDesc.cur.fRatio = 0.f;
}


HRESULT CModel::SetUp_OnShader(CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (m_Materials[iMaterialIndex].pTexture[eTextureType] == nullptr)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Bind_ShaderResource(pShader, pConstantName);
}

HRESULT CModel::SetUp_VTF(class CShader* pShader)
{
	if (nullptr == m_pSRV ||  nullptr == pShader || 0 > m_TweenDesc.cur.iAnimIndex)
		return E_FAIL;

	if (FAILED(pShader->Bind_Texture("g_TransformMap", m_pSRV)))
		return E_FAIL;

	if (FAILED(pShader->Bind_RawValue("g_TweenFrames", &m_TweenDesc, sizeof(TWEEN_DESC))))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, _uint iPassIndex)
{
	pShader->Begin(iPassIndex);
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}


HRESULT CModel::Render_Instancing(CShader* pShader, _uint iMeshIndex, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, _uint iPassIndex)
{
	if (TYPE_ANIM == m_eModelType)
	{
		if (FAILED(pShader->Bind_Texture("g_TransformMap", m_pSRV)))
			return E_FAIL;
	}

	pShader->Begin(iPassIndex);
	pInstancingBuffer->Render(WorldMatrices, m_Meshes[iMeshIndex]);

	return S_OK;
}

const _float CModel::Get_Progress()
{
	if (m_TweenDesc.cur.iFinish)
		return 1.f;

	_float fProgress = (_float(m_TweenDesc.cur.iCurFrame) + m_TweenDesc.cur.fRatio) / _float(m_Animations[m_TweenDesc.cur.iAnimIndex]->Get_MaxFrameCount() - 1.f);
	
	std::clamp(fProgress, 0.f, 1.f);
	
	return fProgress;
}

HRESULT CModel::Swap_Animation(_uint iSrcIndex, _uint iDestIndex)
{
	if (TYPE_ANIM != m_eModelType)
		return E_FAIL;

	if (0 > iSrcIndex || 0 > iDestIndex || m_Animations.size() <= iSrcIndex || m_Animations.size() <= iDestIndex)
		return E_FAIL;

	CAnimation* Temp = m_Animations[iDestIndex];
	m_Animations[iDestIndex] = m_Animations[iSrcIndex];
	m_Animations[iSrcIndex] = Temp;

	//m_iCurrentAnimIndex = iDestIndex;

	return S_OK;
}

void CModel::Clear_SocketTransformsCache(const _uint iSocketIndex)
{
	if (m_HierarchyNodes.size() <= iSocketIndex || m_SocketTransformIndexCache.empty())
		return;

	_int iIndex = 0;
	for (vector<_uint>::iterator iter = m_SocketTransformIndexCache.begin(); iter != m_SocketTransformIndexCache.end();)
	{
		if (iSocketIndex == *iter)
		{
			m_SocketTransformIndexCache.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iIndex;
		}
	}

	_int iCount = 0;
	for (vector<Vec3>::iterator iter = m_SocketCustomPivotRotation.begin(); iter != m_SocketCustomPivotRotation.end();)
	{
		if (iCount == iIndex)
		{
			m_SocketCustomPivotRotation.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

	iCount = 0;
	for (vector<Vec3>::iterator iter = m_SocketCustomPivotPosition.begin(); iter != m_SocketCustomPivotPosition.end();)
	{
		if (iCount == iIndex)
		{
			m_SocketCustomPivotPosition.erase(iter);
			break;
		}
		else
		{
			++iter;
			++iCount;
		}
	}

}

void CModel::Clear_All_SocketTransformsCaches()
{
	m_SocketTransformIndexCache.clear(); 
	m_SocketTransformIndexCache.shrink_to_fit(); 

	m_SocketCustomPivotRotation.clear(); 
	m_SocketCustomPivotRotation.shrink_to_fit();

	m_SocketCustomPivotPosition.clear();
	m_SocketCustomPivotPosition.shrink_to_fit();
}

HRESULT CModel::Delete_Animation(_uint iIndex)
{
	if (TYPE_ANIM != m_eModelType)
		return E_FAIL;

	if (0 > iIndex || m_Animations.size() <= iIndex)
		return E_FAIL;

	vector<CAnimation*>::iterator iter = m_Animations.begin();
	iter += iIndex;
	Safe_Release(*iter);

	m_Animations.erase(iter);

	//m_iCurrentAnimIndex = 0 > m_iCurrentAnimIndex - 1 ? 0 : m_iCurrentAnimIndex;
	//m_iCurrentAnimIndex = m_Animations.size() >= m_iCurrentAnimIndex ? m_Animations.size() - 1 : m_iCurrentAnimIndex;

	return S_OK;
}

_int CModel::Find_AnimationIndex(const wstring& strAnimationTag)
{
	_int iIndex = 0;

	for (auto& pAnimation : m_Animations)
	{
		if (pAnimation->Get_AnimationName() == strAnimationTag)
			return iIndex;

		++iIndex;
	}

	return -1;
}

HRESULT CModel::Ready_MeshContainers(_fmatrix PivotMatrix)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMeshContainer = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Ready_MeshContainers_Part(_fmatrix PivotMatrix, CModel* pCharacterModel)
{
	if (nullptr == pCharacterModel)
		return E_FAIL;

	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (_uint i = 0; i < m_iNumMeshes; ++i)
	{
		CMesh* pMeshContainer = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], this, PivotMatrix);
		if (nullptr == pMeshContainer)
			return E_FAIL;

		m_Meshes.push_back(pMeshContainer);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const wstring& ModelFilePath)
{
	if (nullptr == m_pAIScene)
		return E_FAIL;

	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (_uint i = 0; i < m_iNumMaterials; ++i)
	{
		MATERIALDESC		MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		for (_uint j = 0; j < AI_TEXTURE_TYPE_MAX; ++j)
		{
			aiString		strPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strPath)))
				continue;

			_tchar			szFullPath[MAX_PATH] = L"";
			_tchar			szFileName[MAX_PATH] = L"";
			_tchar			szExt[MAX_PATH] = L"";

			_wsplitpath_s(CUtils::ToWString(strPath.C_Str()).c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);


			lstrcpy(szFullPath, ModelFilePath.c_str());
			lstrcat(szFullPath, szFileName);
			lstrcat(szFullPath, szExt);

			memcpy(MaterialDesc.strName, &szFileName, sizeof(wchar_t) * MAX_PATH);

			MaterialDesc.pTexture[j] = CTexture::Create(m_pDevice, m_pContext, szFullPath);
			if (nullptr == MaterialDesc.pTexture[j])
				return E_FAIL;
		}

		m_Materials.push_back(MaterialDesc);
	}

	return S_OK;
}

HRESULT CModel::Ready_HierarchyNodes(aiNode* pNode, CHierarchyNode* pParent, _uint iDepth)
{
	CHierarchyNode* pHierarchyNode = CHierarchyNode::Create(pNode, pParent, iDepth++);

	if (nullptr == pHierarchyNode)
		return E_FAIL;

	m_HierarchyNodes.push_back(pHierarchyNode);

	for (_uint i = 0; i < pNode->mNumChildren; ++i)
	{
		Ready_HierarchyNodes(pNode->mChildren[i], pHierarchyNode, iDepth);
	}

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	for (_uint i = 0; i < m_pAIScene->mNumAnimations; ++i)
	{
		aiAnimation* pAIAnimation = m_pAIScene->mAnimations[i];

		CAnimation* pAnimation = CAnimation::Create(pAIAnimation);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.push_back(pAnimation);
	}
	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const wstring& strModelFilePath, const wstring& strModelFileName, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, strModelFilePath, strModelFileName, PivotMatrix)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CModel* CModel::Create_Bin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const wstring& strModelFilePath, const wstring& strModelFileName, _fmatrix PivotMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);
	pInstance->m_bFromBinary = true;

	return pInstance;
}

CModel* CModel::Create_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const wstring& strModelFilePath, const wstring& strModelFileName, CModel* pCharacterModel, _fmatrix PivotMatrix)
{
	if (nullptr == pCharacterModel)
		return nullptr;

	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype_Part(strModelFilePath, strModelFileName, PivotMatrix, pCharacterModel)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);
	if (false == m_bFromBinary)
	{
		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX("Failed To Cloned : CModel");
			Safe_Release(pInstance);
		}
		return pInstance;
	}
	else
	{
		if (FAILED(pInstance->Initialize_Bin(pArg)))
		{
			MSG_BOX("Failed To Cloned Bin: CModel");
			Safe_Release(pInstance);
		}
		return pInstance;
	}
}

void CModel::Free()
{
	__super::Free();

	for (auto& Material : m_Materials)
	{
		for (_uint i = 0; i < AI_TEXTURE_TYPE_MAX; ++i)
			Safe_Release(Material.pTexture[i]);
	}
	m_Materials.clear();

	for (auto& pMeshContainer : m_Meshes)
		Safe_Release(pMeshContainer);
	m_Meshes.clear();


	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);
	m_Animations.clear();


	for (auto& pHierarchyNode : m_HierarchyNodes)
		Safe_Release(pHierarchyNode);
	m_HierarchyNodes.clear();

	m_Importer.FreeScene();

	Safe_Release(m_pSRV);
}