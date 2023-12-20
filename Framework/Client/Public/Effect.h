#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CShader;
class CTexture;
class CRenderer;
class CPipeLine;
class CTransform;
class CRigidBody;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CEffect final : public CGameObject
{
public:
	enum EFFECT_TYPE { EFFECT_TEXTURE, EFFECT_MESH, EFFECT_END };

public:
	typedef struct tagEffectDesc // 반복X 자체적으로 계속 생성
	{	
		// 이펙트 타입
		EFFECT_TYPE eType = EFFECT_TYPE::EFFECT_MESH;

		// 중력 
		_bool bGravity = false; // ->

		// 위치
		_float3 fRange         = _float3(0.f, 0.f, 0.f);
		_float2 fRangeDistance = _float2(0.f, 0.f);

#pragma region 크기
		_bool   bScaleSameRate = true; // 정비율
		_float2 fScaleStart    = _float2(1.f, 1.f);

		_bool   bScaleChange           = false;
		_float2 fScaleChangeStartDelay = _float2(0.f, 0.f);

		_bool   bScaleChangeRandom = false;
		_float2 fScaleChangeTime   = _float2(1.f, 5.f);

		_float3	fScaleDir       = _float3(0.f, 0.f, 0.f);
		_bool   bScaleLoop      = false;
		_bool   bScaleLoopStart = false;

		_float2 fScaleMin   = _float2(1.f, 1.f);
		_float2 fScaleMax   = _float2(10.f, 10.f);
		_float	fScaleSpeed = 0.f;
#pragma endregion

#pragma region 이동
		_float3			fMoveDir   = _float3(0.f, 0.f, 0.f);
		_float			fMoveSpeed = 0.f;
#pragma endregion

#pragma region 회전
		_bool	bBillboard = false;

		_bool	bRotationChange = false;
		_float3 fTurnDir        = _float3(0.f, 1.f, 0.f);
		_float	fTurnSpeed      = 0.f;
#pragma endregion

		// 지속 시간
		_float fLifeTime = 10.f;

#pragma region 모델 && 텍스처
		wstring strModelName            = L"Prototype_Component_Model_PS_NOTE_CIRCLE01";
		wstring strDiffuseTetextureName = L"";
		wstring strAlphaTexturName      = L"Prototype_Component_Texture_Effect_Aura";

		_int    iTextureIndexDiffuse = 0;
		_int    iTextureIndexAlpha   = 0;

		// SequenceTexture : Diffuse && Alpha
		_bool   bRandomStartIndex = false;
		_float2	fUVIndex  = _float2(0.f, 0.f);
		_float2	fMaxCount = _float2(1.f, 1.f);

		// UVFlow
		_bool   bUVFlow = true;
		_int	iUVLoop = -1; // 0 < UVLoop -> NoLoop
		_float2	fUVFlow = _float2(0.f, 0.f);
#pragma endregion

#pragma region 애니메이션
		_bool  bAnimation  = false;
		_bool  bLoop       = true;
		_bool  bIncrement  = true;
		_float fIndexSpeed = 20.f;
#pragma endregion

#pragma region 알파
		_float2 fAlphaStart = _float2(1.f, 1.f);

		_bool bAlphaCreate = false;
		_bool bAlphaDelete = false;
		_float2 fAlphaSpeed = _float2(0.f, 0.f);

		_bool bAlphaChange = false;
		_bool bAlphaIn     = false;
		_float2 fAlphaChangeStartDelay = _float2(0.f, 0.f);
#pragma endregion

#pragma region 색상
		_bool   bColorRandom = false;
		_float4	vAdditiveDiffuseColor = _float4(0.f, 0.f, 0.f, 0.f);
#pragma endregion

#pragma region 블러
		_float	fBlurPower  = 0.01f;
		_float3	vBloomPower = { 0.01f, 0.01f, 0.01f };
#pragma endregion

#pragma region 기타 정보
		_uint   iShaderPass = 0;
#pragma endregion

		// 
		_float4x4 OffsetMatrix;

		tagEffectDesc()
		{
			XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());
		}

		// 복사 생성자 내용 수정 전부 추가
		tagEffectDesc(const tagEffectDesc& rhs)
		{
			eType = rhs.eType;

			strModelName            = rhs.strModelName;
			strDiffuseTetextureName = rhs.strDiffuseTetextureName;
			strAlphaTexturName      = rhs.strAlphaTexturName;

			bBillboard = rhs.bBillboard;
			iUVLoop = rhs.iUVLoop;

			fTurnSpeed = rhs.fTurnSpeed;
			fMoveSpeed = rhs.fMoveSpeed;

			// Sequencetexture Effect
			fMaxCount = rhs.fMaxCount;

			//fAlpha = rhs.fAlpha;
			//fDestAlphaSpeed = rhs.fDestAlphaSpeed;

			fIndexSpeed = rhs.fIndexSpeed;
			fUVFlow = rhs.fUVFlow;

			fBlurPower = rhs.fBlurPower;
			vBloomPower = rhs.vBloomPower;

			vAdditiveDiffuseColor = rhs.vAdditiveDiffuseColor;

			fScaleDir = rhs.fScaleDir;
			fScaleSpeed = rhs.fScaleSpeed;


			// Translation
			fMoveDir = rhs.fMoveDir;
			fTurnDir = rhs.fTurnDir;

			OffsetMatrix = rhs.OffsetMatrix;
		}

	} EFFECT_DESC;

protected:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize_Prototype(const EFFECT_DESC* tEffectDesc);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) override;

public:
	const EFFECT_DESC& Get_EffectDesc() { return m_tEffectDesc; }
	void Set_EffectDesc(const EFFECT_DESC& tDesc);
	void Reset_Effect();
	void Reset_UV();

public:
	void Set_Owner(CGameObject* pGameObject) { m_pOwnerObject = pGameObject; }
	void Set_OffsetMatrix(_fmatrix fOffsetMatrix) { XMStoreFloat4x4(&m_tEffectDesc.OffsetMatrix, fOffsetMatrix); }
	const _float4x4& Get_OffsetMatrix() { return  m_tEffectDesc.OffsetMatrix; }
	void Set_ParentMatrix(_matrix ParentMatrix) { XMStoreFloat4x4(&m_ParentMatrix, ParentMatrix); }

public:
	void Set_DeletionTime(_float fLifeTime) { m_tEffectDesc.fLifeTime = fLifeTime; }

	_bool Is_End() { return m_bEnd; };
	void Set_End(_bool bEnd) { m_bEnd = bEnd; }

	void Set_MoveDir(_vector vDir);
	void Set_MoveSpeed(_float fSpeed) { m_tEffectDesc.fMoveSpeed = fSpeed; }
	void Set_Gravity(_bool bGravity);

public:
	class CTexture* Get_DiffuseTexture() { return m_pDiffuseTextureCom; }
	class CTexture* Get_AlphaTexture()   { return m_pAlphaTextureCom; }
	class CTransform* Get_TransformCom() { return m_pTransformCom; }
	class CRigidBody* Get_RigidBodyCom() { return m_pRigidBodyCom; }

private:
	EFFECT_DESC m_tEffectDesc;

private:
	CGameObject* m_pOwnerObject = nullptr;
	_float4x4    m_ParentMatrix;

private:
	_bool   m_bEffectDie = false;
	_bool   m_bEnd       = false;

	_float  m_fAccDeletionTime = 0.f;

	_float  m_fAccIndex        = 0.f;
	_float2 m_fAccUVFlow       = _float2(0.f, 0.f);

	// 알파
	_float m_fAlpha      = 1.f;
	_float m_fAlphaSpeed = 0.f;
	_float m_fAlphaChangeStartTime  = 0.f;
	_float m_fAlphaChangeStartDelay = 0.f;
	_bool  m_bAlphaCreateSucc       = false;

private:
	class CRenderer*  m_pRendererCom  = nullptr;
	class CTransform* m_pTransformCom = nullptr;
	class CRigidBody* m_pRigidBodyCom = nullptr;
	class CModel*         m_pModelCom = nullptr;
	class CVIBuffer_Rect* m_pVIBufferCom = nullptr;
	class CTexture* m_pDiffuseTextureCom = nullptr;
	class CTexture* m_pAlphaTextureCom = nullptr;

private:
	HRESULT Bind_ShaderResource();
	HRESULT Bind_ShaderResource_Instance(class CShader* pShader);

	/* For.Texture_Effect */
	virtual void Increment(_float fTimeDelta);
	virtual void Decrement(_float fTimeDelta);

	/* Alpha */
	void Change_Alpha(_float fTimeDelta);

	void Set_Model();
	void Set_Texture_Diffuse();
	void Set_Texture_Alpha();

protected:
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Ready_Components() override;

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, 
		const wstring& strObjectTag, const EFFECT_DESC* tEffectDesc = nullptr, const wstring& strEffectFilePath = {});
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END


