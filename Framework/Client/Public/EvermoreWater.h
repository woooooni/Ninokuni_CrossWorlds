#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CTexture;
class CRenderer;
class CTransform;
class CNavigation;
class CVIBuffer_Ocean;
END

BEGIN(Client)

class CEvermoreWater final : public CGameObject
{
private:
	enum TEX_TYPE { TEX_DIFFUSE, TEX_NOISE1, TEX_NOISE2, TEX_NORMAL, TEX_END };
private:
	struct VS_OceanBuffer
	{
		_float fWaveFrequency = 0.05f; // 파장
		_float fWaveAmplitude = 1.0f; // 진폭 위 아래
		_float fBumpHeight = 0.6f; // 잔 물결 높이
		_float fpadding2 = 0.0f; 


		Vec2 vBumpSpeed = Vec2(0.0f, 0.05f); // 잔 물결 속도
		Vec2 fTextureScale = Vec2(14.0f, 14.0f);
	};

	struct PS_OceanBuffer
	{
		_float fFresnelBias = 0.8f; // 편향
		_float fFresnelPower = 0.5f; // 강도
		_float fFresnelAmount = 0.5f; // 보정값
		_float fShoreBlend = 35.0f; // 알파값

		_float fHeightRatio = 7.5f; // 높이 보정값
		_float fpadding3 = 0.0f;
		Vec2 vOceanSize = Vec2(64.0f * 8.0f, 64.0f * 8.0f); // 버텍스 전체 크기

		Vec4 vShallowColor = Vec4(0.7f, 0.85f, 0.8f, 1.0f); // 얕은 색
		Vec4 vDeepColor = Vec4(0.2f, 0.5f, 0.95f, 1.0f); // 깊은 색
	};
private:
	explicit CEvermoreWater(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEvermoreWater(const CGameObject& rhs);
	virtual ~CEvermoreWater() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private: /* 해당 객체가 사용해야할 컴포넌트들을 저장하낟. */
	CRenderer* m_pRendererCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TEX_TYPE::TEX_END] = {};
	CModel* m_pModelCom = nullptr;
	//CVIBuffer_Ocean* m_pVIBufferCom = { nullptr };
private:
	VS_OceanBuffer m_vsOceanBuffer = {};
	PS_OceanBuffer m_psOceanBuffer = {};
private:
	// Water State
	_float m_fReflectRefractSacle = 0.1f;
	_float m_fWaterTranslationSpeed = 0.015f;

	_float m_fTime = 0.0f;

	Vec2 m_vDirection;

	_bool m_bFresnel = false;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CEvermoreWater* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override; /* 사본객체를 생성할때 원본데이터로부터 복제해올 데이터외에 더 추가해 줘야할 데이터가 있다라면 받아오겠다. */
	virtual void Free() override;
};

END

