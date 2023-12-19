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

	typedef struct tagEffectDesc
	{	
		// 이펙트 타입
		EFFECT_TYPE eType = EFFECT_TYPE::EFFECT_MESH;

		_bool bLoop    = true;
		_bool bGravity = false;
		_float4x4 OffsetMatrix;

#pragma region 크기
		_float3			vScaleDir   = _float3(0.f, 0.f, 0.f);
		_float			fScaleSpeed = 0.f;
#pragma endregion

#pragma region 이동
		_float3			vMoveDir = _float3(0.f, 0.f, 0.f);
		_float			fMoveSpeed = 0.f;
#pragma endregion

#pragma region 회전
		_bool	bBillboard = false;
		_float3 vTurnDir = _float3(0.f, 1.f, 0.f);
		_float	fTurnSpeed = 0.f;
#pragma endregion

		// 지속 시간
		_float fLifeTime = 1.f;

#pragma region 모델 && 텍스처
		wstring strModelName            = L"Prototype_Component_Model_PS_NOTE_CIRCLE01";
		wstring strDiffuseTetextureName = L"Prototype_Component_Texture_Effect_SubUV";
		wstring strAlphaTexturName      = L"";

		_int    iTextureIndexDiffuse = 0;
		_int    iTextureIndexAlpha   = 0;

		// Sequencetexture Effect
		_float2	vUVIndex  = _float2(0.f, 0.f);
		_float2	fMaxCount = _float2(1.f, 1.f);

		_int	bCutUV = -1;
		_float2	vUVFlow = { 0.f, 0.f };
		_float2 fAccUVFlow = _float2(0.f, 0.f);
#pragma endregion

#pragma region 애니메이션
		_bool  bIncrement  = true;
		_float fIndexSpeed = 20.f;
#pragma endregion

#pragma region 알파
		_float fAlpha = 1.f;
		_float fDestAlphaSpeed = 0.f;
#pragma endregion

#pragma region 색상
		_float4	vDiffuseColor         = _float4(0.f, 0.f, 0.f, 0.f);
		_float4	vAdditiveDiffuseColor = _float4(0.f, 0.f, 0.f, 0.f);
#pragma endregion

#pragma region 블러
		_float	fBlurPower  = 0.01f;
		_float3	vBloomPower = { 0.01f, 0.01f, 0.01f };
#pragma endregion

#pragma region 기타 정보
		_uint   iShaderPass = 0;
#pragma endregion

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
			bCutUV = rhs.bCutUV;

			fTurnSpeed = rhs.fTurnSpeed;
			fMoveSpeed = rhs.fMoveSpeed;

			// Sequencetexture Effect
			fMaxCount = rhs.fMaxCount;

			fAlpha = rhs.fAlpha;
			fDestAlphaSpeed = rhs.fDestAlphaSpeed;

			fIndexSpeed = rhs.fIndexSpeed;
			vUVFlow = rhs.vUVFlow;

			fBlurPower = rhs.fBlurPower;
			vBloomPower = rhs.vBloomPower;

			vDiffuseColor = rhs.vDiffuseColor;
			vAdditiveDiffuseColor = rhs.vAdditiveDiffuseColor;

			vScaleDir = rhs.vScaleDir;
			fScaleSpeed = rhs.fScaleSpeed;


			// Translation
			vMoveDir = rhs.vMoveDir;
			vTurnDir = rhs.vTurnDir;

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
	void Reset_Effect();
	void Reset_UV();

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
	_bool   m_bEnd = false;
	_float  m_fAccDeletionTime = 0.f;
	_float  m_fAccIndex        = 0.f;

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


