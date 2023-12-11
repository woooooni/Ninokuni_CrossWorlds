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

	// << : VTF
	//if (FAILED(Create_Model_Vtf(pModel)))
	//	return E_FAIL;

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

HRESULT CModel_Manager::Create_Model_Vtf(class CModel* pModel)
{
	if (nullptr == pModel || CModel::TYPE::TYPE_ANIM != pModel->Get_ModelType())
		return E_FAIL;

	/* 01. For m_AnimTransforms */
	/* 해당 모델이 사용하는 모든 애니메이션과 Bone의 정보를 m_AnimTransforms에 세팅한다. */
	m_AnimationsCache = pModel->Get_Animations();
	m_HierarchyNodes = pModel->Get_HierarchyNodes();

	const _uint iAnimCount = (_uint)m_AnimationsCache.size();
	const _uint iHiearachynodeCount = (_uint)m_HierarchyNodes.size();

	_uint iAnimMaxFrameCount = 0;

	for (uint32 i = 0; i < iAnimCount; i++)
	{
		_uint iCurAnimFrameCnt = (_uint)m_AnimationsCache[i]->Get_Duration();

		iAnimMaxFrameCount = iAnimMaxFrameCount < iCurAnimFrameCnt ? iCurAnimFrameCnt : iAnimMaxFrameCount;
	}

	if (0 == iAnimMaxFrameCount) 
		return E_FAIL;

	m_AnimTransformsCache.resize(iAnimCount);

	for (_uint i = 0; i < iAnimCount; i++)
	{
		if (FAILED(Create_AnimationTransform(i)))
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

			for (uint32 f = 0; f < iAnimMaxFrameCount; f++) /* 키프레임 갯수만큼 반복 (세로 크기만큼) */
			{
				void* ptr = pageStartPtr + dataSize * f;

				::memcpy(ptr, m_AnimTransformsCache[c].transforms[f].data(), dataSize); /* 텍스처에 가로 1줄만큼 데이터 저장 */

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

	/* 04 For. Set Texture To Model */
	/*if (FAILED(pModel->Set_VtfTexture(pSrv)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CModel_Manager::Import_Model_Vtf()
{
	return S_OK;
}

ID3D11ShaderResourceView* CModel_Manager::Get_Model_Vtf(const wstring strKey)
{
	return nullptr;
}

#pragma region Import_ModelData

/* 모델 컴포넌트를 생성해 컴포넌트 매니저에 프로토타입 등록시킨다. */
HRESULT CModel_Manager::Import_Model_Data(_uint iLevelIndex, 
	const wstring& strProtoTypeTag, 
	_uint eType, 
	wstring strFolderPath, 
	wstring strFileName,
	__out class CModel** ppOut)
{
	_tchar szFileName[MAX_PATH];
	_tchar szExt[MAX_PATH];
	_wsplitpath_s(strFileName.c_str(), nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

	CModel* pModel = nullptr;

	if (0 == lstrcmp(szExt, L".fbx"))
	{
 		pModel = CModel::Create(
			m_pDevice, 
			m_pContext, CModel::TYPE(eType), 
			strFolderPath, strFileName, XMLoadFloat4x4(&m_PivotMatrix));

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
	}
	else 
	{
		pModel = CModel::Create_Bin(
			m_pDevice,
			m_pContext, CModel::TYPE(eType),
			strFolderPath, strFileName, XMLoadFloat4x4(&m_PivotMatrix));

		if (nullptr == pModel)
			return E_FAIL;

		wstring strFinalFolderPath = strFolderPath + szFileName;

		XMStoreFloat4x4(&pModel->m_PivotMatrix, XMLoadFloat4x4(&m_PivotMatrix));

		if (FAILED(Import_Mesh(strFinalFolderPath, pModel)))
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
#pragma endregion

#pragma region Export_Mesh
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
#pragma endregion

#pragma region Export_Material

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
#pragma endregion

#pragma region Export_Animation
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
		File->Write<string>(CUtils::ToString(Animation->m_strName));
		File->Write<_float>(Animation->m_fDuration);
		File->Write<_float>(Animation->m_fTickPerSecond);
		File->Write<_float>(Animation->m_fSpeed);
		File->Write<_bool>(Animation->m_bRootAnimation);
		File->Write<_bool>(Animation->m_bLoop);
		File->Write<_bool>(Animation->m_bTweeningAnim);
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

#pragma endregion


#pragma region Export_Texture
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

HRESULT CModel_Manager::Create_AnimationTransform(const _uint& iAnimIndex)
{
	/* 현재 애니메이션에 대한 텍스처 한 장(프레임 행, 본 열)정보를 세팅한다. */
	CAnimation* pAnimation = m_AnimationsCache[iAnimIndex];

	const _uint iMaxFrame = (_uint)pAnimation->Get_Duration();

	/* 모든 프레임 순회 (텍스처 가로) */
	for (uint32 iFrameIndex = 0; iFrameIndex < iMaxFrame; iFrameIndex++)
	{
		/* 모든 채널의 현재 프레임 갱신 */
		pAnimation->Calculate_Animation(iFrameIndex);

		/* 모든 본 글로벌 변환 -> 애니메이션 변환 */

		for (uint32 iBoneIndex = 0; iBoneIndex < m_HierarchyNodes.size(); iBoneIndex++)
		{
			m_HierarchyNodes[iBoneIndex]->Set_CombinedTransformation();

			m_AnimTransformsCache[iAnimIndex].transforms[iFrameIndex][iBoneIndex]
				= Matrix(m_HierarchyNodes[iBoneIndex]->Get_OffSetMatrix())
					* Matrix(m_HierarchyNodes[iBoneIndex]->Get_CombinedTransformation()) 
					* Matrix(m_PivotMatrix);

			/* 여기서 원래 개별 보관 뼈 저장 (ex 루트, 무기 뼈) */
		}
	}

	return S_OK;
}

#pragma endregion

#pragma region Import_Mesh
HRESULT CModel_Manager::Import_Mesh(const wstring strFinalPath, CModel* pModel)
{
	wstring strMeshFilePath = strFinalPath + L".mesh";
	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMeshFilePath, FileMode::Read);

	_uint iAnimationCount = File->Read<_uint>();

	pModel->m_eModelType = iAnimationCount > 0 ? CModel::TYPE::TYPE_ANIM : CModel::TYPE::TYPE_NONANIM;
	pModel->m_iNumAnimations = iAnimationCount;

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


	pModel->m_iNumMeshes = File->Read<_uint>();
	for (_uint i = 0; i < pModel->m_iNumMeshes; ++i)
	{
		
		CMesh* Mesh = CMesh::Create_Bin(m_pDevice, m_pContext, pModel->m_eModelType, XMLoadFloat4x4(&pModel->m_PivotMatrix));
		
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
#pragma endregion


#pragma region Import_Material
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
#pragma endregion

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
		File->Read<_float>(pAnimation->m_fSpeed);
		File->Read<_bool>(pAnimation->m_bRootAnimation);
		File->Read<_bool>(pAnimation->m_bLoop);
		File->Read<_bool>(pAnimation->m_bTweeningAnim);

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

	if (FAILED(pModel->Ready_Animation_Texture()))
		return E_FAIL;

	return S_OK;
}



void CModel_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
