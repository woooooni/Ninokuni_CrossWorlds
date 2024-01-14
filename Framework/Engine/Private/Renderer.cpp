#include "..\Public\Renderer.h"

#include "GameInstance.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "PhysX_Manager.h"
#include "GameObject.h"
#include "Shader.h"
#include "Utils.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pLight_Manager);
	Safe_AddRef(m_pTarget_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	int  i = sizeof(ANIMODEL_INSTANCE_DESC);

	// Create_Buffer
	if (FAILED(Create_Buffer()))
		return E_FAIL;

	// Create_Shader
	if (FAILED(Create_Shader()))
		return E_FAIL;

	// Create_Target
	if (FAILED(Create_Target()))
		return E_FAIL;

	// Set_TargetsMrt
	if (FAILED(Set_TargetsMrt()))
		return E_FAIL;

	// Set_Debug
#ifdef _DEBUG
	if (FAILED(Set_Debug()))
		return E_FAIL;
#endif // DEBUG

	// Initialize_SSAO
	//if (FAILED(Initialize_SSAO()))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

#pragma region Add / Check
HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pGameObject);
	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_Instancing(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, CGameObject* pGameObject, _float4x4 WorldMatrix)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	auto iter = m_Render_Instancing_Objects[eRenderGroup].find(pGameObject->Get_PrototypeTag());
	if (iter == m_Render_Instancing_Objects[eRenderGroup].end())
	{
		INSTANCING_DESC InstancingDesc;
		InstancingDesc.pGameObject = pGameObject;
		Safe_AddRef(pGameObject);

		InstancingDesc.eShaderType = eShaderType;
		InstancingDesc.WorldMatrices.reserve(100);
		InstancingDesc.WorldMatrices.push_back(WorldMatrix);

		m_Render_Instancing_Objects[eRenderGroup].emplace(pGameObject->Get_PrototypeTag(), InstancingDesc);
	}
	else
	{
		if (nullptr == iter->second.pGameObject)
		{
			iter->second.pGameObject = pGameObject;
			Safe_AddRef(pGameObject);
		}

		iter->second.WorldMatrices.push_back(WorldMatrix);
	}
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_Instancing_Effect(RENDERGROUP eRenderGroup, INSTANCING_SHADER_TYPE eShaderType, CGameObject* pGameObject, _float4x4 WorldMatrix, const EFFECT_INSTANCE_DESC& EffectInstanceDesc)
{

	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	auto iter = m_Render_Instancing_Objects[eRenderGroup].find(pGameObject->Get_PrototypeTag());
	if (iter == m_Render_Instancing_Objects[eRenderGroup].end())
	{
		INSTANCING_DESC InstancingDesc;
		InstancingDesc.pGameObject = pGameObject;
		Safe_AddRef(pGameObject);

		InstancingDesc.eShaderType = eShaderType;
		InstancingDesc.WorldMatrices.reserve(500);
		InstancingDesc.WorldMatrices.push_back(WorldMatrix);

		InstancingDesc.EffectInstancingDesc.reserve(500);
		InstancingDesc.EffectInstancingDesc.push_back(EffectInstanceDesc);

		m_Render_Instancing_Objects[eRenderGroup].emplace(pGameObject->Get_PrototypeTag(), InstancingDesc);
	}
	else
	{
		if (nullptr == iter->second.pGameObject)
		{
			iter->second.pGameObject = pGameObject;
			Safe_AddRef(pGameObject);
		}

		if (iter->second.WorldMatrices.size() <= 500)
		{
			iter->second.WorldMatrices.push_back(WorldMatrix);
			iter->second.EffectInstancingDesc.push_back(EffectInstanceDesc);
		}

	}
	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup_AnimInstancing(RENDERGROUP eRenderGroup, CGameObject* pGameObject, _float4x4 WorldMatrix, const TweenDesc& TweenInstanceDesc, const ANIMODEL_INSTANCE_DESC& AnimModelInstanceDesc)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	auto iter = m_Render_Instancing_Objects[eRenderGroup].find(pGameObject->Get_PrototypeTag());
	if (iter == m_Render_Instancing_Objects[eRenderGroup].end())
	{
		INSTANCING_DESC InstancingDesc;
		InstancingDesc.pGameObject = pGameObject;
		Safe_AddRef(pGameObject);

		InstancingDesc.eShaderType = INSTANCING_SHADER_TYPE::ANIM_MODEL;
		InstancingDesc.WorldMatrices.reserve(500);
		InstancingDesc.WorldMatrices.push_back(WorldMatrix);

		InstancingDesc.TweenDesc.reserve(500);
		InstancingDesc.TweenDesc.push_back(TweenInstanceDesc);

		InstancingDesc.AnimInstanceDesc.reserve(500);
		InstancingDesc.AnimInstanceDesc.push_back(AnimModelInstanceDesc);

		m_Render_Instancing_Objects[eRenderGroup].emplace(pGameObject->Get_PrototypeTag(), InstancingDesc);
	}

	else
	{
		if (nullptr == iter->second.pGameObject)
		{
			iter->second.pGameObject = pGameObject;
			Safe_AddRef(pGameObject);
		}

		if (iter->second.WorldMatrices.size() <= 200)
		{
			iter->second.WorldMatrices.push_back(WorldMatrix);
			iter->second.TweenDesc.push_back(TweenInstanceDesc);
			iter->second.AnimInstanceDesc.push_back(AnimModelInstanceDesc);
		}
	}

	return S_OK;
}

HRESULT CRenderer::Add_Text(const TEXT_DESC& TextDesc)
{
	m_RenderTexts.push_back(TextDesc);
	return S_OK;
}

HRESULT CRenderer::Check_Option()
{
	if (!m_bNaturalDraw)
	{
		if (FAILED(Render_ClearTarget(L"Target_Aurora_Diffuse")))
			return E_FAIL;
	}
	if (!m_bShadowDraw)
	{
		if (FAILED(Render_ClearTarget(L"Target_ShadowDepth")))
			return E_FAIL;
	}
	if (!m_bSsaoDraw)
	{
		if (FAILED(Render_ClearTarget(L"Target_SSAO_Blur")))
			return E_FAIL;
	}
	if (!m_bOutlineDraw)
	{
		if (FAILED(Render_ClearTarget(L"Target_Outline")))
			return E_FAIL;
	}
	if (!m_bBlurDraw)
	{
		if (FAILED(Render_ClearTarget(L"Target_SSAO_Blur")))
			return E_FAIL;

		if (FAILED(Render_ClearTarget(L"Target_SSAO")))
			return E_FAIL;
	}
	if (!m_bBlomDraw)
	{
		if (FAILED(Render_ClearTarget(L"Target_Bloom_Blur")))
			return E_FAIL;
	}
	//if (!m_bPbrDraw)
	//{
	//	if (FAILED(Render_ClearTarget(L"")))
	//		return E_FAIL;
	//}

	return S_OK;
}
#pragma endregion

#pragma region Draw
HRESULT CRenderer::Draw()
{
	Input_Key();
#ifdef _DEBUG
#endif // DEBUG

	if (FAILED(Draw_BackGround()))
		return E_FAIL;

	if (FAILED(Draw_World()))
		return E_FAIL;

	/*
	if (FAILED(Render_GodRay()))
		return E_FAIL;

	CLight_Manager* pLightManger = GET_INSTANCE(CLight_Manager);
	const CGameObject* pLight = pLightManger->Get_Sun();

	if (nullptr != pLight)
	{
		if (FAILED(Render_AlphaBlendTargetMix(L"Target_GodRay", L"MRT_Blend", false)))
			return E_FAIL;
	}
	*/

	if (FAILED(Draw_UI()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif // DEBUG

	if (FAILED(Render_Final()))
		return E_FAIL;

	if (FAILED(Render_Cursor()))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(Render_Debug_Target()))
		return E_FAIL;
#endif // DEBUG

	return S_OK;
}

HRESULT CRenderer::Draw_BackGround()
{
	if (FAILED(Render_Priority()))  // MRT_Blend
		return E_FAIL;

	if (FAILED(Render_Aurora()))    // MRT_Aurora / MRT_Blend
		return E_FAIL;

	/*if (FAILED(Render_Sun()))     // 
		  return E_FAIL;*/

	if (FAILED(Render_NonLight()))  // MRT_Blend
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_World()
{
	// Target : Object
	{
		if (FAILED(Render_NonBlend())) // MRT_GameObjects -> Diffuse / Normal / Depth / Bloom
			return E_FAIL;


		if (FAILED(Render_Shadow())) // MRT_Shadow -> ShadowDepth
			return E_FAIL;

		if (FAILED(Render_Shadow_Caculation()))
			return E_FAIL;

		if (FAILED(Render_Blur(L"Target_ShadowDepth_Caculation", L"MRT_Shadow_Caculation_Blur", true, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
			return E_FAIL;


		if (FAILED(Render_Lights())) // MRT_Lights -> Shade / Specular
			return E_FAIL;
	}

	// PostEffect : Ssao / Bloom / OutLine
	{
		// Target : Ssao
		{
			if (m_bSsaoDraw && m_bBlurDraw)
			{
				if (FAILED(Render_Ssao()))
					return E_FAIL;

				if (FAILED(Render_Blur(L"Target_SSAO", L"MRT_SSAO_Blur", true, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
					return E_FAIL;
			}
		}

		// Target : Bloom
		{
			if (m_bBlomDraw)
			{
				if (FAILED(Render_Blur(L"Target_Bloom", L"MRT_Bloom_Blur", true, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
					return E_FAIL;
			}
		}

		// Target : OutLine
		{
			if (m_bOutlineDraw) // MRT_Outline -> Outline
			{
				if (FAILED(Render_OutLine()))
					return E_FAIL;
			}
		}
	}

	if (FAILED(Render_Deferred()))
		return E_FAIL;

	if (FAILED(Draw_WorldEffect()))
		return E_FAIL;

	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_WorldEffect()
{
	if (FAILED(Render_Effect()))
		return E_FAIL;

	if (FAILED(Render_Decal()))
		return E_FAIL;
	

	if (FAILED(Render_AlphaBlendTargetMix(L"Target_Decal_Diffuse", L"MRT_Blend", false)))
		return E_FAIL;

	if (m_bBlomDraw)
	{
		if (FAILED(Render_Blur(L"Target_Decal_Bloom", L"MRT_Blend", false, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
			return E_FAIL;
	}


	if (!m_bBlurDraw)
	{
		if (FAILED(Render_AlphaBlendTargetMix(L"Target_Effect_Diffuse_All", L"MRT_Blend", false)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Render_AlphaBlendTargetMix(L"Target_Effect_Diffuse_None", L"MRT_Blend", false)))
			return E_FAIL;

		if (FAILED(Render_Blur(L"Target_Effect_Diffuse_Low", L"MRT_Blend", false, BLUR_HOR_LOW, BLUR_VER_LOW, BLUR_UP_ONEMAX)))
			return E_FAIL;
		if (FAILED(Render_Blur(L"Target_Effect_Diffuse_Middle", L"MRT_Blend", false, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEMAX)))
			return E_FAIL;
		if (FAILED(Render_Blur(L"Target_Effect_Diffuse_High", L"MRT_Blend", false, BLUR_HOR_HIGH, BLUR_VER_HIGH, BLUR_UP_ONEMAX)))
			return E_FAIL;
	}

	if (m_bBlomDraw)
	{
		if (FAILED(Render_Blur(L"Target_Effect_Bloom", L"MRT_Blend", false, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Draw_UI()
{
	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_Text()))
		return E_FAIL;

	// Target : Object
	{
		//if (FAILED(Render_Shadow_UI()))
		//	return E_FAIL;

		if (FAILED(Render_NonBlend_UI()))
			return E_FAIL;

		if (FAILED(Render_Lights_UI()))
			return E_FAIL;
	}

	// PostEffect : Bloom / OutLine
	{
		// Target : Bloom
		{
			if (m_bBlomDraw)
			{
				if (FAILED(Render_Blur(L"Target_Bloom_UI", L"MRT_Bloom_Blur_UI", true, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
					return E_FAIL;
			}
		}

		// Target : OutLine
		{
			if (m_bOutlineDraw)
			{
				if (FAILED(Render_OutLine_UI()))
					return E_FAIL;
			}
		}
	}

	if (FAILED(Render_Deferred_UI()))
		return E_FAIL;

	if (FAILED(Draw_UIEffect()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Draw_UIEffect()
{
	if (FAILED(Render_UIEffectNonBlend()))
		return E_FAIL;

	if (!m_bBlurDraw)
	{
		if (FAILED(Render_AlphaBlendTargetMix(L"Target_Effect_UI_Diffuse_All", L"MRT_Blend", false)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(Render_AlphaBlendTargetMix(L"Target_Effect_UI_Diffuse_None", L"MRT_Blend", false)))
			return E_FAIL;

		if (FAILED(Render_Blur(L"Target_Effect_UI_Diffuse_Low", L"MRT_Blend", false, BLUR_HOR_LOW, BLUR_VER_LOW, BLUR_UP_ONEMAX)))
			return E_FAIL;
		if (FAILED(Render_Blur(L"Target_Effect_UI_Diffuse_Middle", L"MRT_Blend", false, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEMAX)))
			return E_FAIL;
		if (FAILED(Render_Blur(L"Target_Effect_UI_Diffuse_High", L"MRT_Blend", false, BLUR_HOR_HIGH, BLUR_VER_HIGH, BLUR_UP_ONEMAX)))
			return E_FAIL;
	}

	if (FAILED(Render_UIEffectBlend()))
		return E_FAIL;

	if (m_bBlomDraw)
	{
		if (FAILED(Render_Blur(L"Target_Effect_UI_Bloom", L"MRT_Blend", false, BLUR_HOR_MIDDLE, BLUR_VER_MIDDLE, BLUR_UP_ONEADD)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Input_Key()
{
	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::F2))
	{
		m_bDebugDraw = !m_bDebugDraw;
	}
	else if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::F3))
	{
		m_bOption = !m_bOption;

		m_bNaturalDraw = m_bOption;
		m_bShadowDraw = m_bOption;
		//m_bSsaoDraw = m_bOption;
		m_bOutlineDraw = m_bOption;
		m_bBlurDraw = m_bOption;
		m_bBlomDraw = m_bOption;
		m_bPbrDraw = m_bOption;

		Check_Option();
	}

	return S_OK;
}

#ifdef _DEBUG
#endif // DEBUG
#pragma endregion

#pragma region Render
// MRT_Blend
HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_PRIORITY])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
		{
			if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
				return E_FAIL;
		}

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
			return E_FAIL;

		Pair.second.TweenDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.AnimInstanceDesc.clear();
		Pair.second.EffectInstancingDesc.clear();



		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Aurora / MRT_Blend
HRESULT CRenderer::Render_Aurora()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Aurora"), true)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_AURORA])
	{
		if (m_bNaturalDraw)
		{
			if (FAILED(iter->Render()))
				return E_FAIL;
		}
		Safe_Release(iter);
	}
	m_RenderObjects[RENDERGROUP::RENDER_AURORA].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (m_bNaturalDraw)
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
			return E_FAIL;

		if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA]->Bind_Matrix("world", &m_WorldMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA]->Bind_Matrix("view", &m_ViewMatrix)))
			return E_FAIL;
		if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA]->Bind_Matrix("projection", &m_ProjMatrix)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA], TEXT("Target_Aurora_Diffuse"), "DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA]->Begin(0)))
			return E_FAIL;
		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;
	}

	return S_OK;
}

//HRESULT CRenderer::Render_Sun()
//{
//	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GodRay"), true)))
//		return E_FAIL;
//
//	Vec3 vScreenSunPos = m_pLight_Manager->Get_SunScreenPos();
//	if (vScreenSunPos.x > 1.05f || vScreenSunPos.x < -0.05f
//		|| vScreenSunPos.y > 1.05f || vScreenSunPos.y < -0.05f
//		|| vScreenSunPos.z > 1.f || vScreenSunPos.z < 0.f)
//	{
//		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
//			return E_FAIL;
//
//		return S_OK;
//	}
//
//	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_SUN])
//	{
//		if (FAILED(iter->Render()))
//			return E_FAIL;
//
//		Safe_Release(iter);
//	}
//
//	m_RenderObjects[RENDERGROUP::RENDER_SUN].clear();
//
//	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
//		return E_FAIL;
//
//
//	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
//		return E_FAIL;
//
//	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_Matrix("world", &m_WorldMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_Matrix("view", &m_ViewMatrix)))
//		return E_FAIL;
//	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_Matrix("projection", &m_ProjMatrix)))
//		return E_FAIL;
//
//
//	Vec2 vScreenSunUV(vScreenSunPos.x, vScreenSunPos.y);
//	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_RawValue("vScreenSunPosition",
//		&vScreenSunUV, sizeof(Vec2))))
//		return E_FAIL;
//
//	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS], TEXT("Target_SunOccluder"), "SunOccluderTexture")))
//		return E_FAIL;
//
//	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Begin(0)))
//		return E_FAIL;
//	if (FAILED(m_pVIBuffer->Render()))
//		return E_FAIL;
//
//	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
//		return E_FAIL;
//	
//
//	return S_OK;
//}

// MRT_Blend
HRESULT CRenderer::Render_NonLight()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_NONLIGHT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_NONLIGHT])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
			return E_FAIL;

		Pair.second.TweenDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.AnimInstanceDesc.clear();
		Pair.second.EffectInstancingDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_Shadow
HRESULT CRenderer::Render_Shadow()
{
	if (FAILED(m_pTarget_Manager->Begin_Shadow_MRT(m_pContext, TEXT("MRT_Shadow"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_SHADOW])
	{
		if (m_bShadowDraw)
		{
			if (FAILED(iter->Render_ShadowDepth()))
				return E_FAIL;
		}
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_SHADOW].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_SHADOW])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (m_bShadowDraw)
		{
			if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
			{
				if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
					return E_FAIL;
			}

			if (FAILED(Pair.second.pGameObject->Render_Instance_Shadow(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
				return E_FAIL;

			if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel_Shadow(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
				return E_FAIL;
		}

		Pair.second.TweenDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.AnimInstanceDesc.clear();
		Pair.second.EffectInstancingDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Shadow_Caculation
HRESULT CRenderer::Render_Shadow_Caculation()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Shadow_Caculation"))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4x4 LightMatix = GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel());
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_LightViewMatrix", &LightMatix)))
		return E_FAIL;
	_float4x4 CamProjMatrix = GI->Get_TransformFloat4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_CamProjMatrix", &CamProjMatrix)))
		return E_FAIL;
	_float4x4 ViewMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;
	_float4x4 ProjMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_ShadowDepth"), "g_ShadowTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Begin(6)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_GameObjects
HRESULT CRenderer::Render_NonBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_NONBLEND])
	{
		if (nullptr == iter)
			continue;

		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_NONBLEND].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_NONBLEND])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
		{
			if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
				return E_FAIL;
		}

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
			return E_FAIL;

		Pair.second.TweenDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.AnimInstanceDesc.clear();
		Pair.second.EffectInstancingDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Lights
HRESULT CRenderer::Render_Lights()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Lights"))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;
	_float4x4 ViewMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;
	_float4x4 ProjMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Normal"), "g_NormalTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;

	m_pLight_Manager->Render(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_SSAO
HRESULT CRenderer::Render_Ssao()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSAO"))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO], TEXT("Target_Normal"), "g_NormalTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO], TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO]->Begin(0)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Outline
HRESULT CRenderer::Render_OutLine()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Outline"))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE], TEXT("Target_Normal"), "g_NormalTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_RawValue("g_vLineColor", &m_vLineColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Begin(0)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	// Bias
	if (KEY_TAP(KEY::OPEN_SQUARE_BRACKET))
	{
		if (KEY_HOLD(KEY::SHIFT))
		{
			m_fBias += 0.0001f;
		}
		else
		{
			m_fBias += 0.001f;
		}

	}
	if (KEY_TAP(KEY::CLOSE_SQUARE_BRACKET))
	{
		if (KEY_HOLD(KEY::SHIFT))
		{
			m_fBias -= 0.0001f;
		}
		else
		{
			m_fBias -= 0.001f;
		}

	}
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_fBias", &m_fBias, sizeof(_float))))
		return E_FAIL;
	

	// Fog
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_vFogColor", &m_vFogColor, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_fFogStartEnd", &m_fFogStartEnd, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Diffuse"), "g_DiffuseTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Shade"), "g_ShadeTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Specular"), "g_SpecularTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Depth"), "g_DepthTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_ShadowDepth_Caculation_Blur"), "g_ShadowTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_SSAO_Blur"), "g_SSAOTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Bloom_Blur"), "g_BloomTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Outline"), "g_OutlineTarget")))
		return E_FAIL;

	// 옵션 셋팅
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bShadowDraw", &m_bShadowDraw, sizeof(_bool))))
		return E_FAIL;

	_bool bSsaoDraw = false;
	if (m_bSsaoDraw && m_bBlurDraw)
		bSsaoDraw = true;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bSsaoDraw", &bSsaoDraw, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bOutLineDraw", &m_bOutlineDraw, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Begin(3)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_Effect
HRESULT CRenderer::Render_Effect()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effect"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_EFFECT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_EFFECT].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_EFFECT])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
		{
			if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
				return E_FAIL;
		}

		if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_EffectDesc", Pair.second.EffectInstancingDesc.data(), sizeof(EFFECT_INSTANCE_DESC) * Pair.second.WorldMatrices.size())))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;


		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
			return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();
		Pair.second.AnimInstanceDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Decal
HRESULT CRenderer::Render_Decal()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Decal"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_DECAL])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_DECAL].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_AlphaBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_ALPHABLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_ALPHABLEND].clear();


	for (auto& Pair : m_Render_Instancing_Objects[RENDER_ALPHABLEND])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
		{
			if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
				return E_FAIL;
		}

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
			return E_FAIL;

		Pair.second.EffectInstancingDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.TweenDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_GodRay
HRESULT CRenderer::Render_GodRay()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GodRay"))))
		return E_FAIL;

	Vec3 vScreenSunPos = m_pLight_Manager->Get_SunScreenPos();
	if (vScreenSunPos.x > 1.05f || vScreenSunPos.x < -0.05f
		|| vScreenSunPos.y > 1.05f || vScreenSunPos.y < -0.05f
		|| vScreenSunPos.z > 1.f || vScreenSunPos.z < 0.f)
	{
		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		return S_OK;
	}

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_Matrix("world", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_Matrix("view", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_Matrix("projection", &m_ProjMatrix)))
		return E_FAIL;


	Vec2 vScreenSunUV(vScreenSunPos.x, vScreenSunPos.y);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Bind_RawValue("vScreenSunPosition",
		&vScreenSunUV, sizeof(Vec2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS],
		TEXT("Target_SunOccluder"), "SunOccluderTexture")))
		return E_FAIL;

	m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS]->Begin(0);

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_Blend
HRESULT CRenderer::Render_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UI])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UI].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_Text()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderTexts)
	{
		if (FAILED(GI->Render_Fonts(iter.strFontTag, iter.strText,
			iter.vPosition,
			XMLoadFloat4(&iter.vColor),
			iter.fAngle,
			iter.vOrigin,
			iter.vScale)))
			return E_FAIL;
	}
	m_RenderTexts.clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_Shadow_UI
//HRESULT CRenderer::Render_Shadow_UI()
//{
	// Begin_Shadow_UI_MRT
	//if (FAILED(m_pTarget_Manager->Begin_Shadow_MRT(m_pContext, TEXT("MRT_Shadow_UI"))))
	//	return E_FAIL;

	//for (auto& iter : m_RenderObjects[RENDER_SHADOW_UI])
	//{
	//	if (m_bShadowDraw)
	//	{
	//		if (FAILED(iter->Render_ShadowDepth()))
	//			return E_FAIL;
	//	}
	//	Safe_Release(iter);
	//}
	//m_RenderObjects[RENDER_SHADOW_UI].clear();

	//for (auto& Pair : m_Render_Instancing_Objects[RENDER_SHADOW_UI])
	//{
	//	if (nullptr == Pair.second.pGameObject)
	//		continue;

	//	if (m_bShadowDraw)
	//	{
	//		if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
	//		{
	//			if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
	//				return E_FAIL;
	//		}

	//		if (FAILED(Pair.second.pGameObject->Render_Instance_Shadow(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
	//			return E_FAIL;

	//		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel_Shadow(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
	//			return E_FAIL;
	//	}

	//	Pair.second.TweenDesc.clear();
	//	Pair.second.WorldMatrices.clear();
	//	Pair.second.AnimInstanceDesc.clear();
	//	Pair.second.EffectInstancingDesc.clear();
	//	
	//	Safe_Release(Pair.second.pGameObject);
	//	Pair.second.pGameObject = nullptr;
	//}

	//if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
	//	return E_FAIL;

	//return S_OK;
//}

// MRT_GameObjects_UI
HRESULT CRenderer::Render_NonBlend_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_UI_MRT(m_pContext, TEXT("MRT_GameObjects_UI"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_NONBLEND_UI])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_NONBLEND_UI].clear();

	for (auto& Pair : m_Render_Instancing_Objects[RENDER_NONBLEND_UI])
	{
		if (nullptr == Pair.second.pGameObject)
			continue;

		if (Pair.second.eShaderType == INSTANCING_SHADER_TYPE::ANIM_MODEL)
		{
			if (FAILED(m_pIntancingShaders[Pair.second.eShaderType]->Bind_RawValue("g_AnimInstancingDesc", Pair.second.AnimInstanceDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * Pair.second.AnimInstanceDesc.size())))
				return E_FAIL;
		}

		if (FAILED(Pair.second.pGameObject->Render_Instance(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices)))
			return E_FAIL;

		if (FAILED(Pair.second.pGameObject->Render_Instance_AnimModel(m_pIntancingShaders[Pair.second.eShaderType], m_pVIBuffer_Instancing, Pair.second.WorldMatrices, Pair.second.TweenDesc, Pair.second.AnimInstanceDesc)))
			return E_FAIL;

		Pair.second.TweenDesc.clear();
		Pair.second.WorldMatrices.clear();
		Pair.second.AnimInstanceDesc.clear();
		Pair.second.EffectInstancingDesc.clear();

		Safe_Release(Pair.second.pGameObject);
		Pair.second.pGameObject = nullptr;
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Lights_UI
HRESULT CRenderer::Render_Lights_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Lights_UI"))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;
	_float4x4 ViewMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_VIEW);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrixInv", &ViewMatrixInv)))
		return E_FAIL;
	_float4x4 ProjMatrixInv = GI->Get_TransformMatrixInverse_Float4x4(CPipeLine::D3DTS_PROJ);
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrixInv", &ProjMatrixInv)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Normal_UI"), "g_NormalTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Depth_UI"), "g_DepthTarget")))
		return E_FAIL;

	m_pLight_Manager->Render(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer);

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Outline_UI
HRESULT CRenderer::Render_OutLine_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Outline_UI"))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE], TEXT("Target_Normal_UI"), "g_NormalTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Bind_RawValue("g_vLineColor", &m_vLineColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE]->Begin(0)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_Deferred_UI()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	// Fog
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_vFogColor", &m_vFogColor, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_fFogStartEnd", &m_fFogStartEnd, sizeof(_float2))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Diffuse_UI"), "g_DiffuseTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Shade_UI"), "g_ShadeTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Specular_UI"), "g_SpecularTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Depth_UI"), "g_DepthTarget")))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_ShadowDepth_UI"), "g_ShadowTarget")))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Bloom_Blur_UI"), "g_BloomTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], TEXT("Target_Outline_UI"), "g_OutlineTarget")))
		return E_FAIL;

	// 옵션 셋팅
	_bool bShadowDraw = false;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bShadowDraw", &bShadowDraw, sizeof(_bool))))
		return E_FAIL;

	_bool bSsaoDraw = false;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bSsaoDraw", &bSsaoDraw, sizeof(_bool))))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_RawValue("g_bOutLineDraw", &m_bOutlineDraw, sizeof(_bool))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Begin(3)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// MRT_Effect_UI
HRESULT CRenderer::Render_UIEffectNonBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effect_UI"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UI_EFFECT_NONBLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UI_EFFECT_NONBLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

// MRT_Blend
HRESULT CRenderer::Render_UIEffectBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UI_EFFECT_BLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UI_EFFECT_BLEND].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}


// Final
HRESULT CRenderer::Render_Final()
{
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	// Diffuse
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL], L"Target_Blend", "g_DiffuseTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL]->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

// Cursor
HRESULT CRenderer::Render_Cursor()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_CURSOR])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_CURSOR].clear();

	return S_OK;
}


#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Blend"), false)))
		return E_FAIL;

	for (auto& pDebugCom : m_RenderDebug)
	{
		if (true == m_bDebugDraw)
			pDebugCom->Render();

		Safe_Release(pDebugCom);
	}
	m_RenderDebug.clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Debug_Target()
{
	if (false == m_bDebugDraw)
		return S_OK;



	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(CPhysX_Manager::GetInstance()->Render()))
		return E_FAIL;

	// 
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_GameObjects"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Lights"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Shadow"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_SSAO"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_SSAO_Blur"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Outline"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Decal"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effect"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effect_UI"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Blend"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Aurora"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_GodRay"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Shadow_Caculation_Blur"), m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], m_pVIBuffer)))
		return E_FAIL;

	wstring strPlayerPosition = L"";
	strPlayerPosition += L"X : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.x);
	strPlayerPosition += L'\n';
	strPlayerPosition += L"Y : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.y);
	strPlayerPosition += L'\n';
	strPlayerPosition += L"Z : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.z);
	strPlayerPosition += L'\n';
	strPlayerPosition += L"W : ";
	strPlayerPosition += to_wstring(m_vPlayerPosition.w);

	GI->Render_Fonts(L"Basic", strPlayerPosition.c_str(), _float2(1600.f / 2.f, 0.f));

	return S_OK;
}
#endif // DEBUG
#pragma endregion

#pragma region Blur
HRESULT CRenderer::Render_Blur_All(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear, _int iBlurSamplers, _float fBlurRange)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, strFinalTragetTag, bClear)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR], strStartTargetTag, "g_BlurTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_RawValue("g_iBlurSamplers", &iBlurSamplers, sizeof(_int))))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_RawValue("g_fBlurRange", &fBlurRange, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Begin(BLUR_PASS::BLUR_ALL)))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear, BLUR_PASS eHorizontalPass, BLUR_PASS eVerticalPass, BLUR_PASS eBlendType)
{
	if (FAILED(Render_BlurDownSample(strStartTargetTag)))
		return E_FAIL;

	if (FAILED(Render_Blur_Horizontal(eHorizontalPass)))
		return E_FAIL;

	if (FAILED(Render_Blur_Vertical(eVerticalPass)))
		return E_FAIL;

	if (FAILED(Render_BlurUpSample(strFinalTragetTag, bClear, eBlendType)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurDownSample(const wstring& strTargetTag)
{
	// 블러(DownSample)
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_Blur_DownSampling")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR], strTargetTag, "g_BlurTarget")))
		return E_FAIL;

	_float4x4 WorldMatrix = m_WorldMatrix;
	WorldMatrix._11 = m_WorldMatrix._11 * 0.5f;
	WorldMatrix._22 = m_WorldMatrix._22 * 0.5f;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Begin(BLUR_PASS::BLUR_DOWN)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Horizontal(BLUR_PASS eHorizontalPass)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_Blur_Horizontal")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR], TEXT("Target_Blur_DownSampling"), "g_BlurTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Begin(eHorizontalPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blur_Vertical(BLUR_PASS eVerticalPass)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, L"MRT_Blur_Vertical")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR], TEXT("Target_Blur_Horizontal"), "g_BlurTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Begin(eVerticalPass)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_BlurUpSample(const wstring& strFinalMrtTag, _bool bClear, BLUR_PASS eBlendType)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, strFinalMrtTag, bClear)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR], TEXT("Target_Blur_Vertical"), "g_BlurTarget")))
		return E_FAIL;

	_float4x4 WorldMatrix = m_WorldMatrix;
	WorldMatrix._11 = m_WorldMatrix._11 * 2.f;
	WorldMatrix._22 = m_WorldMatrix._22 * 2.f;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR]->Begin(eBlendType)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}
#pragma endregion

#pragma region Mix / Clear
HRESULT CRenderer::Render_AlphaBlendTargetMix(const wstring& strStartTargetTag, const wstring& strFinalTragetTag, _bool bClear)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, strFinalTragetTag, bClear)))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	// Diffuse
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED], strStartTargetTag, "g_BlendMixTarget")))
		return E_FAIL;

	if (FAILED(m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED]->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ClearTarget(const wstring& strStartTargetTag)
{
	if (FAILED(m_pTarget_Manager->Clear_RenderTarget(strStartTargetTag)))
		return E_FAIL;

	return S_OK;
}
#pragma endregion

#pragma region Initialize
HRESULT CRenderer::Create_Buffer()
{
	// VIBuffer_Rect
	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	// VIBuffer_Instancing
	m_pVIBuffer_Instancing = CVIBuffer_Instancing::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer_Instancing)
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Create_Shader()
{
	// SHADER_DEFERRED
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_DEFERRED])
		return E_FAIL;

	// SHADER_OUTLINE
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_OutLine.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_OUTLINE])
		return E_FAIL;

	// SHADER_BLUR
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Blur.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_BLUR])
		return E_FAIL;

	// SHADER_SSAO
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Ssao.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_SSAO])
		return E_FAIL;

	// SHADER_FINAL
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Final.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_FINAL])
		return E_FAIL;

	// SHADER_POSTAURORA
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_PostAurora.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_AURORA])
		return E_FAIL;

	// SHADER_POSTPROCESS
	m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_PostProcess.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pShaders[RENDERER_SHADER_TYPE::SHADER_POSTPROCESS])
		return E_FAIL;

	// Shader_Model_Instance
	m_pIntancingShaders[INSTANCING_SHADER_TYPE::MODEL] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Model_Instance.hlsl"), VTXMODELINSTANCE_DECLARATION::Elements, VTXMODELINSTANCE_DECLARATION::iNumElements);
	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::MODEL])
		return E_FAIL;

	// Shader_AnimModel_Instance
	m_pIntancingShaders[INSTANCING_SHADER_TYPE::ANIM_MODEL] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel_Instance.hlsl"), VTXANIMMODELINSTANCE_DECLARATION::Elements, VTXANIMMODELINSTANCE_DECLARATION::iNumElements);
	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::ANIM_MODEL])
		return E_FAIL;

	// Shader_Rect_Instance
	m_pIntancingShaders[INSTANCING_SHADER_TYPE::RECT] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Rect_Instance.hlsl"), VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements);
	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::RECT])
		return E_FAIL;

	// Shader_TextureEffect_Instance
	m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_TEXTURE] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_TextureEffect_Instance.hlsl"), VTXRECTINSTANCE_DECLARATION::Elements, VTXRECTINSTANCE_DECLARATION::iNumElements);
	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_TEXTURE])
		return E_FAIL;

	// Shader_MeshEffect_Instance
	m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_MODEL] = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_MeshEffect_Instance.hlsl"), VTXMODELINSTANCE_DECLARATION::Elements, VTXMODELINSTANCE_DECLARATION::iNumElements);
	if (nullptr == m_pIntancingShaders[INSTANCING_SHADER_TYPE::EFFECT_MODEL])
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Create_Target()
{
	D3D11_VIEWPORT ViewportDesc;
	_uint iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

#pragma region MRT_GameObjects : Target_Diffuse / Target_Normal / Target_Depth / Target_Bloom / Target_SunOccluder / Target_MiniMap // Target_Bloom_Blur
	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Bloom"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_SunMask*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SunOccluder"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_MiniMap */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_MiniMap"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	// MRT_Bloom_Blur
	{
		// Target_Bloom_Blur
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Bloom_Blur"),
			ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
	}
#pragma endregion

#pragma region MRT_Lights : Target_Shade / Target_Specular
	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Shadow : Target_ShadowDepth
	/* For.Target_ShadowDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"),
		ViewportDesc.Width * 3.f, ViewportDesc.Height * 3.f, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_SSAO : Target_SSAO // Target_SSAO_Blur
	/* For.Target_SSAO */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAO"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_SSAO_Blur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAO_Blur"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Outline : Target_Outline
	/* For.Target_Outline */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion


#pragma region MRT_Decal : Target_Decal_Diffuse / Target_Decal_Bloom
	/* For.Target_Decal_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Decal_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Decal_Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Decal_Bloom"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Effect : Target_Effect_Diffuse 1 ~ 5 / Target_Effect_Bloom
	/* For.Target_Effect_Diffuse_All */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Diffuse_All"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_Diffuse_None */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Diffuse_None"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_Diffuse_Low */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Diffuse_Low"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_Diffuse_Middle */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Diffuse_Middle"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_Diffuse_High */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Diffuse_High"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Effect_Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_Bloom"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion


#pragma region MRT_GameObjects_UI : Target_Diffuse_UI / Target_Normal_UI / Target_Depth_UI / Target_Bloom_UI // Target_Bloom_Blur_UI
	/* For.Target_Diffuse_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Bloom_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Bloom_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// MRT_Bloom_Blur_UI
	{
		// Target_Bloom_Blur
		if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Bloom_Blur_UI"),
			ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
			return E_FAIL;
	}
#pragma endregion

#pragma region MRT_Lights_UI : Target_Shade_UI / Target_Specular_UI
	/* For.Target_Shade_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Specular_UI */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Shadow_UI : Target_ShadowDepth_UI
	/* For.Target_ShadowDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth_UI"),
		ViewportDesc.Width * 3.f, ViewportDesc.Height * 3.f, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Outline_UI : Target_Outline_UI
	/* For.Target_Outline */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline_UI"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion


#pragma region MRT_Effect_UI : Target_Effect_UI_Diffuse 1 ~ 5 / Target_Effect_UI_Bloom
	/* For.Target_Effect_UI_Diffuse_All */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Diffuse_All"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_UI_Diffuse_None */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Diffuse_None"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_UI_Diffuse_Low */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Diffuse_Low"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_UI_Diffuse_Middle */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Diffuse_Middle"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Effect_UI_Diffuse_High */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Diffuse_High"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Effect_UI_Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Effect_UI_Bloom"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion


#pragma region MRT_Blend : Target_Blend // FinalTarget
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.729f, 0.816f, 0.851f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region Blur_Targets
	// For.Target_Blur_DownSampling
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_DownSampling"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Blur_Horizontal
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Horizontal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Blur_Vertical
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_Vertical"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	// For.Target_Blur_UpSampling
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Blur_UpSampling"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Aurora : Target_Aurora_Diffuse
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Aurora_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_GodRay : Target_GodRay
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_GodRay"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Shadow_Caculation : Target_ShadowDepth_Caculation
	/* For.Target_ShadowDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth_Caculation"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
#pragma endregion

#pragma region MRT_Shadow_Caculation_Blur : Target_ShadowDepth_Caculation_Blur
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth_Caculation_Blur"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
#pragma endregion

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	return S_OK;
}

HRESULT CRenderer::Set_TargetsMrt()
{
	// MRT_GameObjects / MRT_Bloom_Blur
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Bloom"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_SunOccluder"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_MiniMap"))))
			return E_FAIL;
		
		// MRT_Bloom_Blur
		{
			if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Bloom_Blur"), TEXT("Target_Bloom_Blur"))))
				return E_FAIL;
		}
	}

	// MRT_Lights
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Specular"))))
			return E_FAIL;
	}

	// MRT_Shadow
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Shadow"), TEXT("Target_ShadowDepth"))))
			return E_FAIL;
	}

	// MRT_SSAO / MRT_SSAO_Blur
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAO"), TEXT("Target_SSAO"))))
			return E_FAIL;

		// MRT_SSAO_Blur
		{
			if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAO_Blur"), TEXT("Target_SSAO_Blur"))))
				return E_FAIL;
		}
	}

	// MRT_Outline
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Outline"), TEXT("Target_Outline"))))
			return E_FAIL;
	}


	// MRT_Decal
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Decal"), TEXT("Target_Decal_Diffuse"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Decal"), TEXT("Target_Decal_Bloom"))))
			return E_FAIL;
	}

	// MRT_Effect
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Diffuse_All"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Diffuse_None"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Diffuse_Low"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Diffuse_Middle"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Diffuse_High"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Effect_Bloom"))))
			return E_FAIL;
	}


	// MRT_GameObjects_UI / MRT_Bloom_Blur_UI
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects_UI"), TEXT("Target_Diffuse_UI"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects_UI"), TEXT("Target_Normal_UI"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects_UI"), TEXT("Target_Depth_UI"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects_UI"), TEXT("Target_Bloom_UI"))))
			return E_FAIL;

		// MRT_Bloom_Blur_UI
		{
			if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Bloom_Blur_UI"), TEXT("Target_Bloom_Blur_UI"))))
				return E_FAIL;
		}
	}

	// MRT_Lights_UI
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights_UI"), TEXT("Target_Shade_UI"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights_UI"), TEXT("Target_Specular_UI"))))
			return E_FAIL;
	}

	// MRT_Shadow_UI
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Shadow_UI"), TEXT("Target_ShadowDepth_UI"))))
			return E_FAIL;
	}

	// MRT_Outline_UI
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Outline_UI"), TEXT("Target_Outline_UI"))))
			return E_FAIL;
	}


	// MRT_Effect_UI
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Diffuse_All"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Diffuse_None"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Diffuse_Low"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Diffuse_Middle"))))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Diffuse_High"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect_UI"), TEXT("Target_Effect_UI_Bloom"))))
			return E_FAIL;
	}

	// MRT_Aurora
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Aurora"), TEXT("Target_Aurora_Diffuse"))))
			return E_FAIL;
	}

	// MRT_GodRay
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GodRay"), TEXT("Target_GodRay"))))
			return E_FAIL;
	}

	// MRT_Blend
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Blend", L"Target_Blend")))
			return E_FAIL;
	}

	// MRT_Shadow_Caculation
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Shadow_Caculation", L"Target_ShadowDepth_Caculation")))
			return E_FAIL;
	}

	// MRT_Shadow_Caculation_Blur
	{
		if (FAILED(m_pTarget_Manager->Add_MRT(L"MRT_Shadow_Caculation_Blur", L"Target_ShadowDepth_Caculation_Blur")))
			return E_FAIL;
	}

	// Blur
	{
		/* For.MRT_BlurDownSampling */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_DownSampling"), TEXT("Target_Blur_DownSampling"))))
			return E_FAIL;

		/* For.MRT_Blur_Horizontal */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Horizontal"), TEXT("Target_Blur_Horizontal"))))
			return E_FAIL;

		/* For.MRT_Blur_Vertical */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_Vertical"), TEXT("Target_Blur_Vertical"))))
			return E_FAIL;

		/* For.MRT_BlurUpSampling */
		if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Blur_UpSampling"), TEXT("Target_Blur_UpSampling"))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CRenderer::Initialize_SSAO()
{
	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Set_Debug()
{
	_float fSizeX = 150.f;
	_float fSizeY = 100.f;

	// MRT_GameObjects
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"),     (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 0), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"),      (fSizeX / 2.f) + (fSizeX * 1), (fSizeY / 2.f) + (fSizeY * 0), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"),       (fSizeX / 2.f) + (fSizeX * 2), (fSizeY / 2.f) + (fSizeY * 0), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Bloom"),       (fSizeX / 2.f) + (fSizeX * 3), (fSizeY / 2.f) + (fSizeY * 0), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SunOccluder"), (fSizeX / 2.f) + (fSizeX * 4), (fSizeY / 2.f) + (fSizeY * 0), fSizeX, fSizeY)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_MiniMap"),     (fSizeX / 2.f) + (fSizeX * 6), (fSizeY / 2.f) + (fSizeY * 0), fSizeX, fSizeY)))
		return E_FAIL;

	// MRT_Lights
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"),                  (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 1), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"),               (fSizeX / 2.f) + (fSizeX * 1), (fSizeY / 2.f) + (fSizeY * 1), fSizeX, fSizeY)))
		return E_FAIL;
	// MRT_Shadow
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"),                 (fSizeX / 2.f) + (fSizeX * 3), (fSizeY / 2.f) + (fSizeY * 1), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth_Caculation_Blur"), (fSizeX / 2.f) + (fSizeX * 4), (fSizeY / 2.f) + (fSizeY * 1), fSizeX, fSizeY)))
		return E_FAIL;


	//// MRT_SSAO
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSAO"), (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 2), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSAO_Blur"), (fSizeX / 2.f) + (fSizeX * 1), (fSizeY / 2.f) + (fSizeY * 2), fSizeX, fSizeY)))
		return E_FAIL;
	// MRT_Outline
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), (fSizeX / 2.f) + (fSizeX * 3), (fSizeY / 2.f) + (fSizeY * 2), fSizeX, fSizeY)))
		return E_FAIL;


	// MRT_Effect
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Diffuse_All"), (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 3), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Diffuse_None"), (fSizeX / 2.f) + (fSizeX * 1), (fSizeY / 2.f) + (fSizeY * 3), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Diffuse_Low"), (fSizeX / 2.f) + (fSizeX * 2), (fSizeY / 2.f) + (fSizeY * 3), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Diffuse_Middle"), (fSizeX / 2.f) + (fSizeX * 3), (fSizeY / 2.f) + (fSizeY * 3), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Diffuse_High"), (fSizeX / 2.f) + (fSizeX * 4), (fSizeY / 2.f) + (fSizeY * 3), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_Bloom"), (fSizeX / 2.f) + (fSizeX * 5), (fSizeY / 2.f) + (fSizeY * 3), fSizeX, fSizeY)))
		return E_FAIL;


	// MRT_Effect_UI
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Diffuse_All"), (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 4), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Diffuse_None"), (fSizeX / 2.f) + (fSizeX * 1), (fSizeY / 2.f) + (fSizeY * 4), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Diffuse_Low"), (fSizeX / 2.f) + (fSizeX * 2), (fSizeY / 2.f) + (fSizeY * 4), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Diffuse_Middle"), (fSizeX / 2.f) + (fSizeX * 3), (fSizeY / 2.f) + (fSizeY * 4), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Diffuse_High"), (fSizeX / 2.f) + (fSizeX * 4), (fSizeY / 2.f) + (fSizeY * 4), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Effect_UI_Bloom"), (fSizeX / 2.f) + (fSizeX * 5), (fSizeY / 2.f) + (fSizeY * 4), fSizeX, fSizeY)))
		return E_FAIL;

	// Aurora
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Aurora_Diffuse"), (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 5), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_GodRay"), (fSizeX / 2.f) + (fSizeX * 2), (fSizeY / 2.f) + (fSizeY * 5), fSizeX, fSizeY)))
		return E_FAIL;

	// Mrt_Decal
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Decal_Diffuse"), (fSizeX / 2.f) + (fSizeX * 0), (fSizeY / 2.f) + (fSizeY * 6), fSizeX, fSizeY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Decal_Bloom"), (fSizeX / 2.f) + (fSizeX * 1), (fSizeY / 2.f) + (fSizeY * 6), fSizeX, fSizeY)))
		return E_FAIL;

	// MRT_Blend
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Blend"), 150.f, 825.f, 300.f, 150.f)))
		return E_FAIL;

	return S_OK;
}
#endif // DEBUG
#pragma endregion

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();

	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);

	for (_uint i = 0; i < RENDERER_SHADER_TYPE::SHADER_END; ++i)
		Safe_Release(m_pShaders[i]);

	for (_uint i = 0; i < INSTANCING_SHADER_TYPE::TYPE_END; ++i)
		Safe_Release(m_pIntancingShaders[i]);

	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pVIBuffer_Instancing);

	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for (auto& pObj : m_RenderObjects[i])
			Safe_Release(pObj);
		m_RenderObjects[i].clear();
	}

	for (_uint i = 0; i < RENDERGROUP::RENDER_END; ++i)
	{
		for (auto iter : m_Render_Instancing_Objects[i])
		{
			iter.second.EffectInstancingDesc.clear();
			iter.second.TweenDesc.clear();
			iter.second.WorldMatrices.clear();
			Safe_Release(iter.second.pGameObject);
		}
		m_Render_Instancing_Objects[i].clear();
	}

	for (auto& pComponent : m_RenderDebug)
		Safe_Release(pComponent);
	m_RenderDebug.clear();

	//Safe_Release(m_pRandomSrv);
}
