#pragma once

#include "Base.h"

BEGIN(Engine)

class CModel_Manager final : public CBase
{
	DECLARE_SINGLETON(CModel_Manager);

public:
	CModel_Manager();
	virtual ~CModel_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	HRESULT Ready_Model_Data_FromPath(_uint iLevelIndex, _uint eType, const wstring& strFolderPath);
	
public:
	HRESULT Import_Model_Data(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut = nullptr);
	HRESULT Export_Model_Data(class CModel* pModel, const wstring& strSubFolderName, const wstring& strFileName);
	HRESULT Export_Model_Data_FromPath(_uint eType, wstring strFolderPath); 
	
private:
	HRESULT Import_Mesh(const wstring strFinalPath, class CModel* pModel);
	HRESULT Import_Material(const wstring strFinalPath, const wstring strFolderPath, class CModel* pModel);
	HRESULT Import_Animation(const wstring strFinalPath, class CModel* pModel);

private:
	HRESULT Export_Mesh(const wstring& strFinalPath, class CModel* pModel);
	HRESULT Export_Material(const wstring& strFinalPath, class CModel* pModel);
	HRESULT Export_Animation(const wstring& strFinalPath, class CModel* pModel);
	string Export_Texture(const wstring& strOriginFolder, const string& strSaveFolder, class CTexture* pTexture, _uint iIdx = 0);

public: 
	HRESULT Create_Model_Vtf(class CModel* pModel, const wstring strFilePath);
	HRESULT Save_Model_Vtf(const wstring strSaveFilePath, ID3D11Texture2D* pTexture); 
	HRESULT Load_Model_Vtf(class CModel* pModel, const wstring strLoadFilePath);
	ID3D11ShaderResourceView* Find_Model_Vtf(const wstring strModelName);

public:
	vector<ANIM_TRANSFORM_CACHE> Calculate_AnimationTransform_Cache(class CModel* pModel, const _uint& iSocketBoneIndex); /* 소켓 본을 계산하기 위한 캐시 (다수 채널) */

private:
	HRESULT Import_Model_Data_From_Fbx(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut = nullptr);
	HRESULT Import_Model_Data_From_Bin_In_Tool(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut = nullptr);
	HRESULT Import_Model_Data_From_Bin_In_Game(_uint iLevelIndex, const wstring& strProtoTypeTag, _uint eType, wstring strFolderPath, wstring strFileName, __out class CModel** ppOut = nullptr);
	HRESULT Create_AnimationTransform_Caches(const _uint& iAnimIndex); /* VTF 텍스처를 만들기 위한 캐시 (다수 채널) */

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

	wstring m_strExportFolderPath = L"../Bin/Export/";
	_float4x4 m_PivotMatrix;

	map<wstring, ID3D11ShaderResourceView*> m_VtfTextures;

	vector<ANIM_TRANSFORM_CACHES> m_AnimTransformsCaches; 
	vector<class CHierarchyNode*> m_HierarchyNodes;
	vector<class CAnimation*> m_AnimationsCache;

public:
	virtual void Free() override;
};

END