#pragma once

#include "Component.h"

BEGIN(Engine)
class CModel;
class CVIBuffer_Instancing;

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {
		RENDER_PRIORITY, RENDER_NONLIGHT,
		RENDER_NONBLEND, RENDER_LIGHT, RENDER_SHADOW,
		RENDER_ALPHABLEND, RENDER_EFFECT,
		RENDER_UI, RENDER_UI_EFFECT_NONBLEND, RENDER_UI_EFFECT_BLEND,
		RENDER_CURSOR, RENDER_WATER,
		RENDER_END
	};

	enum RENDERER_SHADER_TYPE   { SHADER_DEFERRED, SHADER_OUTLINE, SHADER_BLUR, SHADER_SSAO, SHADER_FINAL, SHADER_END };
	enum INSTANCING_SHADER_TYPE { ANIM_MODEL, MODEL, RECT, EFFECT_TEXTURE, EFFECT_MODEL, TYPE_END };
	enum BLUR_PASS              { 
		BLUR_DOWN, BLUR_UP,
		BLUR_HOR_LOW,    BLUR_VER_LOW,
		BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE,
		BLUR_HOR_HIGH,   BLUR_VER_HIGH
	};

public:
	typedef struct tagEffectInstancingDesc
	{
		_float2	g_fUVIndex;  // 8
		_float2	g_fMaxCount; // 8

		_float2	g_fUVFlow; // 8
		_int	g_iUVLoop; // 4
		_float	g_fAlpha;  // 4

		_float4	g_fAdditiveDiffuseColor; // 16

		_float3	g_vBlurColor;            // 12
		_float  g_fBlurPower;            // 4
	} EFFECT_INSTANCE_DESC;

	typedef struct tagTextDesc
	{
		wstring strText;
		wstring strFontTag;
		_float2 vPosition;
		_float4 vColor = { 1.f, 1.f, 1.f, 1.f };
		_float  fAngle = 0.f;
		_float2 vOrigin = { 0.f, 0.f };
		_float2 vScale = { 1.f, 1.f };
	} TEXT_DESC;

private:
	typedef struct tagInstancingDesc
	{
		class CGameObject* pGameObject = { nullptr };

		vector<XMFLOAT4X4> WorldMatrices;
		vector<EFFECT_INSTANCE_DESC> EffectInstancingDesc;
		vector<TweenDesc> TweenDesc;
		INSTANCING_SHADER_TYPE eShaderType = INSTANCING_SHADER_TYPE::TYPE_END;
	}INSTANCING_DESC;

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderer(const CRenderer& rhs) = delete;
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Set_Fog(_float4 vFogColor, _float fFogStart, _float fFogEnd)
	{
		m_vFogColor    = vFogColor;
		m_vFogStartEnd = _float2(fFogStart, fFogEnd);
	}

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderGroup_Instancing(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, class CGameObject* pGameObject, _float4x4 WorldMatrix);
	HRESULT Add_RenderGroup_AnimInstancing(RENDERGROUP eRenderGroup, class CGameObject* pGameObject, _float4x4 WorldMatrix, const TweenDesc& TweenInstanceDesc);
	HRESULT Add_RenderGroup_Instancing_Effect(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, class CGameObject* pGameObject, _float4x4 WorldMatrix, const EFFECT_INSTANCE_DESC& EffectInstanceDesc);
	HRESULT Add_Text(const TEXT_DESC& TextDesc);
#ifdef _DEBUG
public:
	HRESULT Add_Debug(class CComponent* pDebug) {
		if (nullptr == pDebug)
			return E_FAIL;

		m_RenderDebug.push_back(pDebug);
		Safe_AddRef(pDebug);
		return S_OK;
	}

	void Set_PlayerPosition(_vector vPosition) { XMStoreFloat4(&m_vPlayerPosition, vPosition); }

#endif

public:
	HRESULT Draw();

private:
	HRESULT Render_Priority();
	HRESULT Render_NonLight();

	HRESULT Render_Shadow();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Lights();

	HRESULT Render_Ssao();
	HRESULT Render_OutLine();

	HRESULT Render_Deferred();

	HRESULT Render_AlphaBlend();
	HRESULT Render_Effect();

	HRESULT Render_UI();
	HRESULT Render_Text();
	HRESULT Render_UIEffectNonBlend();
	HRESULT Render_UIEffectBlend();

	HRESULT Render_Final();
	HRESULT Render_Cursor();

	// Blur
	HRESULT Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear, BLUR_PASS eHorizontalPass, BLUR_PASS eVerticalPass);
	HRESULT Render_BlurDownSample(const wstring& strStartTargetTag);
	HRESULT Render_Blur_Horizontal(BLUR_PASS eHorizontalPass);
	HRESULT Render_Blur_Vertical(BLUR_PASS eVerticalPass);
	HRESULT Render_BlurUpSample(const wstring& strFinalMrtTag, _bool bClear);

	// Mix
	HRESULT Render_AlphaBlendTargetMix(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear);

#ifdef _DEBUG
private:
	HRESULT Render_Debug();
#endif // DEBUG

private:
	HRESULT Create_Buffer();
	HRESULT Create_Shader();
	HRESULT Create_Target();
	HRESULT Set_TargetsMrt();
	HRESULT Initialize_SSAO();

#ifdef _DEBUG
	HRESULT Set_Debug();
#endif // DEBUG

private:
	list<class CGameObject*> m_RenderObjects[RENDER_END];
	list<TEXT_DESC>			 m_RenderTexts;
	list<class CComponent*>	 m_RenderDebug;
	map<wstring, INSTANCING_DESC> m_Render_Instancing_Objects[RENDER_END];

private:
	class CShader* m_pShaders[RENDERER_SHADER_TYPE::SHADER_END];
	class CShader* m_pIntancingShaders[INSTANCING_SHADER_TYPE::TYPE_END];

	class CVIBuffer_Rect*       m_pVIBuffer = { nullptr };
	class CVIBuffer_Instancing* m_pVIBuffer_Instancing = nullptr;
	_float4x4 m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

private:
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CLight_Manager*  m_pLight_Manager = { nullptr };

private: 
	_bool	m_bDebugDraw   = false;
	_bool   m_bOption      = false;
	// On/Off_Option
	_bool   m_bShadowDraw  = false;
	_bool   m_bSsaoDraw    = false;
	_bool   m_bOutlineDraw = false;
	_bool   m_bBlurDraw    = false;

	// 구현 필요
	_bool   m_bBlomDraw = false;
	_bool   m_bPbrDraw  = false;

private:
	_float4	m_vFogColor = {.5f, .5f, .5f, 1.f };
	_float2	m_vFogStartEnd = { 1000000.f, 1000000.f };


	_float	m_fBias = 0.2f;
	_float4	m_vPlayerPosition = {0.f, 0.f, 0.f, 1.f};
	
public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END