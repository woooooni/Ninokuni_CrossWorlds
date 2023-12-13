#pragma once

#include "Component.h"

BEGIN(Engine)

#define DEFAULT_TWEEN_DURATION	0.2f

class ENGINE_DLL CModel final : public CComponent
{
public:
	enum TYPE { TYPE_NONANIM, TYPE_ANIM, TYPE_END };

	typedef struct	KeyframeDesc
	{
		_int	iAnimIndex	= -1;
		_uint	iCurFrame	= 0;
		_uint	iNextFrame	= 1;
		_float	fRatio		= 0.f;
		_float	fFrameAcc	= 0.f;

		void ClearAnim()
		{
			iCurFrame		= 0;
			iNextFrame		= 1;
			fRatio			= 0.f;
			fFrameAcc		= 0.f;
		}

	}KEYFRAME_DESC;

	typedef struct	TweenDesc
	{
		KEYFRAME_DESC cur	= {};
		KEYFRAME_DESC next	= {};

		_float fTweenDuration	= DEFAULT_TWEEN_DURATION;
		_float fTweenRatio		= 0.f;
		_float fTweenAcc		= 0.f;
		_float fPadding			= 0.f;

		TweenDesc()
		{
			cur.iAnimIndex	= -1;
			next.iAnimIndex = -1;
		}

		void ClearNextAnim()
		{
			next.iAnimIndex = -1;
			next.iCurFrame	= 0;
			next.iNextFrame = 1;

			fTweenAcc		= 0.f;
			fTweenRatio		= 0.f;
			fTweenDuration	= DEFAULT_TWEEN_DURATION;
		}

	}TWEEN_DESC;

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& rhs);
	virtual ~CModel() = default;

public:
	const _int Get_HierarchyNodeIndex(const char* szBonename);
	class CHierarchyNode* Get_HierarchyNode(const wstring & strNodeName);

	_uint Get_NumMeshes() const { return m_iNumMeshes; }
	const vector<class CMesh*>& Get_Meshes() { return m_Meshes; }

	_uint Get_MaterialIndex(_uint iMeshIndex);
	class CTexture* Get_MaterialTexture(_uint iMeshIndex, _uint iTextureType);


	// << : 추가
	const wstring& Get_Name() const { return m_strName; }
	void Set_Name(const wstring& strName) { m_strName = strName; }
	const TweenDesc& Get_TweenDesc() const { return m_TweenDesc; }

	HRESULT Set_Animation(const _uint& iAnimationIndex, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION);
	HRESULT Set_Animation(const wstring& strAnimationName, const _float& fTweenDuration = DEFAULT_TWEEN_DURATION);
	// >> 

	HRESULT Set_Animation(const wstring & strAnimationName);
	void Set_AnimIndex(_uint iAnimIndex);

	void Complete_Interpolation();

	_matrix Get_PivotMatrix() { return XMLoadFloat4x4(&m_PivotMatrix); }

	TYPE Get_ModelType() { return m_eModelType; }

	vector<class CHierarchyNode*>& Get_HierarchyNodes() { return m_HierarchyNodes; }

public:
	// << : Fbx 로드 
	virtual HRESULT Initialize_Prototype(TYPE eType, const wstring & strModelFilePath, const wstring & strModelFileName, _fmatrix PivotMatrix); // << : Assimp 
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT Initialize_Bin(void* pArg);

	// << : 추가
	HRESULT LateTick(_float fTimeDelta);

public:
	HRESULT SetUp_OnShader(class CShader* pShader, _uint iMaterialIndex, aiTextureType eTextureType, const char* pConstantName);
	HRESULT Play_Animation(class CTransform* pTransform, _float fTimeDelta);
	HRESULT Render(class CShader* pShader, _uint iMeshIndex, _uint iPassIndex = 0);
	HRESULT Render_Instancing(class CShader* pShader, _uint iMeshIndex, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, _uint iPassIndex = 0);


	HRESULT Swap_Animation(_uint iSrcIndex, _uint iDestIndex);
	HRESULT Delete_Animation(_uint iIndex);
	_bool Is_InterpolatingAnimation() { return m_bInterpolationAnimation; }
	_bool Is_Animation_Finished(_uint iAnimationIndex);

public:
	vector<class CAnimation*>& Get_Animations() { return m_Animations; }
	_int Find_AnimationIndex(const wstring& strAnimationTag);

	_uint Get_CurrAnimationIndex() { return m_iCurrentAnimIndex; }
	class CAnimation* Get_CurrAnimation() { return m_Animations[m_iCurrentAnimIndex]; }

	HRESULT Set_VtfSrv(ID3D11ShaderResourceView* pSrv);
	HRESULT Clear_NotUsedData();


public:
	const aiScene* Get_Scene() { return m_pAIScene; }

private:
	wstring m_strName;
	wstring m_strFileName;
	wstring m_strFolderPath;

private:
	const aiScene* m_pAIScene = nullptr;

	Assimp::Importer			m_Importer;

	_float4x4					m_PivotMatrix;
	TYPE						m_eModelType = TYPE_END;
	_bool						m_bFromBinary = false;

private:
	_uint							m_iNumMeshes = 0;
	vector<class CMesh*>			m_Meshes;
	typedef vector<class CMesh*>	MESHES;

private:
	_uint							m_iNumMaterials = 0;
	vector<MATERIALDESC>			m_Materials;

private:
	vector<class CHierarchyNode*>	m_HierarchyNodes;

private:
	_uint								m_iCurrentAnimIndex = 0;
	_int								m_iNextAnimIndex = -1;
	_bool								m_bInterpolationAnimation = false;
	_bool								m_bFirstRootConvert = false;
	_float4								m_vPreAnimPos = {0.f, 0.f, 0.f, 1.f };

	_uint								m_iNumAnimations = 0;
	vector<class CAnimation*>			m_Animations;


private:
	ID3D11Texture2D* m_pMatrixTexture = nullptr;
	ID3D11ShaderResourceView* m_pSRV = nullptr;
	vector<_float4x4> m_Matrices;


	TWEEN_DESC					m_TweenDesc = {};


private:
	// << : Assimp 
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

	friend class CModel_Manager;
};



END