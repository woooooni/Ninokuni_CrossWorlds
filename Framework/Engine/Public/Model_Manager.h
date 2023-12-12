#pragma once

#include "Base.h"

BEGIN(Engine)

#define MAX_MODEL_CHANNELS	300 
#define MAX_MODEL_KEYFRAMES		300 

class CModel_Manager final : public CBase
{
	DECLARE_SINGLETON(CModel_Manager);

	typedef struct	AnimTransformCache /* Vtf 텍스처를 만들기 위한 캐시 데이터 (툴에서만 사용) */
	{
		using TransformArrayType = array<Matrix, MAX_MODEL_CHANNELS>;
		array<TransformArrayType, MAX_MODEL_KEYFRAMES> transforms;

	}ANIM_TRANSFORM_CACHE;

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
	HRESULT Export_Model_Data_FromPath(_uint eType, wstring strFolderPath); /* 폴더 내의 모든 모델 바이너리화 */
	
public: /* VTF */
	HRESULT Create_Model_Vtf(class CModel* pModel);
	HRESULT Import_Model_Vtf();
	ID3D11ShaderResourceView* Get_Model_Vtf(const wstring strKey);

private:
	HRESULT Import_Mesh(const wstring strFinalPath, class CModel* pModel);
	HRESULT Import_Material(const wstring strFinalPath, const wstring strFolderPath, class CModel* pModel);
	HRESULT Import_Animation(const wstring strFinalPath, class CModel* pModel);

private:
	HRESULT Export_Mesh(const wstring& strFinalPath, class CModel* pModel);
	HRESULT Export_Material(const wstring& strFinalPath, class CModel* pModel);
	HRESULT Export_Animation(const wstring& strFinalPath, class CModel* pModel);
	string Export_Texture(const wstring& strOriginFolder, const string& strSaveFolder, class CTexture* pTexture, _uint iIdx = 0);

private:
	/* VTF */
	HRESULT Create_AnimationTransform(const _uint& iAnimIndex);

private:
	wstring m_strExportFolderPath = L"../Bin/Export/";

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	_float4x4 m_PivotMatrix;

private: /* VTF */
	map<wstring, ID3D11ShaderResourceView*> m_VtfTextures;

	/* Cache */
	vector<ANIM_TRANSFORM_CACHE> m_AnimTransformsCache;
	vector<class CHierarchyNode*> m_HierarchyNodes;
	vector<class CAnimation*> m_AnimationsCache;

public:
	virtual void Free() override;
};

END