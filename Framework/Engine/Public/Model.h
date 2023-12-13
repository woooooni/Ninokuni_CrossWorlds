#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	/* Model Prop */
	const wstring& Get_Name() const { return m_strName; }
	void Set_Name(const wstring& strName) { m_strName = strName; }
	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	TYPE Get_ModelType() { return m_eModelType; }

	/* HierarchyNode */
	class CHierarchyNode* Get_HierarchyNode(const wstring & strNodeName);
	const _int Get_HierarchyNodeIndex(const char* szBonename);
	vector<class CHierarchyNode*>& Get_HierarchyNodes() { return m_HierarchyNodes; }

	/* Meshes */
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	const vector<class CMesh*>& Get_Meshes() { return m_Meshes; }

	/* Matertial */
	_uint Get_MaterialIndex(_uint iMeshIndex);
	class CTexture* Get_MaterialTexture(_uint iMeshIndex, _uint iTextureType);

	/* Animation */
	const TweenDesc& Get_TweenDesc() const { return m_TweenDesc; }
	_uint Get_CurrAnimationIndex() { return m_TweenDesc.cur.iAnimIndex; }
	class CAnimation* Get_CurrAnimation() { return m_Animations[m_TweenDesc.cur.iAnimIndex]; }
	HRESULT Set_Animation(const _uint& iAnimationIndex, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION);
	HRESULT Set_Animation(const wstring& strAnimationName, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION);

public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix); // << : Assimp 
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Bin(void* pArg);

	HRESULT LateTick(_float fTimeDelta); /* 모델의 애니메이션 키프레임 업데이트*/

public:
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	HRESULT Render_Instancing(class CShader* pShader, _uint iMeshIndex, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, _uint iPassIndex = 0);

public:
	/* ImGui Tool */
	const aiScene* Get_Scene() { return m_pAIScene; }
	HRESULT Swap_Animation(_uint iSrcIndex, _uint iDestIndex);
	HRESULT Delete_Animation(_uint iIndex);
	vector<class CAnimation*>& Get_Animations() { return m_Animations; }
	_int Find_AnimationIndex(const wstring& strAnimationTag);

public:
	/* Vtf */
	HRESULT Set_VtfSrv(ID3D11ShaderResourceView* pSrv);
	HRESULT Clear_NotUsedData();

private:
	wstring m_strName;
	wstring m_strFileName;
	wstring m_strFolderPath;

private:
	const aiScene* m_pAIScene = nullptr;
	Assimp::Importer m_Importer;

	_float4x4 m_PivotMatrix;
	TYPE m_eModelType = TYPE_END;
	_bool m_bFromBinary = false;

private:
	vector<class CHierarchyNode*> m_HierarchyNodes;

	_uint m_iNumMeshes = 0;
	vector<class CMesh*> m_Meshes;
	typedef vector<class CMesh*> MESHES;

	_uint m_iNumMaterials = 0;
	vector<MATERIALDESC> m_Materials;

	vector<class CAnimation*> m_Animations;
	_uint m_iNumAnimations = 0;

private:
	ID3D11Texture2D* m_pMatrixTexture = nullptr;
	vector<_float4x4> m_Matrices;


	ID3D11ShaderResourceView*	m_pSRV = nullptr;
	TWEEN_DESC					m_TweenDesc = {};

private:
	/* Assimp */
	HRESULT Ready_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Ready_Materials(const wstring & pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode * pNode, class CHierarchyNode* pParent, _uint iDepth);
	HRESULT Ready_Animations();
	HRESULT Ready_Animation_Texture();

private:
	static CModel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	static CModel* Create_Bin(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());

public:
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	friend class CModel_Manager;
};



END