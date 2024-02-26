#pragma once

#include "Component.h"

BEGIN(Engine)
class CModel;
class CVIBuffer_Instancing;

class ENGINE_DLL CRenderer final : public CComponent
{
public: // enum
	enum RENDERGROUP {
		RENDER_PRIORITY, RENDER_AURORA, RENDER_NONLIGHT,
		RENDER_SHADOW, RENDER_NONBLEND,	RENDER_STENCIL_ONLY, RENDER_MIRROR, RENDER_REFLECT,
        RENDER_DECAL, RENDER_EFFECT, RENDER_LENSFLARE, RENDER_GODRAY, RENDER_ALPHABLEND,
		RENDER_CASCADE,
		RENDER_UI, 
		/*RENDER_SHADOW_UI,*/ RENDER_NONBLEND_UI, RENDER_UI_MINIMAP, RENDER_UI_MINIMAP_ICON,
		RENDER_UI_EFFECT_NONBLEND, RENDER_UI_EFFECT_BLEND,
		RENDER_CURSOR,

		RENDER_END
	};
	enum RENDERER_SHADER_TYPE   { SHADER_DEFERRED, SHADER_OUTLINE, SHADER_BLUR, SHADER_SSAO, SHADER_FINAL, SHADER_AURORA, SHADER_POSTPROCESS, SHADER_END };
	enum INSTANCING_SHADER_TYPE { ANIM_MODEL, MODEL, RECT, EFFECT_TEXTURE, EFFECT_MODEL, TYPE_END };

	enum BLUR_PASS              { 
		BLUR_DOWN, BLUR_UP_ONEMAX, BLUR_UP_ONEADD,
		BLUR_HOR_LOW,    BLUR_VER_LOW,
		BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE,
		BLUR_HOR_HIGH,   BLUR_VER_HIGH,
		BLUR_HOR_HIGHHIGH, BLUR_VER_HIGHHIGH,
		BLUR_ALL
	};

	enum SCREEN_EFFECT { SWORDMAN_SPLIT = 1, DESTROYER_BREAK, SCREENEFFECT_END };

	enum RENDER_SWITCH { GODRAY_SWITCH, UIMESH_SWITCH, SWITCH_END};

public: // struct
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

	typedef struct tagFogDesc
	{	
		_float fFogStartDepth = 100.f;
		_float fFogStartDistance = 10.f;
		_float fFogDistanceValue = 30.f;
		_float fFogHeightValue = 50.f;
		_float fFogDistanceDensity = 0.04f;
		_float fFogHeightDensity = 0.04f;

		_float2 fUVAcc = { 0.f, 0.f };
	} FOG_DESC;

private:
	typedef struct tagInstancingDesc
	{
		class CGameObject* pGameObject = { nullptr };

		vector<XMFLOAT4X4> WorldMatrices;
		vector<EFFECT_INSTANCE_DESC> EffectInstancingDesc;
		vector<TweenDesc> TweenDesc;
		vector<ANIMODEL_INSTANCE_DESC> AnimInstanceDesc;


		INSTANCING_SHADER_TYPE eShaderType = INSTANCING_SHADER_TYPE::TYPE_END;
	}INSTANCING_DESC;

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderer(const CRenderer& rhs) = delete;
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public: // RenderSwitch
	const _bool& Get_RenderSwitch(_uint iIndex) const { return m_bRenderSwitch[iIndex]; }
	void Set_RenderSwitch(uint32 iIndex, _bool onoff)
	{
		if (iIndex >= RENDER_SWITCH::SWITCH_END)
			return;

		m_bRenderSwitch[iIndex] = onoff;
	}

public: // Fog
	void Set_Fog(_float4 vFogColor, _float fFogStart, _float fFogEnd)
	{
		m_vFogColor = vFogColor;
		m_fFogStartEnd = _float2(fFogStart, fFogEnd);
	}

	void Set_FogColor(Vec4 vFogColor) { m_vFogColor = vFogColor; }
	Vec4 Get_FogColor() { return m_vFogColor; }

	void Set_FogDesc(const FOG_DESC& FogDesc) { m_FogDesc = FogDesc; }
	const FOG_DESC& Get_FogDesc() { return m_FogDesc; }

public: // Line
	void Set_LineColor(_float4 vLineColor)
	{
		m_vLineColor = vLineColor;
	}

public: // Radial
	_bool Get_RadialBlur() { return m_bRadialBlurDraw; }

	// Quality : �󸶳� ������ ����, Power : �󸶳� ��������.
	void Set_RadialBlur(_bool bSet, _float fQuality = 16.f, _float fRadialBlurPower = 0.1f) { 
		m_fRadialBlurPower = fRadialBlurPower;
		m_fRadialBlurQuality = min(fQuality, 16.f);
		m_fRadialBlurQuality = max(1.f, fQuality);
		m_bRadialBlurDraw = bSet;
	}

public: // ScreenEffect
	SCREEN_EFFECT Get_Current_ScreenEffect() { return m_eCurrentScreenEffect; }
	void Set_ScreenEffect(SCREEN_EFFECT eScreenEffect) { 
		if (eScreenEffect == SCREENEFFECT_END)
		{
			m_eCurrentScreenEffect = eScreenEffect;
			m_vScreenEffectAcc = { 0.f, 0.f };
		}
		else
		{
			if (m_eCurrentScreenEffect == SCREEN_EFFECT::SCREENEFFECT_END)
			{
				m_eCurrentScreenEffect = eScreenEffect;
				m_vScreenEffectAcc = { 0.f, 0.f };
			}
		}
	}
	
	Vec2 Get_ScreenEffectAcc() { return m_vScreenEffectAcc; }
	void Set_ScreenEffectAcc(const Vec2& vEffectAcc) { m_vScreenEffectAcc = vEffectAcc; }
	void Add_ScreenEffectAcc(const Vec2& vEffectAcc) { m_vScreenEffectAcc += vEffectAcc; }

public: // Minimap
	void Set_MinimapView(_float4x4 matView) { m_MinimapView = matView; }
	_float4x4 Get_MinimapView() { return m_MinimapView; }
	_float4x4 Get_MinimapProj() { return m_MinimapProj; }

public: // UI Setting Option
	_bool Get_NatrualDraw() { return m_bNaturalDraw; }
	void Set_NaturalDraw(_bool bSet) 
	{ 
		m_bNaturalDraw = bSet; 
		Check_Option();
	}

	_bool Get_ShadowDraw() { return m_bShadowDraw; }
	void Set_ShadowDraw(_bool bSet) 
	{ 
		m_bShadowDraw = bSet; 
		Check_Option();
	}

	_bool Get_SsaoDraw() { return m_bSsaoDraw; }
	void Set_SsaoDraw(_bool bSet)
	{ 
		m_bSsaoDraw = bSet; 
		Check_Option();
	}

	_bool Get_OutlineDraw() { return m_bOutlineDraw; }
	void Set_OutlineDraw(_bool bSet) 
	{ 
		m_bOutlineDraw = bSet; 
		Check_Option();
	}

	_bool Get_BlurDraw() { return m_bBlurDraw; }
	void Set_BlurDraw(_bool bSet) 
	{ 
		m_bBlurDraw = bSet; 
		Check_Option();
	}

	_bool Get_BloomDraw() { return m_bBlomDraw; }
	void Set_BloomDraw(_bool bSet) 
	{ 
		m_bBlomDraw = bSet; 
		Check_Option();
	}

	_bool Get_PbrDraw() { return m_bPbrDraw; }
	void Set_PbrDraw(_bool bSet) 
	{ 
		m_bPbrDraw = bSet; 
		Check_Option();
	}

public: // Add
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderGroup_Instancing(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, class CGameObject* pGameObject, _float4x4 WorldMatrix);
	HRESULT Add_RenderGroup_AnimInstancing(RENDERGROUP eRenderGroup, class CGameObject* pGameObject, _float4x4 WorldMatrix, const TweenDesc& TweenInstanceDesc, const ANIMODEL_INSTANCE_DESC& AnimModelInstanceDesc);
	HRESULT Add_RenderGroup_Instancing_Effect(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, class CGameObject* pGameObject, _float4x4 WorldMatrix, const EFFECT_INSTANCE_DESC& EffectInstanceDesc);
	HRESULT Add_Text(const TEXT_DESC& TextDesc);

//#ifdef _DEBUG
public:
	HRESULT Add_Debug(class CComponent* pDebug) {
		if (nullptr == pDebug)
			return E_FAIL;

		m_RenderDebug.push_back(pDebug);
		Safe_AddRef(pDebug);
		return S_OK;
	}

	void Set_PlayerPosition(_vector vPosition) { XMStoreFloat4(&m_vPlayerPosition, vPosition); }
//#endif

public: // Draw
	HRESULT Draw();

private:
	HRESULT Draw_BackGround();
	HRESULT Draw_World();
	HRESULT Draw_WorldEffect();
	HRESULT Draw_UI();
	HRESULT Draw_UIEffect();

private:
	HRESULT Render_Priority();
	HRESULT Render_Aurora();
	//HRESULT Render_Sun();
	HRESULT Render_NonLight();

	HRESULT Render_Stencil_ONLY();
	HRESULT Render_Reflect_Object();
	HRESULT Render_Blending_Mirror();

	HRESULT Render_Shadow();
	HRESULT Render_Shadow_Caculation();

	HRESULT Render_Cascade_Shadow();
	HRESULT Render_Cascade_Caculation();

	HRESULT Render_NonBlend();
	HRESULT Render_Lights();

	HRESULT Render_Ssao();
	HRESULT Render_OutLine();

	HRESULT Render_Deferred();
	HRESULT Render_Effect();
	HRESULT Render_Distortion();
	HRESULT Render_RadialBlur();
	HRESULT Render_Decal();
	HRESULT Render_LensFlare();
	HRESULT Render_AlphaBlend();

	HRESULT	Render_GodRay();

	HRESULT Render_UI();
	HRESULT Render_Text();

	//HRESULT Render_Shadow_UI();
	HRESULT Render_NonBlend_UI();
	HRESULT Render_Lights_UI();

	HRESULT Render_OutLine_UI();

	HRESULT Render_Deferred_UI();
	HRESULT Render_UI_Minimap();
	HRESULT Render_UI_Minimap_Icon();
	HRESULT Render_UIEffectNonBlend();
	HRESULT Render_UIEffectBlend();

	HRESULT Render_Screen_Effect();
	HRESULT Render_Final();
	HRESULT Render_Cursor();

	// Blur
	HRESULT Render_Blur_All(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear, _int iBlurSamplers, _float fBlurRange);
	HRESULT Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear, BLUR_PASS eHorizontalPass, BLUR_PASS eVerticalPass, BLUR_PASS eBlendType);
	HRESULT Render_BlurDownSample(const wstring& strStartTargetTag);
	HRESULT Render_Blur_Horizontal(BLUR_PASS eHorizontalPass);
	HRESULT Render_Blur_Vertical(BLUR_PASS eVerticalPass);
	HRESULT Render_BlurUpSample(const wstring& strFinalMrtTag, _bool bClear, BLUR_PASS eBlendType);

	// Mix
	HRESULT Render_AlphaBlendTargetMix(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear);

	// Clear
	HRESULT Render_ClearTarget(const wstring& strStartTargetTag);

//#ifdef _DEBUG
private:
	HRESULT Input_Key();
	HRESULT Render_Debug();
	HRESULT Render_Debug_Target();
//#endif // DEBUG

private:
	HRESULT Check_Option();

private: // SSAO
	void BuildFrustumFarCorners();
	void BuildOffsetVectors();

	HRESULT InitializeScreenQuad();
	HRESULT RenderScreenQuad();

private: // Initialize
	HRESULT Create_Buffer();
	HRESULT Create_Shader();
	HRESULT Create_Target();
	HRESULT Set_TargetsMrt();
	HRESULT Ready_Textures();
	HRESULT Initialize_SSAO();

//#ifdef _DEBUG
	HRESULT Set_Debug();
//#endif // DEBUG

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

private: // Fog
	class CTexture* m_pPerlinNoiseTextureCom = nullptr;

	_float4	m_vFogColor    = _float4(0.f, 0.635f, 1.f, 1.f);
	_float2	m_fFogStartEnd = _float2(300.f, 600.f);
	
	FOG_DESC m_FogDesc = {};

	_float4 m_vLineColor = _float4(0.5f, 0.4f, 0.2f, 1.f);

private: 
	_bool  m_bRenderSwitch[RENDER_SWITCH::SWITCH_END] = { true, false };

	_bool	m_bDebugDraw   = false;
	_bool	m_bTargetDraw  = false;
	_bool   m_bOption      = false;

	// On/ Off_Option
	_bool   m_bNaturalDraw = true;
	_bool   m_bShadowDraw  = true;
	_bool   m_bSsaoDraw    = true;
	_bool   m_bOutlineDraw = true;
	_bool   m_bBlurDraw    = true;
	_bool   m_bBlomDraw    = true;
	_bool	m_bRadialBlurDraw = false;
	_bool   m_bPbrDraw        = false;

private:
	_float m_fRadialBlurQuality = 16.f;
	_float m_fRadialBlurPower = 0.1f;

private: // Minimap
	_float4x4 m_MinimapView;
	_float4x4 m_MinimapProj;

private: // SSAO
	Vec4 m_vFrustumFarCorner[4];
	Vec4 m_vOffsets[26];

	class CTexture* m_pRandomVectorTexture = nullptr;

	// ScreenQuad
	ID3D11Buffer* m_pQuadVertexBuffer = nullptr;
	ID3D11Buffer* m_pQuadIndexBuffer = nullptr;

	struct QuadVertex
	{
		Vec3 pos;
		Vec3 ToFarPlaneIndex;
		Vec2 tex;
	};
	 
	_int m_iQuadVerCount;
	_int m_iQuadIndexCount;

private:
	 class CTexture* m_pScreenTextureCom = nullptr;

	Vec2 m_vScreenEffectAcc = { 0.f, 0.f };
	SCREEN_EFFECT m_eCurrentScreenEffect = SCREEN_EFFECT::SCREENEFFECT_END;

private:
	_float	m_fBias = 0.2f;
	_float4	m_vPlayerPosition = {0.f, 0.f, 0.f, 1.f};

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END