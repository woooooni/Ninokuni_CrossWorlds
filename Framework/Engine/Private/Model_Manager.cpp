#include "..\Public\Model_Manager.h"
#include "Model.h"
#include <filesystem>
#include "tinyxml2.h"
#include "FileUtils.h"
#include "HierarchyNode.h"
#include "Utils.h"
#include "Mesh.h"
#include "Texture.h"
#include "Animation.h"
#include "Channel.h"
#include "GameInstance.h"

#include "Sound_Manager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CModel_Manager);

CModel_Manager::CModel_Manager()
{

}

HRESULT CModel_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);

	XMStoreFloat4x4(&m_PivotMatrix, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationAxis(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(180.f)));

	return S_OK;
}

HRESULT CModel_Manager::Export_Model_Data(CModel* pModel, const wstring& strSubFolderName, const wstring& strFileName)
{
	_tchar szOriginFileName[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szOriginFileName, MAX_PATH, nullptr, 0);

	wstring strFinalFolderPath = m_strExportFolderPath + strSubFolderName + szOriginFileName;

	if (FAILED(Export_Mesh(strFinalFolderPath, pModel)))
		return E_FAIL;

	if (FAILED(Export_Material(strFinalFolderPath, pModel)))
		return E_FAIL;

	if (CModel::TYPE::TYPE_NONANIM == pModel->Get_ModelType())
		return S_OK;

	if (FAILED(Export_Animation(strFinalFolderPath, pModel)))
		return E_FAIL;

	if (CModel::TYPE::TYPE_ANIM == pModel->Get_ModelType())
	{
		if (FAILED(Create_Model_Vtf(pModel, strFinalFolderPath)))
			return E_FAIL;

		if (FAILED(Export_Socket(strFinalFolderPath, pModel)))
			return E_FAIL;

		if (FAILED(Export_Animation_KeyFrameSpeed(strFinalFolderPath, pModel)))
			return E_FAIL;

		if (FAILED(Export_Animation_Events(strFinalFolderPath, pModel)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CModel_Manager::Export_Model_Data_FromPath(_uint eType, wstring strFolderPath)
{
	for (auto& p : std::filesystem::directory_iterator(strFolderPath))
	{
		if (p.is_directory())
		{
			Export_Model_Data_FromPath(eType, p.path());
		}

		wstring strFilePath = CUtils::PathToWString(p.path().wstring());
		wstring strSubFolderPath = CUtils::PathToWString(p.path().parent_path());
		strSubFolderPath = strSubFolderPath.substr(strSubFolderPath.find_last_of(L"/") + 1, strSubFolderPath.size() - 1) + L"/";

		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		CModel* pModel = nullptr;
		if (0 == lstrcmp(TEXT(".fbx"), strExt))
		{
			Import_Model_Data(0, wstring(L"Prototype_Model_") + strFileName, eType, strFolderName, wstring(strFileName) + strExt, &pModel);
			if (nullptr == pModel)
				return E_FAIL;

			if (FAILED(Export_Model_Data(pModel, strSubFolderPath, strFileName)))
				return E_FAIL;

			Safe_Release(pModel);
		}
		Safe_Release(pModel);
	}
	return S_OK;
}

HRESULT CModel_Manager::Create_Model_Vtf(class CModel* pModel, const wstring strFilePath)
{
	if (nullptr == pModel || CModel::TYPE::TYPE_ANIM != pModel->Get_ModelType())
		return E_FAIL;

	if (0 == pModel->Get_Animations().size())
		return S_OK;

	ID3D11ShaderResourceView* pSrvCopy = Find_Model_Vtf(pModel->Get_Name());
	if(nullptr != pSrvCopy && 99 != GI->Get_CurrentLevel())
	{
		if (FAILED(pModel->Set_VtfSrv(pSrvCopy)))
			return E_FAIL;

		return S_OK;
	}

	/* 01. For m_AnimTransforms */
	/* 해당 모델이 사용하는 모든 애니메이션과 Bone의 정보를 m_AnimTransforms에 세팅한다. */
	m_HierarchyNodes	= pModel->Get_HierarchyNodes();
	m_AnimationsCache	= pModel->Get_Animations();

	const _uint iAnimCount = (_uint)m_AnimationsCache.size();
	const _uint iHiearachynodeCount = (_uint)m_HierarchyNodes.size();

	_uint iAnimMaxFrameCount = 0;

	for (uint32 i = 0; i < iAnimCount; i++)
	{
		_uint iCurAnimFrameCnt = (_uint)m_AnimationsCache[i]->Get_MaxFrameCount();

		iAnimMaxFrameCount = iAnimMaxFrameCount < iCurAnimFrameCnt ? iCurAnimFrameCnt : iAnimMaxFrameCount;
	}

	if (0 == iAnimMaxFrameCount) 
		return E_FAIL;

	if (!m_AnimTransformsCaches.empty())
	{
		m_AnimTransformsCaches.clear();
		m_AnimTransformsCaches.shrink_to_fit();
	}
	m_AnimTransformsCaches.resize(iAnimCount);

	for (_uint i = 0; i < iAnimCount; i++)
	{
		if (FAILED(Create_AnimationTransform_Caches(i)))
			return E_FAIL;
	}

	/* 02. For. m_pTexture */
  	ID3D11Texture2D* pTexture = nullptr;
	{
		D3D11_TEXTURE2D_DESC desc;
		{
			ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
			desc.Width = iHiearachynodeCount * 4;
			desc.Height = iAnimMaxFrameCount;
			desc.ArraySize = iAnimCount;
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			//desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.MipLevels = 1;
			desc.SampleDesc.Count = 1;
		}

		const uint32 dataSize = iHiearachynodeCount * sizeof(Matrix);	/* 가로 */
		const uint32 pageSize = dataSize * iAnimMaxFrameCount;			/* 한 장 (가로 * 세로) */
		void* mallocPtr = ::malloc(pageSize * iAnimCount);				/* 텍스처 총 데이터 = n 장 */

		
		for (uint32 c = 0; c < iAnimCount; c++) /* 애니메이션 갯수만큼 반복 (장 수) */
		{
			uint32 startOffset = c * pageSize; /* 애님 카운트 * 한 장 */

			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < m_AnimationsCache[c]->Get_MaxFrameCount() ; f++) /* 키프레임 갯수만큼 반복 (세로 크기만큼) */
			{
				void* ptr = pageStartPtr + dataSize * f;

				::memcpy(ptr, m_AnimTransformsCaches[c].transforms[f].data(), dataSize); /* 텍스처에 가로 1줄만큼 데이터 저장 */
			}
		}

		/* 텍스처를 만들기 위한 D3D11_SUBRESOURCE_DATA 생성 */
		vector<D3D11_SUBRESOURCE_DATA> subResources(iAnimCount);
		for (uint32 c = 0; c < iAnimCount; c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		/* 텍스처 생성 */
		if (FAILED(GI->Get_Device()->CreateTexture2D(&desc, subResources.data(), &pTexture)))
			return E_FAIL;

		::free(mallocPtr);
	}

	/* 03. For. m_pSrv */
	ID3D11ShaderResourceView* pSrv = { nullptr };
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = iAnimCount;

		if (FAILED(GI->Get_Device()->CreateShaderResourceView(pTexture, &desc, &pSrv)))
			return E_FAIL;
	}

	/* 04 For. Set Vtf Data To Model */
	if (FAILED(pModel->Set_VtfSrv(pSrv)))
		return E_FAIL;

	//* 05 For. Save Vtf Texture */
	/*if (FAILED(Save_Model_Vtf(strFilePath, pTexture)))
		return E_FAIL;*/

	/* 06 For. map */
	if (nullptr != Find_Model_Vtf(pModel->Get_Name()))
		m_VtfTextures.emplace(pModel->Get_Name(), pSrv);
	else
		Safe_Release(pSrv);

	/* 트랜스폼 맵 지우기 */
	m_AnimTransformsCaches.clear();
	m_AnimationsCache.shrink_to_fit();

	Safe_Release(pTexture);

	return S_OK;
}

HRESULT CModel_Manager::Save_Model_Vtf(const wstring strSaveFilePath, ID3D11Texture2D* pTexture)
{
	if ((99 != GI->Get_CurrentLevel()))
		return S_OK;

	if (nullptr == pTexture)
		return E_FAIL;

	const wstring strFinalSaveFilePath = strSaveFilePath + L"_vtf.dds";

	if (FAILED(SaveDDSTextureToFile(m_pContext, pTexture, strFinalSaveFilePath.c_str())))
	{
		MSG_BOX("VTF 텍스처 저장을 실패했습니다.");
		return E_FAIL;
	}
	
	return S_OK;
}

HRESULT CModel_Manager::Load_Model_Vtf(CModel* pModel, const wstring strLoadFilePath)
{
	if (nullptr == pModel)
		return E_FAIL;

	ID3D11ShaderResourceView* pSrv = Find_Model_Vtf(pModel->Get_Name());

	if(nullptr == pSrv)
	{
		/*ID3D11ShaderResourceView* pSrv = { nullptr };
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.MipLevels = 1;
			desc.Texture2DArray.ArraySize = pModel->Get_Animations().size();

			if (FAILED(GI->Get_Device()->CreateShaderResourceView(pTexture, &desc, &pSrv)))
				return E_FAIL;
		}*/
		

		/* Set File Path */
		const wstring	strFinalLoadFilePath = strLoadFilePath + L"_vtf.dds";

		if (!filesystem::exists(strFinalLoadFilePath) || !filesystem::is_regular_file(strFinalLoadFilePath))
			return E_FAIL;

		_tchar			szTextureFilePath[MAX_PATH] = TEXT("");
		wsprintf(szTextureFilePath, strFinalLoadFilePath.c_str());

		_tchar			szExt[MAX_PATH] = TEXT("");
		_wsplitpath_s(szTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);

		/* Load Texture And Create Srv */
		if (FALSE == lstrcmp(szExt, TEXT(".dds")))
		{
			if (FAILED(CreateDDSTextureFromFile(m_pDevice, szTextureFilePath, nullptr, &pSrv)))
				return E_FAIL;
		}
		else
			return E_FAIL;
	}

	if (nullptr == pSrv)
		return E_FAIL;

	/* Set To Model*/
	if (FAILED(pModel->Set_VtfSrv(pSrv)))
		return E_FAIL;

	/* Push Map */
	m_VtfTextures.emplace(pModel->Get_Name(), pSrv);

	return S_OK;
}

ID3D11ShaderResourceView* CModel_Manager::Find_Model_Vtf(const wstring strModelName)
{
	auto	iter = m_VtfTextures.find(strModelName);

	if (iter == m_VtfTextures.end())
		return nullptr;

	return iter->second;
}

HRESULT CModel_Manager::Import_Model_Data_From_Fbx(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, CModel** ppOut)
{
	CModel* pModel = nullptr; 
	
	/* 00. 기존 fbx 파일 임포트 루틴 */
	{
		pModel = CModel::Create(m_pDevice, m_pContext, CModel::TYPE(eType), strFolderPath, strFileName, XMLoadFloat4x4(&m_PivotMatrix));
	
		if (nullptr == pModel)
			return E_FAIL;

		if (FAILED(pModel->Initialize(nullptr)))
		{
			MSG_BOX("Model Initialize Failed : Data Manager");
			Safe_Release(pModel);
			return E_FAIL;
		}

		if (FAILED(GI->Add_Prototype(iLevelIndex, strProtoTypeTag, pModel)))
		{
			MSG_BOX("Model Add Prototype Failed : Model Manager");
			Safe_Release(pModel);
			return E_FAIL;
		}

		pModel->Set_Name(strFileName);

		if (ppOut != nullptr)
		{
			*ppOut = pModel;
			if (*ppOut == nullptr)
			{
				MSG_BOX("ppOut Initialize Failed. : Model Manager");
				return E_FAIL;
			}
		}
	}

	/* 01. 더미 바이너리와 vtf를 추출하기 위한 임시 익스포트 과정 (스태시 폴더에 저장되므로 기존 파일에 대한 수정은 이루어지지 않는다) */
	{
		if (CModel::TYPE::TYPE_ANIM == pModel->Get_ModelType())
		{
			const wstring strStashExportPath = L"../Bin/stash/stash";

			if (FAILED(Export_Mesh(strStashExportPath, pModel)))
				return E_FAIL;

			if (FAILED(Export_Material(strStashExportPath, pModel)))
				return E_FAIL;

			if (CModel::TYPE::TYPE_NONANIM == pModel->Get_ModelType())
				return S_OK;

			if (FAILED(Export_Animation(strStashExportPath, pModel)))
				return E_FAIL;

			if (FAILED(Create_Model_Vtf(pModel, strStashExportPath)))
				return E_FAIL;

			/*if (FALSE == filesystem::remove_all(strStashExportPath))
				return E_FAIL;*/
		}
	}
	return S_OK;
}

HRESULT CModel_Manager::Import_Model_Data_From_Bin_In_Tool(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, CModel** ppOut)
{
	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	CModel* pModel = CModel::Create_Bin(m_pDevice, m_pContext, CModel::TYPE(eType), strFolderPath, strFileName, XMLoadFloat4x4(&m_PivotMatrix));

	if (nullptr == pModel)
		return E_FAIL;

	wstring strFinalFolderPath = strFolderPath + szFileName;

	XMStoreFloat4x4(&pModel->m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix));

	if (FAILED(Import_Mesh(strFinalFolderPath, pModel, eType)))
	{
		MSG_BOX("Import_Mesh Failed.");
		Safe_Release(pModel);
		return E_FAIL;
	}

	if (FAILED(Import_Material(strFinalFolderPath, strFolderPath, pModel)))
	{
		MSG_BOX("Import_Material Failed.");
		Safe_Release(pModel);
		return E_FAIL;
	}

	if (CModel::TYPE::TYPE_NONANIM == pModel->Get_ModelType())
	{
		if (FAILED(GI->Add_Prototype(iLevelIndex, strProtoTypeTag, pModel)))
		{
			MSG_BOX("Model Add Prototype Failed : Model Manager");
			Safe_Release(pModel);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(Import_Animation(strFinalFolderPath, pModel)))
		{
			MSG_BOX("Import_Animation Failed.");
			Safe_Release(pModel);
			return E_FAIL;
		}

		if (FAILED(GI->Add_Prototype(iLevelIndex, strProtoTypeTag, pModel)))
		{
			MSG_BOX("Model Add Prototype Failed : Model Manager");
			Safe_Release(pModel);
			return E_FAIL;
		}
	}

	pModel->Set_Name(strFileName);

	if (CModel::TYPE::TYPE_ANIM == pModel->Get_ModelType())
	{
		if (FAILED(Create_Model_Vtf(pModel, strFinalFolderPath)))
			return E_FAIL;

		if (FAILED(Import_Socket(strFinalFolderPath, pModel)))
			return E_FAIL;

		if (FAILED(Import_Animation_KeyFrameSpeed(strFinalFolderPath, pModel)))
			return E_FAIL;

		if (FAILED(Import_Animation_Events(strFinalFolderPath, pModel)))
			return E_FAIL;
	}

	if (ppOut != nullptr)
	{
		*ppOut = pModel;
		if (*ppOut == nullptr)
		{
			MSG_BOX("ppOut Initialize Failed. : Model Manager");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CModel_Manager::Import_Model_Data_From_Bin_In_Game(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, CModel** ppOut)
{
	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	CModel* pModel = CModel::Create_Bin(m_pDevice, m_pContext, CModel::TYPE(eType), strFolderPath, strFileName, XMLoadFloat4x4(&m_PivotMatrix));

	if (nullptr == pModel)
		return E_FAIL;

	wstring strFinalFolderPath = strFolderPath + szFileName;

	XMStoreFloat4x4(&pModel->m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix));

	if (FAILED(Import_Mesh(strFinalFolderPath, pModel, eType)))
	{
		MSG_BOX("Import_Mesh Failed.");
		Safe_Release(pModel);
		return E_FAIL;
	}

	if (FAILED(Import_Material(strFinalFolderPath, strFolderPath, pModel)))
	{
		MSG_BOX("Import_Material Failed.");
		Safe_Release(pModel);
		return E_FAIL;
	}

	if (CModel::TYPE::TYPE_NONANIM == pModel->Get_ModelType())
	{
		if (FAILED(GI->Add_Prototype(iLevelIndex, strProtoTypeTag, pModel)))
		{
			MSG_BOX("Model Add Prototype Failed : Model Manager");
			Safe_Release(pModel);
			return E_FAIL;
		}
	}
	else
	{
		if (FAILED(Import_Animation(strFinalFolderPath, pModel)))
		{
			MSG_BOX("Import_Animation Failed.");
			Safe_Release(pModel);
			return E_FAIL;
		}

		if (FAILED(GI->Add_Prototype(iLevelIndex, strProtoTypeTag, pModel)))
		{
			MSG_BOX("Model Add Prototype Failed : Model Manager");
			Safe_Release(pModel);
			return E_FAIL;
		}
	}

	pModel->Set_Name(strFileName);

	if (CModel::TYPE::TYPE_ANIM == pModel->Get_ModelType())
	{
		if (FAILED(Create_Model_Vtf(pModel, strFinalFolderPath)))
			return E_FAIL;

		/*if (FAILED(Load_Model_Vtf(pModel, strFinalFolderPath)))
			return E_FAIL;*/

		/*if (FAILED(pModel->Clear_NotUsedData()))
			return E_FAIL;*/ 
			
			// 이거 살리면 CAnimation::Initialize(CModel* pModel)의 첫 주석도 풀어야함 리턴 하게끔
			// 이 클래스에 Save_Model_Vtf도 내부 필터 제거

		if (FAILED(Import_Socket(strFinalFolderPath, pModel)))
			return E_FAIL;

		if (FAILED(Import_Animation_KeyFrameSpeed(strFinalFolderPath, pModel)))
			return E_FAIL;

		if (FAILED(Import_Animation_Events(strFinalFolderPath, pModel)))
			return E_FAIL;
	}

	if (ppOut != nullptr)
	{
		*ppOut = pModel;
		if (*ppOut == nullptr)
		{
			MSG_BOX("ppOut Initialize Failed. : Model Manager");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CModel_Manager::Import_Model_Data(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut)
{
	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
 	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	const _bool bFbx = (0 == lstrcmp(szExt, L".fbx")) ? TRUE : FALSE;
	const _bool bToolLevel = (99 == GI->Get_CurrentLevel()) ? TRUE : FALSE;

	if (bFbx)
	{
		if (FAILED(Import_Model_Data_From_Fbx(iLevelIndex, strProtoTypeTag, eType, strFolderPath, strFileName, ppOut)))
			return E_FAIL;
	}
	else
	{
		if (bToolLevel)
		{
			if (FAILED(Import_Model_Data_From_Bin_In_Tool(iLevelIndex, strProtoTypeTag, eType, strFolderPath, strFileName, ppOut)))
				return E_FAIL;
		}
		else
		{
			if (FAILED(Import_Model_Data_From_Bin_In_Game(iLevelIndex, strProtoTypeTag, eType, strFolderPath, strFileName, ppOut)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CModel_Manager::Ready_Model_Data_FromPath(_uint iLevelIndex, _uint eType, const wstring& strFolderPath)
{
	for (auto& p : std::filesystem::directory_iterator(strFolderPath))
	{
		if (p.is_directory())
		{
			Ready_Model_Data_FromPath(iLevelIndex, eType, p.path().wstring());
			continue;
		}
		
		wstring strFilePath = CUtils::PathToWString(p.path().wstring());

		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		
		if (0 == lstrcmp(TEXT(".fbx"), strExt) || 0 == lstrcmp(TEXT(".mesh"), strExt))
		{
			if (FAILED(Import_Model_Data(iLevelIndex, wstring(L"Prototype_Component_Model_") + strFileName, eType, strFolderPath + L"/", wstring(strFileName) + strExt, nullptr)))
				return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CModel_Manager::Export_Mesh(const wstring& strFinalFolderPath, CModel* pModel)
{
	if (pModel == nullptr)
		return E_FAIL;

	wstring strMeshFilePath = strFinalFolderPath + L".mesh";
	auto path = filesystem::path(strMeshFilePath);

	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMeshFilePath, FileMode::Write);

	// Start
	File->Write<_uint>(_uint(pModel->m_Animations.size()));

	File->Write<_uint>(_uint(pModel->m_HierarchyNodes.size()));
	for (auto& Node : pModel->m_HierarchyNodes)
	{
		File->Write<_uint>(Node->m_iDepth);
		File->Write<string>(CUtils::ToString(Node->m_strName));
		File->Write<string>(CUtils::ToString(Node->Get_ParentName()));

		// Warning ! Origin Save, To Load Transform
		File->Write<_float4x4>(Node->m_OriginTransformation);
		File->Write<_float4x4>(Node->m_OffsetMatrix);
	}

	File->Write<_uint>(pModel->m_iNumMeshes);
	
	for (auto& Mesh : pModel->m_Meshes)
	{
		File->Write<string>(CUtils::ToString(Mesh->m_strName));
		File->Write<_uint>(Mesh->m_iMaterialIndex);
		File->Write<_uint>(Mesh->m_iNumBones);
		File->Write<_uint>(Mesh->m_iNumVertexBuffers);
		File->Write<_uint>(Mesh->m_iNumVertices);
		File->Write<_uint>(Mesh->m_iStride);
		File->Write<_uint>(Mesh->m_iNumPrimitives);
		File->Write<_uint>(Mesh->m_iIndexSizeofPrimitive);
		File->Write<_uint>(Mesh->m_iNumIndicesofPrimitive);
		File->Write<D3D11_PRIMITIVE_TOPOLOGY>(Mesh->m_eTopology);
		File->Write<DXGI_FORMAT>(Mesh->m_eIndexFormat);

		for (auto& Bone : Mesh->m_Bones)
		{
			File->Write<string>(CUtils::ToString(Bone->m_strName));
		}


		File->Write<_uint>((_uint)Mesh->m_FaceIndices.size());
		for (auto Indices : Mesh->m_FaceIndices)
			File->Write(&Indices, sizeof(FACEINDICES32));


		if (pModel->m_eModelType == CModel::TYPE::TYPE_ANIM)
		{
			for (auto& Vertex : Mesh->m_AnimVertices)
				File->Write(&Vertex, sizeof(VTXANIMMODEL));
		}
		else
		{
			for (auto& Vertex : Mesh->m_NonAnimVertices)
				File->Write(&Vertex, sizeof(VTXMODEL));
		}
	}

	return S_OK;
}

HRESULT CModel_Manager::Export_Material(const wstring& strFinalFolderPath, CModel* pModel)
{
	if (pModel == nullptr)
		return E_FAIL;

	wstring strMaterialFilePath = strFinalFolderPath + L".xml";
	auto path = filesystem::path(strMaterialFilePath);
	filesystem::create_directories(path.parent_path());
	
	shared_ptr<tinyxml2::XMLDocument> Document = make_shared<tinyxml2::XMLDocument>();
	tinyxml2::XMLDeclaration* Decl = Document->NewDeclaration();
	Document->LinkEndChild(Decl);

	tinyxml2::XMLElement* Root = Document->NewElement("Materials");
	Document->LinkEndChild(Root);

	for (auto& Material : pModel->m_Materials)
	{
		tinyxml2::XMLElement* Node = Document->NewElement("Material");
		Root->LinkEndChild(Node);

		tinyxml2::XMLElement* Element = nullptr;

		Element = Document->NewElement("Name");
		Element->SetText(CUtils::ToString(Material.strName).c_str());
		Node->LinkEndChild(Element);

		Element = Document->NewElement("Diffuse_Texture");
		Element->SetText(Export_Texture(pModel->m_strFolderPath, 
										path.parent_path().string(), 
										Material.pTexture[aiTextureType_DIFFUSE]).c_str());
		Node->LinkEndChild(Element);

		Element = Document->NewElement("Specular_Texture");
		Element->SetText(Export_Texture(pModel->m_strFolderPath, 
										path.parent_path().string(), 
										Material.pTexture[aiTextureType_SPECULAR]).c_str());
		Node->LinkEndChild(Element);

		Element = Document->NewElement("Normal_Texture");
		Element->SetText(Export_Texture(pModel->m_strFolderPath, 
										path.parent_path().string(), 
										Material.pTexture[aiTextureType_NORMALS]).c_str());
		Node->LinkEndChild(Element);

		Element = Document->NewElement("Emissive_Texture");
		Element->SetText(Export_Texture(pModel->m_strFolderPath, 
										path.parent_path().string(), 
										Material.pTexture[aiTextureType_EMISSIVE]).c_str());
		Node->LinkEndChild(Element);

	}

	Document->SaveFile(CUtils::ToString(strMaterialFilePath).c_str());
	

	return S_OK;
}

HRESULT CModel_Manager::Export_Animation(const wstring& strFinalFolderPath, CModel* pModel)
{
	if (pModel == nullptr)
		return E_FAIL;

	wstring strAnimationFilePath = strFinalFolderPath + L".anim";
	auto path = filesystem::path(strAnimationFilePath);
	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strAnimationFilePath, FileMode::Write);

	File->Write<_uint>(_uint(pModel->m_Animations.size()));
	for (auto& Animation : pModel->m_Animations)
	{
		_bool b;
		File->Write<string>(CUtils::ToString(Animation->m_strName));
		File->Write<_float>(Animation->m_fDuration);
		File->Write<_float>(Animation->m_fTickPerSecond);
		File->Write<_float>(Animation->m_fOriginSpeed);
		File->Write<_bool>(b);
		File->Write<_bool>(Animation->m_bLoop);
		File->Write<_bool>(b);
		File->Write<_uint>(Animation->m_iNumChannels);

		for (auto& Channel : Animation->m_Channels)
		{
			File->Write<string>(CUtils::ToString(Channel->m_strName));
			File->Write<_uint>(Channel->m_iNumKeyFrames);
			for (_uint i = 0; i < Channel->m_iNumKeyFrames; ++i)
			{
				File->Write(&Channel->m_KeyFrames[i], sizeof(KEYFRAME));
			}
		}
	}
	return S_OK;
}

HRESULT CModel_Manager::Export_Socket(const wstring& strFinalFolderPath, CModel* pModel)
{
	if (pModel == nullptr)
		return E_FAIL;

	wstring strSocketFilePath = strFinalFolderPath + L".socket";
	auto path = filesystem::path(strSocketFilePath);
	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strSocketFilePath, FileMode::Write);
	
	vector<_uint> SocketIndices = pModel->m_SocketTransformIndexCache;
	vector<Vec3> SocketCustomPivotRotaiton = pModel->m_SocketCustomPivotRotation;
	vector<Vec3> SocketCustomPivotPosition = pModel->m_SocketCustomPivotPosition;

	File->Write<_uint>(_uint(SocketIndices.size()));

	for (size_t i = 0; i < SocketIndices.size(); i++)
	{
		File->Write<_uint>(SocketIndices[i]);
		File->Write<Vec3>(SocketCustomPivotRotaiton[i]);
		File->Write<Vec3>(SocketCustomPivotPosition[i]);
	}

	return S_OK;
}

HRESULT CModel_Manager::Export_Animation_KeyFrameSpeed(const wstring& strFinalPath, CModel* pModel)
{
	if (pModel == nullptr)
		return E_FAIL;

	wstring strSocketFilePath = strFinalPath + L".KeyFrameSpeed";
	auto path = filesystem::path(strSocketFilePath);
	filesystem::create_directories(path.parent_path());

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strSocketFilePath, FileMode::Write);

	/* 전체 애니메이션 갯수*/
	vector<CAnimation*> Anims = pModel->Get_Animations();
	const _uint iAnimCount = Anims.size();
	File->Write<_uint>(iAnimCount);
	for (size_t i = 0; i < iAnimCount; i++)
	{
		if (nullptr != Anims[i])
		{
			vector<ANIM_SPEED_DESC> Descs = Anims[i]->Get_SpeedDescs();

			/* 현재 애니메이션의 스피드 컨테이너 소유 유무*/
			const _bool bHave = !Descs.empty();
			File->Write<_bool>(bHave);

			if (bHave)
			{
				/* 스피드 컨테이너 원소 갯수 */
				File->Write<_uint>(Descs.size());
				for (auto& desc : Descs)
				{
					/* 원소 값 */
					File->Write<_float>(desc.fStartFrame);
					File->Write<_float>(desc.fEndFrame);
					File->Write<_float>(desc.fStartSpeed);
					File->Write<_float>(desc.fEndSpeed);
				}
			}
		}
	}

	return S_OK;
}

HRESULT CModel_Manager::Export_Animation_Events(const wstring& strFinalPath, CModel* pModel)
{
	if (nullptr == pModel)
		return E_FAIL;

	Json json;
	vector<CAnimation*>	Anims = pModel->Get_Animations();
	const _uint			iAnimCount = Anims.size();

	for (size_t i = 0; i < iAnimCount; i++)
	{
		if (nullptr != Anims[i])
		{
			/* Sound */
			Anims[i]->Sort_SoundEvents();

			const vector<pair<_float, ANIM_EVENT_SOUND_DESC>>& SountEvents = Anims[i]->Get_SoundEvents();

			for (auto& SoundEvent : SountEvents)
			{
				json["Sound Event"].push_back({
					{"Animation Name", CUtils::ToString(Anims[i]->Get_AnimationName())},
					{"KeyFrame", SoundEvent.first},
					{"Sound Key", CUtils::TCharToString(SoundEvent.second.pSoundKey)},
					{"Channel ID", (_uint)(SoundEvent.second.iChannelID)},
					{"Volume", SoundEvent.second.fVolume },
					{"Stop", SoundEvent.second.bStop}
					});

			}
		}
	}

	json.dump(2);
	
	GI->Json_Save(strFinalPath + L".json", json);
	return S_OK;
}

string CModel_Manager::Export_Texture(const wstring& strOriginFolder, const string& strSaveFolder, CTexture* pTexture, _uint iIdx)
{
	if (pTexture == nullptr)
		return "";

	string TextureName = CUtils::ToString(pTexture->Get_Name(iIdx));
	
	string OriginTextureFilePath = CUtils::ToString(strOriginFolder) + TextureName;
	string SaveTextureFilePath = strSaveFolder + "/" + TextureName;

	CopyFileA(OriginTextureFilePath.c_str(), SaveTextureFilePath.c_str(), false);

	return TextureName;
}

HRESULT CModel_Manager::Create_AnimationTransform_Caches(const _uint& iAnimIndex)
{
	/* 현재 애니메이션에 대한 텍스처 한 장(프레임 행, 본 열)정보를 세팅한다. */
	CAnimation* pAnimation = m_AnimationsCache[iAnimIndex];

	const _uint iMaxFrame = (_uint)pAnimation->Get_MaxFrameCount();

	/* 모든 프레임 순회 (텍스처 가로) */
	wstring strAnimationName = pAnimation->Get_AnimationName();
	for (uint32 iFrameIndex = 0; iFrameIndex < iMaxFrame; iFrameIndex++)
	{
		/* Fit */
		{
			vector<Matrix> vecMat;
			m_AnimTransformsCaches[iAnimIndex].transforms.push_back(vecMat);
		}

		/* 모든 채널의 현재 프레임 갱신 */
		pAnimation->Calculate_Animation(iFrameIndex);

		/* 모든 본 글로벌 변환 -> 애니메이션 변환 */

		for (uint32 iBoneIndex = 0; iBoneIndex < m_HierarchyNodes.size(); iBoneIndex++)
		{
			m_HierarchyNodes[iBoneIndex]->Set_CombinedTransformation();

			/* Fit */
			{
				m_AnimTransformsCaches[iAnimIndex].transforms[iFrameIndex].push_back(Matrix());
			}

			m_AnimTransformsCaches[iAnimIndex].transforms[iFrameIndex][iBoneIndex]
					= Matrix(m_HierarchyNodes[iBoneIndex]->Get_OffSetMatrix())
					* Matrix(m_HierarchyNodes[iBoneIndex]->Get_CombinedTransformation()) 
					* Matrix(m_PivotMatrix);
		}
	}

	return S_OK;
}

vector<ANIM_TRANSFORM_CACHES> CModel_Manager::Create_AnimationTransform_Caches_InTool(CModel* pModel)
{
	/* 툴에서 디버깅 하기 위한 용도의 트랜스폼 맵, 어디에도 저장 되지 않고 툴레벨 클래스 객체에 저장된다. */
	vector<ANIM_TRANSFORM_CACHES> AnimTransformsCache;

	/* 툴 레벨이 아니라면 리턴 */
	if (99 != GI->Get_CurrentLevel() || nullptr == pModel)
		return AnimTransformsCache;

	m_HierarchyNodes = pModel->Get_HierarchyNodes();
	m_AnimationsCache = pModel->Get_Animations();

	const _uint iAnimCount = pModel->Get_Animations().size();

	AnimTransformsCache.resize(iAnimCount);

	for (size_t iAnimIndex = 0; iAnimIndex < iAnimCount; iAnimIndex++)
	{
		CAnimation* pAnimation = m_AnimationsCache[iAnimIndex];

		const _uint iMaxFrame = (_uint)pAnimation->Get_MaxFrameCount();

		for (uint32 iFrameIndex = 0; iFrameIndex < iMaxFrame; iFrameIndex++)
		{
			/* Fit */
			{
				vector<Matrix> vecMat;
				AnimTransformsCache[iAnimIndex].transforms.push_back(vecMat);
			}

			pAnimation->Calculate_Animation(iFrameIndex);

			for (uint32 iBoneIndex = 0; iBoneIndex < m_HierarchyNodes.size(); iBoneIndex++)
			{
				m_HierarchyNodes[iBoneIndex]->Set_CombinedTransformation();

				/* Fit */
				{
					AnimTransformsCache[iAnimIndex].transforms[iFrameIndex].push_back(Matrix());
				}


				AnimTransformsCache[iAnimIndex].transforms[iFrameIndex][iBoneIndex]
					= Matrix(m_HierarchyNodes[iBoneIndex]->Get_OffSetMatrix())
					* Matrix(m_HierarchyNodes[iBoneIndex]->Get_CombinedTransformation())
					* Matrix(m_PivotMatrix);
			}
		}
	}

	return AnimTransformsCache;
}

vector<ANIM_TRANSFORM_CACHE> CModel_Manager::Create_AnimationSocketTransform(class CModel* pModel, const _uint& iSocketBoneIndex)
{
	/* 툴레벨이 아닌데 이전에 저장된 데이터가 있는 경우 찾아서 리턴 */
	if (99 != GI->Get_CurrentLevel() && Find_AnimationSocketTransform(pModel->Get_Name(), iSocketBoneIndex))
	{
		return Get_AnimationSocketTransform(pModel->Get_Name(), iSocketBoneIndex);
	}

	m_HierarchyNodes = pModel->Get_HierarchyNodes();
	m_AnimationsCache = pModel->Get_Animations();

	/* 툴레벨이거나 혹은, 툴레벨이 아닌데 이전에 저장된 데이터가 없는 경우 새로 만든다. */

	vector<ANIM_TRANSFORM_CACHE> AnimTransformsCache;

	if (nullptr == pModel)
		return AnimTransformsCache;

	const _uint iAnimCount = pModel->Get_Animations().size();
	
	AnimTransformsCache.resize(iAnimCount);

	for (size_t iAnimIndex = 0; iAnimIndex < iAnimCount; iAnimIndex++)
	{
		CAnimation* pAnimation = m_AnimationsCache[iAnimIndex];

		const _uint iMaxFrame = (_uint)pAnimation->Get_MaxFrameCount();

		for (uint32 iFrameIndex = 0; iFrameIndex < iMaxFrame; iFrameIndex++)
		{
			/* Fit */
			{
				vector<Matrix> vecMat;
				AnimTransformsCache[iAnimIndex].transforms.push_back(vecMat);
			}

			pAnimation->Calculate_Animation(iFrameIndex);

			for (uint32 iBoneIndex = 0; iBoneIndex < m_HierarchyNodes.size(); iBoneIndex++)
			{
				m_HierarchyNodes[iBoneIndex]->Set_CombinedTransformation();

				if (iSocketBoneIndex == iBoneIndex)
				{
					AnimTransformsCache[iAnimIndex].transforms[iFrameIndex].push_back(Matrix());
					AnimTransformsCache[iAnimIndex].transforms[iFrameIndex][0]
						= Matrix(m_HierarchyNodes[iBoneIndex]->Get_OffSetMatrix())
						* Matrix(m_HierarchyNodes[iBoneIndex]->Get_CombinedTransformation())
						* Matrix(m_PivotMatrix);
				}
			}
		}
	}

	return AnimTransformsCache;
}

vector<ANIM_TRANSFORM_CACHE> CModel_Manager::Get_AnimationSocketTransform(const wstring strModelName, const _uint& iSocketBoneIndex)
{
	vector<ANIM_TRANSFORM_CACHE> SocketTransforms;

	auto	iterModel = m_SocketTransforms.find(strModelName);

	if (iterModel == m_SocketTransforms.end())
		return SocketTransforms;

	auto iterSocket = iterModel->second.find(iSocketBoneIndex);

	if (iterSocket == iterModel->second.end())
		return SocketTransforms;

	SocketTransforms.resize(iterModel->second.size());

	memcpy(&SocketTransforms, &(iterSocket->second), sizeof(iterSocket->second));

	return SocketTransforms;
}

const _bool	CModel_Manager::Find_AnimationSocketTransform(const wstring strModelName, const _uint& iSocketBoneIndex)
{
	/* 해당 모델이 저장되어 있는지 확인. */
	auto	iterModel = m_SocketTransforms.find(strModelName);

	if (iterModel == m_SocketTransforms.end())
		return false;

	/* 해당 소켓이 저장되어 있는지 확인. */
	auto iterSocket = iterModel->second.find(iSocketBoneIndex);

	if (iterSocket == iterModel->second.end())
		return false;

	return true;
}

HRESULT CModel_Manager::Import_Mesh(const wstring strFinalPath, CModel* pModel, _uint eModelType)
{
	wstring strMeshFilePath = strFinalPath + L".mesh";
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMeshFilePath, FileMode::Read);

	_uint iAnimationCount = File->Read<_uint>();

	pModel->m_eModelType = CModel::TYPE(eModelType);
	// pModel->m_eModelType = iAnimationCount > 0 ? CModel::TYPE::TYPE_ANIM : CModel::TYPE::TYPE_NONANIM;
	pModel->m_iNumAnimations = iAnimationCount;

	/* 뼈 만들기 */
	_uint iNumHierarchyNodes = File->Read<_uint>(); 
	for (_uint i = 0; i < iNumHierarchyNodes; ++i)
	{
		CHierarchyNode* pNode = CHierarchyNode::Create_Bin();
	
		File->Read<_uint>(pNode->m_iDepth);
		pNode->m_strName =CUtils::ToWString(File->Read<string>());
		pNode->m_strParentName =CUtils::ToWString(File->Read<string>());
		// Warning ! Load Orgin Transformation to m_Transfomation
		File->Read<_float4x4>(pNode->m_Transformation);
		File->Read<_float4x4>(pNode->m_OffsetMatrix);

		pModel->m_HierarchyNodes.push_back(pNode);
	}


	// 부모를 찾아 링크.
	for (auto& Node : pModel->m_HierarchyNodes)
		Node->Initialize_Bin(pModel);

	/* 메시 데이터 읽기 */
	pModel->m_iNumMeshes = File->Read<_uint>();
	for (_uint i = 0; i < pModel->m_iNumMeshes; ++i)
	{
		CMesh* Mesh = CMesh::Create_Bin(m_pDevice, m_pContext, CModel::TYPE(eModelType), XMLoadFloat4x4(&pModel->m_PivotMatrix));
		
		Mesh->m_strName =CUtils::ToWString(File->Read<string>());
		File->Read<_uint>(Mesh->m_iMaterialIndex);
		File->Read<_uint>(Mesh->m_iNumBones);
		File->Read<_uint>(Mesh->m_iNumVertexBuffers);
		File->Read<_uint>(Mesh->m_iNumVertices);
		File->Read<_uint>(Mesh->m_iStride);
		File->Read<_uint>(Mesh->m_iNumPrimitives);
		File->Read<_uint>(Mesh->m_iIndexSizeofPrimitive);
		File->Read<_uint>(Mesh->m_iNumIndicesofPrimitive);
		File->Read<D3D11_PRIMITIVE_TOPOLOGY>(Mesh->m_eTopology);
		File->Read<DXGI_FORMAT>(Mesh->m_eIndexFormat);

		vector<wstring> BoneNames(Mesh->m_iNumBones);
		for (_uint j = 0; j < Mesh->m_iNumBones; ++j)		
			BoneNames[j] =CUtils::ToWString(File->Read<string>());

		_uint iIndiceSize = File->Read<_uint>();
		Mesh->m_FaceIndices.reserve(iIndiceSize);
		for (_uint j = 0; j < iIndiceSize; ++j)
		{
			FACEINDICES32 Index;
			File->Read<FACEINDICES32>(Index);
			Mesh->m_FaceIndices.push_back(Index);
		}

		if (pModel->m_eModelType == CModel::TYPE::TYPE_ANIM)
		{
			Mesh->m_AnimVertices.reserve(Mesh->m_iNumVertices);
			for (_uint j = 0; j < Mesh->m_iNumVertices; ++j)
			{
				VTXANIMMODEL Vertex;
				ZeroMemory(&Vertex, sizeof(VTXANIMMODEL));
				File->Read<VTXANIMMODEL>(Vertex);

				Mesh->m_AnimVertices.push_back(Vertex);
			}
			if(FAILED(Mesh->Ready_Bin_AnimVertices()))
				return E_FAIL;
		}
		else
		{
			Mesh->m_NonAnimVertices.reserve(Mesh->m_iNumVertices);
			for (_uint j = 0; j < Mesh->m_iNumVertices; ++j)
			{
				VTXMODEL Vertex;
				ZeroMemory(&Vertex, sizeof(VTXMODEL));
				File->Read<VTXMODEL>(Vertex);

				Mesh->m_NonAnimVertices.push_back(Vertex);
			}
			if (FAILED(Mesh->Ready_Bin_Vertices()))
				return E_FAIL;
		}

		Mesh->Initialize_Bin(pModel, BoneNames);
		pModel->m_Meshes.push_back(Mesh);
	}

	return S_OK;
}

HRESULT CModel_Manager::Import_Material(const wstring strFinalPath, const wstring strFolderPath, CModel* pModel)
{
	if (pModel == nullptr)
		return E_FAIL;

	wstring strMaterialFilePath = strFinalPath + L".xml";

	tinyxml2::XMLDocument* Document = new tinyxml2::XMLDocument();
	tinyxml2::XMLError error = Document->LoadFile(CUtils::ToString(strMaterialFilePath).c_str());

	if (error != tinyxml2::XML_SUCCESS)
		return E_FAIL;

	tinyxml2::XMLElement* Root = Document->FirstChildElement();
	tinyxml2::XMLElement* MaterialNode = Root->FirstChildElement();

	while (MaterialNode)
	{
		MATERIALDESC MaterialDesc;
		ZeroMemory(&MaterialDesc, sizeof(MATERIALDESC));

		tinyxml2::XMLElement* Node = nullptr;
		Node = MaterialNode->FirstChildElement();

		lstrcpy(MaterialDesc.strName, CUtils::ToWString(Node->GetText()).c_str());


		// Diffuse Texture
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture =CUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				MaterialDesc.pTexture[aiTextureType_DIFFUSE] = CTexture::Create(m_pDevice, m_pContext, strFolderPath + strTexture);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_DIFFUSE])
					return E_FAIL;
			}
		}

		// Specular
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture =CUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				MaterialDesc.pTexture[aiTextureType_SPECULAR] = CTexture::Create(m_pDevice, m_pContext, strFolderPath + strTexture);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_SPECULAR])
					return E_FAIL;
			}
		}


		// Normal
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture =CUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				MaterialDesc.pTexture[aiTextureType_NORMALS] = CTexture::Create(m_pDevice, m_pContext, strFolderPath + strTexture);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_NORMALS])
					return E_FAIL;
			}
		}

		// Emissive
		Node = Node->NextSiblingElement();
		if (Node->GetText())
		{
			wstring strTexture =CUtils::ToWString(Node->GetText());
			if (strTexture.length() > 0)
			{
				MaterialDesc.pTexture[aiTextureType_EMISSIVE] = CTexture::Create(m_pDevice, m_pContext, strFolderPath + strTexture);
				if (nullptr == MaterialDesc.pTexture[aiTextureType_EMISSIVE])
					return E_FAIL;
			}
		}
		
		pModel->m_Materials.push_back(MaterialDesc);
		MaterialNode = MaterialNode->NextSiblingElement();
	}

	pModel->m_iNumMaterials = pModel->m_Materials.size();
	Safe_Delete(Document);
	return S_OK;
}

HRESULT CModel_Manager::Import_Animation(const wstring strFinalPath, CModel* pModel)
{
	//230926 TODO : Import_Animation
	if (pModel == nullptr)
		return E_FAIL;

	wstring strAnimationFilePath = strFinalPath + L".anim";
	auto path = filesystem::path(strAnimationFilePath);
	

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strAnimationFilePath, FileMode::Read);

	File->Read<_uint>(pModel->m_iNumAnimations);
	for (_uint i = 0; i < pModel->m_iNumAnimations; ++i)
	{
		CAnimation* pAnimation = CAnimation::Create_Bin();

		pAnimation->m_strName = CUtils::ToWString(File->Read<string>());
		File->Read<_float>(pAnimation->m_fDuration);
		File->Read<_float>(pAnimation->m_fTickPerSecond);
		File->Read<_float>(pAnimation->m_fOriginSpeed);

		_bool b;
		File->Read<_bool>(b);
		File->Read<_bool>(pAnimation->m_bLoop);
		File->Read<_bool>(b);

		File->Read<_uint>(pAnimation->m_iNumChannels);
		for (_uint j = 0; j < pAnimation->m_iNumChannels; ++j)
		{
			CChannel* pChannel = CChannel::Create_Bin();

			pChannel->m_strName = CUtils::ToWString(File->Read<string>());
			File->Read<_uint>(pChannel->m_iNumKeyFrames);

			for (_uint k = 0; k < pChannel->m_iNumKeyFrames; ++k)
			{
				KEYFRAME tKeyFrame;
				ZeroMemory(&tKeyFrame, sizeof(KEYFRAME));

				File->Read<KEYFRAME>(tKeyFrame);

				pChannel->m_KeyFrames.push_back(tKeyFrame);
			}
			pAnimation->m_Channels.push_back(pChannel);
		}
		if (FAILED(pAnimation->Initialize(pModel)))
			return E_FAIL;

		pModel->m_Animations.push_back(pAnimation);
	}

	//if (FAILED(pModel->Ready_Animation_Texture()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CModel_Manager::Import_Animation_KeyFrameSpeed(const wstring strFinalPath, CModel* pModel)
{
	if (nullptr == pModel)
		return E_FAIL;

	wstring strAnimationFilePath = strFinalPath + L".KeyFrameSpeed";
	auto path = filesystem::path(strAnimationFilePath);

	if (!filesystem::exists(path))
		return S_OK;

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strAnimationFilePath, FileMode::Read);


	/* 전체 애니메이션 갯수 */
	_uint iAnimCunt = 0;
	File->Read<_uint>(iAnimCunt);

	for (size_t i = 0; i < iAnimCunt; i++)
	{
		CAnimation* pAnim = pModel->Get_Animation(i);
		
		if (nullptr == pAnim)
			continue;
		
		/* 스피드 컨테이너 소유 유무*/
		_bool bHave = FALSE;
		File->Read<_bool>(bHave);
		if (bHave)
		{
			/* 스피드 컨테이너 원소 갯수*/
			_uint descCount = 0;
			File->Read<_uint>(descCount);
			for (size_t k = 0; k < descCount; k++)
			{
				ANIM_SPEED_DESC desc;
				File->Read<_float>(desc.fStartFrame);
				File->Read<_float>(desc.fEndFrame);
				File->Read<_float>(desc.fStartSpeed);
				File->Read<_float>(desc.fEndSpeed);

				pAnim->Add_SpeedDesc(desc);
			}
		}
	}

	return S_OK;
}

HRESULT CModel_Manager::Import_Animation_Events(const wstring strFinalPath, CModel* pModel)
{
	if (nullptr == pModel)
		return E_FAIL;

	wstring strAnimationFilePath = strFinalPath + L".json";
	auto path = filesystem::path(strAnimationFilePath);

	if (!filesystem::exists(path))
		return S_OK;

	Json json = GI->Json_Load(strAnimationFilePath);


	for (const auto& item : json["Sound Event"]) 
	{
		string strAnimName;
		string strSoundFileKey;
		ANIM_EVENT_SOUND_DESC desc;
		
		strAnimName = item["Animation Name"];
		strSoundFileKey = item["Sound Key"];
		desc.iChannelID = item["Channel ID"];
		desc.fVolume = item["Volume"];
		desc.bStop = item["Stop"];
		_float fKeyFrame = item["KeyFrame"];

		desc.pSoundKey = CSound_Manager::GetInstance()->Get_SoundFileKey(strSoundFileKey);
		if (nullptr != desc.pSoundKey)
		{
			CAnimation* pAnim = pModel->Get_Animation(strAnimName);
			if (nullptr != pAnim)
			{
				pAnim->Add_SoundEvent(fKeyFrame, desc);
			}
		}
	}
	return S_OK;
}

HRESULT CModel_Manager::Import_Socket(const wstring strFinalPath, CModel* pModel)
{
	if (nullptr == pModel)
		return E_FAIL;

	wstring strAnimationFilePath = strFinalPath + L".socket";
	auto path = filesystem::path(strAnimationFilePath);

	if (!filesystem::exists(path))
		return S_OK;

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strAnimationFilePath, FileMode::Read);

	_uint iSocketCount;
	File->Read<_uint>(iSocketCount);

	for (size_t i = 0; i < iSocketCount; i++)
	{
		_uint iSocketIndex;
		File->Read<_uint>(iSocketIndex);

		Vec3 vRot, vPos;
		File->Read<Vec3>(vRot);
		File->Read<Vec3>(vPos);

		pModel->Add_SocketTransformIndexCache(iSocketIndex);
		pModel->Add_CustomSocketPivotRotation(vRot);
		pModel->Add_CustomSocketPivotPosition(vPos);

		if(99 != GI->Get_CurrentLevel())
			pModel->Add_SocketTransforms(GI->Create_AnimationSocketTransform(pModel, iSocketCount));
	}

	return S_OK;
}

void CModel_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_VtfTextures)
		Safe_Release(Pair.second);

	m_VtfTextures.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
