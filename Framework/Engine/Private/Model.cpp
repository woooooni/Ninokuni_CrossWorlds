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
	, m_pMatrixTexture(rhs.m_pMatrixTexture)
	, m_Matrices(rhs.m_Matrices)
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
	Safe_AddRef(m_pMatrixTexture);
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

	/*if (FAILED(Ready_Animation_Texture()))
		return E_FAIL;*/

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


	m_Matrices.reserve(m_HierarchyNodes.size());
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


	m_Matrices.reserve(m_HierarchyNodes.size());
	return S_OK;
}

HRESULT CModel::LateTick(_float fTimeDelta)
{
	if (TYPE::TYPE_ANIM != m_eModelType || m_TweenDesc.cur.iAnimIndex < 0 ||  nullptr == m_pSRV)
		return E_FAIL;

	/* 현재 애니메이션 */
	CAnimation* pCurAnim = m_Animations[m_TweenDesc.cur.iAnimIndex];
	if (nullptr != pCurAnim)
	{
		m_TweenDesc.cur.fFrameAcc += fTimeDelta;

		const _float fTimePerFrame = 1 / (pCurAnim->Get_TickPerSecond() * pCurAnim->Get_AnimationSpeed());

		// 루프 체크 필요
		if (fTimePerFrame <= m_TweenDesc.cur.fFrameAcc)
		{
			m_TweenDesc.cur.fFrameAcc = 0.f;
			m_TweenDesc.cur.iCurFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
			m_TweenDesc.cur.iNextFrame = (m_TweenDesc.cur.iCurFrame + 1) % pCurAnim->Get_MaxFrameCount();
		}

		m_TweenDesc.cur.fRatio = m_TweenDesc.cur.fFrameAcc / fTimePerFrame;
		std::clamp(m_TweenDesc.cur.fRatio, 0.f, 1.f);
	}

	/* 다음 애니메이션이 예약되어 있다면 */
	if (0 <= m_TweenDesc.next.iAnimIndex)
	{
		/* 트위닝 보간 비율 설정 */
		m_TweenDesc.fTweenAcc += fTimeDelta;
		m_TweenDesc.fTweenRatio = m_TweenDesc.fTweenAcc / m_TweenDesc.fTweenDuration;
		std::clamp(m_TweenDesc.fTweenRatio, 0.f, 1.f);

		/* 트위닝 종료*/
		if (1.f <= m_TweenDesc.fTweenRatio)
		{
			m_TweenDesc.cur = m_TweenDesc.next;
			m_TweenDesc.ClearNextAnim();
		}
		else
		{
			CAnimation* pNextAnim = m_Animations[m_TweenDesc.next.iAnimIndex];
			if (nullptr != pNextAnim)
			{
				m_TweenDesc.next.fFrameAcc += fTimeDelta;

				const _float fTimePerFrame = 1 / (pNextAnim->Get_TickPerSecond() * pNextAnim->Get_AnimationSpeed());

				// 루프 체크 필요
				if (fTimePerFrame <= m_TweenDesc.next.fFrameAcc)
				{
					m_TweenDesc.next.fFrameAcc = 0.f;
					m_TweenDesc.next.iCurFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
					m_TweenDesc.next.iNextFrame = (m_TweenDesc.next.iCurFrame + 1) % pNextAnim->Get_MaxFrameCount();
				}

				m_TweenDesc.next.fRatio = m_TweenDesc.next.fFrameAcc / fTimePerFrame;
				std::clamp(m_TweenDesc.next.fRatio, 0.f, 1.f);
			}
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

_uint CModel::Get_MaterialIndex(_uint iMeshIndex)
{
	return m_Meshes[iMeshIndex]->Get_MaterialIndex();
}

CTexture* CModel::Get_MaterialTexture(_uint iMeshIndex, _uint iTextureType)
{
	return m_Materials[iMeshIndex].pTexture[iTextureType];
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
	if (m_Animations.size() <= iAnimationIndex)
		return E_FAIL;

	if (m_TweenDesc.cur.iAnimIndex < 0) // 최초 1회 실행 
	{
		m_TweenDesc.cur.iAnimIndex = iAnimationIndex % m_Animations.size();
		return S_OK;
	}

	m_TweenDesc.ClearNextAnim();
	m_TweenDesc.next.iAnimIndex = iAnimationIndex % m_Animations.size();
	m_TweenDesc.fTweenDuration = fTweenDuration;

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

HRESULT CModel::SetUp_OnShader(CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName)
{
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	if (m_Materials[iMaterialIndex].pTexture[eTextureType] == nullptr)
		return E_FAIL;
		
	return m_Materials[iMaterialIndex].pTexture[eTextureType]->Bind_ShaderResource(pShader, pConstantName);
}

HRESULT CModel::Render(CShader* pShader, _uint iMeshIndex, _uint iPassIndex)
{
	if (TYPE_ANIM == m_eModelType)
	{
		if (nullptr == m_pSRV || 0 > m_TweenDesc.cur.iAnimIndex)
			return E_FAIL;

		if (FAILED(pShader->Bind_Texture("g_TransformMap", m_pSRV)))
			return E_FAIL;

		if (FAILED(pShader->Bind_RawValue("g_TweenFrames", &m_TweenDesc, sizeof(TWEEN_DESC))))
			return E_FAIL;
	}

	pShader->Begin(iPassIndex);
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

HRESULT CModel::Render_Instancing(CShader* pShader, _uint iMeshIndex, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, _uint iPassIndex)
{
	if (TYPE_ANIM == m_eModelType)
	{
		m_Meshes[iMeshIndex]->SetUp_BoneMatrices(m_pMatrixTexture, m_Matrices, XMLoadFloat4x4(&m_PivotMatrix));
		if (FAILED(pShader->Bind_Texture("g_MatrixPallete", m_pSRV)))
			return E_FAIL;
	}

	pShader->Begin(iPassIndex);
	pInstancingBuffer->Render(WorldMatrices, m_Meshes[iMeshIndex]);

	return S_OK;
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

HRESULT CModel::Ready_Animation_Texture()
{
	/*if (TYPE::TYPE_NONANIM == m_eModelType)
		return S_OK;

	m_Matrices.resize(m_HierarchyNodes.size());

	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 4096;
	TextureDesc.Height = 2;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	Safe_Release(m_pMatrixTexture);
	Safe_Release(m_pSRV);

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pMatrixTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pMatrixTexture, nullptr, &m_pSRV)))
		return E_FAIL;*/

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
	Safe_Release(m_pMatrixTexture);
}