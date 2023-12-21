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

#pragma region Access Methods 
public: 
	/* Model Prop */
	void Set_Name(const wstring& strName) { m_strName = strName; }
	void Set_CustomSocketPivotRotation(const _uint iIndex, Vec3 vCustomSocket);

	TYPE Get_ModelType() { return m_eModelType; }
	Vec3 Get_CustomSocketPivotRotation(const _uint iIndex); /* 파츠에서 사용하는 커스텀 피벗 매트릭스 */
	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }
	const wstring& Get_Name() const { return m_strName; }

	void Add_CustomSocketPivotRotation(Vec3 vCustomSocket) { m_SocketCustomPivotRotation.push_back(vCustomSocket); }

	/* HierarchyNode */
	Matrix Get_SocketLocalMatrix(const _uint iSocketEnumIndex); /* (모델의 Latetick 이후 호출)캐릭터가 갖고 있는 파츠의 소켓 매트릭스를 리턴한다. (캐릭터에 정의된 enum 인덱스 사용, 뼈 번호 아님) */
	wstring Get_HiearachyNodeName(const _uint iIndex);
	const _int Get_HierarchyNodeIndex(const char* szBonename);
	const _int Get_HierarchyNodeIndex(wstring strBoneName);
	class CHierarchyNode* Get_HierarchyNode(const wstring & strNodeName);
	class CHierarchyNode* Get_HierarchyNode(const _uint iIndex);
	vector<class CHierarchyNode*>& Get_HierarchyNodes() { return m_HierarchyNodes; }

	/* Meshes */
	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	const vector<class CMesh*>& Get_Meshes() { return m_Meshes; }

	/* Matertial */
	_uint Get_MaterialIndex(_uint iMeshIndex);
	class CTexture* Get_MaterialTexture(_uint iMeshIndex, _uint iTextureType);

	/* Animation */
	const _bool Has_Animation(const wstring& strAnimationName);
	
	void Set_Stop_Animation(const _bool& bStop) { m_TweenDesc.cur.iStop = bStop; } /* bStop이 트루라면 모델 레이트틱이 호출되더라도 애니메이션이 갱신되지 않는다. */
	void Set_KeyFrame_By_Progress(_float fProgress); /* 진행률(0 ~ 1)을 매개변수로 받아 키프레임을 세팅한다. */
	HRESULT Set_Animation(const _uint& iAnimationIndex, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION); /* 인덱스로 애니메이션 플레이 (fTweenDuration이 음수라면 보간 X) */
	HRESULT Set_Animation(const wstring& strAnimationName, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION); /* 이름으로 애니메이션 플레이 (fTweenDuration이 음수라면 보간 X) */
	
	_uint Get_CurrAnimationIndex() { return m_TweenDesc.cur.iAnimIndex; } /* 현재 애니메이션의 인덱스 리턴*/
	class CAnimation* Get_Animation(const _uint& iIndex);
	class CAnimation* Get_Animation(const string strName);
	const _int Get_AnimationIndex(const wstring& strName);
	const _uint& Get_CurrAnimationFrame() const { return m_TweenDesc.cur.iCurFrame; } /* 현재 애니메이션의 프레임 리턴 (정수 형태) */
	const _float Get_CurrAnimationFrame_WithRatio() { return (m_TweenDesc.cur.iCurFrame + m_TweenDesc.cur.fRatio); } /* 현재 애니메이션의 프레임(보간 포함) (실수 형태) */
	const _float Get_Progress();  /* 현재 애니메이션의 진행률(0~1) 리턴*/
	const TweenDesc& Get_TweenDesc() const { return m_TweenDesc; } /* 현재, 다음 애니메이션 정보 구조체 리턴 */
	class CAnimation* Get_CurrAnimation() { return m_Animations[m_TweenDesc.cur.iAnimIndex]; } /* 현재 애니메이션 객체 리턴 */
	vector<class CAnimation*>& Get_Animations() { return m_Animations; } /* 전체 애니메이션 컨테이너 리턴*/

	_int Find_AnimationIndex(const wstring& strAnimationTag); /* 이름을 키로 사용해서 애니메이션의 인덱스 리턴 */

	const _bool Is_Half() const { return (0.5f <= m_TweenDesc.cur.fRatio) ? true : false; } /* 현재 애니메이션이 반 이상 진행됐는지 여부 리턴 */
	const _bool Is_Finish() const { return m_TweenDesc.cur.iFinish; } /* 현재 애니메이션이 종료됐는지 리턴 (종료되었는데 만약 다음 애니메이션이 세팅 안되어 있다면 종료 상태 유지) */
	const _bool Is_Tween() const { return (0 <= m_TweenDesc.next.iAnimIndex) ? true : false; } /* 애니메이션 트위닝 (다음 애니메이션 보간) 여부 리턴 */
	const _bool Is_Fix() const { return m_TweenDesc.cur.iFix; } /* 현재 애니메이션이 마지막 프레임에서 고정 상태인지 여부 리턴*/
	const _bool Is_Stop() const { return m_TweenDesc.cur.iStop; } /* 현재 애니메이션 정지 여부 */

	void Debug_Animation();
#pragma endregion

#pragma region Life Cycle
public:
	virtual HRESULT Initialize_Prototype(TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix); // << : Assimp 
	virtual HRESULT Initialize_Prototype_Part(const wstring& strModelFilePath, const wstring& strModelFileName, _fmatrix PivotMatrix, class CModel* pCharacterModel); // << : Assimp 
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Bin(void* pArg);

	HRESULT LateTick(_float fTimeDelta); /* 모델의 애니메이션 키프레임 업데이트 (수업 코드에서의 PlayAnimation() 함수?) */

	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	HRESULT SetUp_VTF(class CShader* pShader);
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	HRESULT Render_Instancing(class CShader* pShader, _uint iMeshIndex, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, _uint iPassIndex = 0);
#pragma endregion

#pragma region ImGui Tool
public:
	const aiScene* Get_Scene() { return m_pAIScene; }
	
	void Add_SocketTransforms(vector<ANIM_TRANSFORM_CACHE> SocketTransforms) { m_SocketTransforms.push_back(SocketTransforms); }

	void Clear_SocketTransformsCache(const _uint iSocketIndex);
	void Clear_All_SocketTransformsCaches();
	void Add_SocketTransformIndexCache(_uint iIndex) { m_SocketTransformIndexCache.push_back(iIndex); }
	vector<_uint> Get_SocketTransformIndexCache() { return m_SocketTransformIndexCache; }

	HRESULT Delete_Animation(_uint iIndex);
	HRESULT Swap_Animation(_uint iSrcIndex, _uint iDestIndex);

#pragma endregion

#pragma region Vtf
public:
	HRESULT Set_VtfSrv(ID3D11ShaderResourceView* pSrv);
	HRESULT Clear_NotUsedData();
#pragma endregion

private:
	wstring m_strName;
	wstring m_strFileName;
	wstring m_strFolderPath;

private:
	const aiScene* m_pAIScene = nullptr;
	Assimp::Importer m_Importer;

	TYPE m_eModelType = TYPE_END;
	_bool m_bFromBinary = false;
	_float4x4 m_PivotMatrix;

private:
	vector<class CHierarchyNode*> m_HierarchyNodes;

	_uint m_iNumMeshes = 0;
	vector<class CMesh*> m_Meshes;
	typedef vector<class CMesh*> MESHES;

	_uint m_iNumMaterials = 0;
	vector<MATERIALDESC> m_Materials;

	_uint m_iNumAnimations = 0;
	vector<class CAnimation*> m_Animations;

private:
	TWEEN_DESC m_TweenDesc = {};
	ID3D11ShaderResourceView* m_pSRV = nullptr;

	vector<_uint> m_SocketTransformIndexCache;
	vector<Vec3> m_SocketCustomPivotRotation;
	vector<vector<ANIM_TRANSFORM_CACHE>> m_SocketTransforms; /* 소켓(뼈) 별로 vector<ANIM_TRANSOFRM_CACHE>를 갖는다.*/

#pragma region Assimp
private:
	HRESULT Ready_MeshContainers(_fmatrix PivotMatrix);
	HRESULT Ready_MeshContainers_Part(_fmatrix PivotMatrix, class CModel* pCharacterModel);
	HRESULT Ready_Materials(const wstring & pModelFilePath);
	HRESULT Ready_HierarchyNodes(aiNode * pNode, class CHierarchyNode* pParent, _uint iDepth);
	HRESULT Ready_Animations();
#pragma endregion

private:
	static CModel* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	static CModel* Create_Bin(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix = XMMatrixIdentity());
	static CModel* Create_Part(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType, const wstring& strModelFilePath, const wstring& strModelFileName, CModel* pCharacterModel, _fmatrix PivotMatrix = XMMatrixIdentity());

public:
	virtual CComponent* Clone(void* pArg = nullptr);
	virtual void Free() override;

public:
	friend class CModel_Manager;
};



END