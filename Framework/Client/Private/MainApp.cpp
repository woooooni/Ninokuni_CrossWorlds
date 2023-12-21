#include "stdafx.h"
#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "VIBuffer_Trail.h"
#include "Level_Loading.h"


#include "ImGui_Manager.h"
#include "Picking_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "UI_Manager.h"
#include "Camera_Manager.h"
#include "Light.h"
#include "Part_Manager.h"

#ifdef _DEBUG
  //#include <vld.h>
#endif

CMainApp::CMainApp()	
{
}


HRESULT CMainApp::Initialize()
{
	/* 1. 내 게임의 초기화를 수행할꺼야. */
	/* 1-1. 그래픽장치를 초기화한다. */
	/* 1-2. 사운드장치를 초기화한다. */
	/* 1-3. 입력장치를 초기화한다. */
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(GI->Initialize_Engine(LEVEL_END, _uint(LAYER_TYPE::LAYER_END), GraphicDesc, &m_pDevice, &m_pContext, g_hWnd, g_hInstance)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	if (FAILED(Initialize_Client()))
		return E_FAIL;

	/* 1-4. 게임내에서 사용할 레벨(씬)을 생성한다.   */
	if (FAILED(Open_Level(LEVEL_TOOL, L"Final_Boss")))
		return E_FAIL;
	
	// UI Cursor
/*	 CUI_Manager::GetInstance()->Ready_Cursor();
	 ShowCursor(false)*/;


	Json Test;
	Test["Name"] = "김태원";
	Test["Test"] = "테스트";
	Test["int"] = 3;
	Test["float"] = 3.f;
	Test["string"] = "스트링";

	GI->Json_Save(L"../Bin/Test.json", Test);
	Json Load = GI->Json_Load(L"../Bin/Test.json");
	string strName = Load["Name"];

	string strLoad = Load.dump();
	

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	CCamera_Manager::GetInstance()->Tick(fTimeDelta);
	GI->Tick(fTimeDelta);
	CUI_Manager::GetInstance()->Tick(fTimeDelta);
	
	CCamera_Manager::GetInstance()->LateTick(fTimeDelta);
	GI->LateTick(fTimeDelta);
	CUI_Manager::GetInstance()->LateTick(fTimeDelta);
	
	
	m_fTimeAcc += fTimeDelta;
	
}

HRESULT CMainApp::Render()
{
	/* 게임내에 존재하는 여러 객체들의 렌더링. */
	GI->Clear_BackBuffer_View(_float4(0.3f, 0.3f, 0.3f, 1.f));
	GI->Clear_DepthStencil_View();

	m_pRenderer_Com->Draw();
	GI->Render_Debug();
	GI->Present();

	++m_iNumDraw;

	if (GI->Get_CurrentLevel() != LEVEL_LOADING)
	{
		if (m_fTimeAcc >= 1.f)
		{
			wsprintf(m_szFPS, TEXT("fps : %d"), m_iNumDraw);
			m_iNumDraw = 0;
			m_fTimeAcc = 0.f;
		}

		SetWindowText(g_hWnd, m_szFPS);
	}
	

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eLevelID, const wstring& strFolderName)
{
	/* 로고레베릉ㄹ 할당하고 싶었지만. 로고레벨을 위한 로딩레벨을 먼저 생성하여 로딩작업을 수행할꺼야. */
	/* 로딩객체에게 eLevelID라는 내가 실제 할당ㅎ아고 싶었던 레벨열거체를 준거지?! 실제할당하고싶었던 레벨에 자원을 준비라하라고 */
	if (FAILED(GI->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID, strFolderName))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Initialize_Client()
{
	if (FAILED(CEffect_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext, L"../Bin/Export/NonAnimModel/Effect/", L"../Bin/DataFiles/Effect/")))
		return E_FAIL;

	if(FAILED(CParticle_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext, L"../Bin/DataFiles/Particle/")))
		return E_FAIL;

	// Add Fonts
	if (FAILED(GI->Add_Fonts(m_pDevice, m_pContext, L"Default_Bold", L"../Bin/Resources/Font/NiNoKuni_Bold.spritefont")))
		return E_FAIL;
	if (FAILED(GI->Add_Fonts(m_pDevice, m_pContext, L"Default_Medium", L"../Bin/Resources/Font/NiNoKuni_Medium.spritefont")))
		return E_FAIL;
	if (FAILED(GI->Add_Fonts(m_pDevice, m_pContext, L"Default_Light", L"../Bin/Resources/Font/NiNoKuni_Light.spritefont")))
		return E_FAIL;

	
	if (FAILED(CUI_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CCamera_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;


	
	
	
	

	//_vector vEye = XMVectorSet(-135.f, 100.f, -135.f, 1.f);
	//_vector vAt = XMVectorSet(0.f, -2.f, 52.f, 1.f);
	//_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//if (FAILED(GI->Add_ShadowLight(LEVEL_TOOL, vEye, vAt, vUp)))
	//	return E_FAIL;

	//vEye = XMVectorSet(-135.f, 100.f, -135.f, 1.f);
	//vAt = XMVectorSet(0.f, -2.f, 52.f, 1.f);
	//vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//if (FAILED(GI->Add_ShadowLight(LEVEL_TRAIN_STATION, vEye, vAt, vUp)))
	//	return E_FAIL;


	//vEye = XMVectorSet(-100.f, 50.f, 400.f, 1.f);
	//vAt = XMVectorSet(100.f, -10.f, 0.f, 1.f);
	//vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);

	//if (FAILED(GI->Add_ShadowLight(LEVEL_TRAIN, vEye, vAt, vUp)))
	//	return E_FAIL;

	//vEye = XMVectorSet(-100.f, 50.f, 400.f, 1.f);
	//vAt = XMVectorSet(100.f, -10.f, 0.f, 1.f);
	//vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//if (FAILED(GI->Add_ShadowLight(LEVEL_TRAIN_BOSS, vEye, vAt, vUp)))
	//	return E_FAIL;

	//vEye = XMVectorSet(0.f, 100.f, -100.f, 1.f);
	//vAt = XMVectorSet(60.f, -10.f, 115.f, 1.f);
	//vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	//if (FAILED(GI->Add_ShadowLight(LEVEL_FINAL_BOSS, vEye, vAt, vUp)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/* For.Prototype_Component_Renderer */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer_Com = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_AddRef(m_pRenderer_Com);

	///* For.Prototype_Component_VIBuffer_Particle */
	//if (FAILED(m_pGame_Instance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"),
	//	CVIBuffer_Particle::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;
	
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Navigation"),
		CNavigation::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cloth_Terrain*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cloth_Terrain"),
		CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 100, 100))))
		return E_FAIL;

	

	/* For.Prototype_Component_VIBuffer_Trail*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		CVIBuffer_Trail::Create(m_pDevice, m_pContext, 220))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Cube*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Particle */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"),
		CVIBuffer_Particle::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_UI*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_UI.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxCube */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxCube"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCubeTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Mesh_Effect */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Mesh_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_MeshEffect.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Texture_Effect */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Texture_Effect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_TextureEffect.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Trail */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Trail"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Trail.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Model*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Model.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_AnimModel */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_AnimModel.hlsl"), VTXANIMMODEL_DECLARATION::Elements, VTXANIMMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Point_Instance */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Point_Instance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Point_Instance.hlsl"), VTXPOINTINSTANCE_DECLARATION::Elements, VTXPOINTINSTANCE_DECLARATION::iNumElements))))
		return E_FAIL;


	/////////////
	// Texture //
	///////////// For UI
	// 디졸브 텍스쳐
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Dissolve/Dissolve.png")))))
		return E_FAIL;

	if (FAILED(Ready_UI_TextureComponent()))
		return E_FAIL;
	if (FAILED(CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_STATIC)))
		return E_FAIL;

	// Particle Texture && Effect Texture
	if (FAILED(Ready_Effect_TextureComponent()))
		return E_FAIL;

#pragma region Terrain Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SnowLandScape"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/HeightTest/T_Winter_snow_05_D.dds")))))
		return E_FAIL;
#pragma endregion Terrain Texture

	// Texture
	/* For.Prototype_Component_Texture_Effect*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Texture/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Trail*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Trail/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Dissolve*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Dissolve"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Effect/Dissolve/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_NextFog*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_NextFog"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/NextFog/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Loading_BackGround*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BackGround"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Loading_Background/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Loading_Anim*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Anim"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Loading_Anim/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Loading_Icon*/
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Icon"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Loading_Icon/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Cursor */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Cursor"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Cursor/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_GaugeBar */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GaugeBar"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Gauge_Bar/Gauge/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Gauge_BackGround */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Gauge_BackGround"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Gauge_Bar/BackGround/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Character_Icon */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Character_Icon"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Character_Icon/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_BattleStart */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BattleStart"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_Start/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_BattleEnd */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_BattleEnd"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Battle_End/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_Sky */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Sky"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Sky/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Texture_WorldQuest */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WorldQuest"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Quest/"), 0, true))))
	//	return E_FAIL;


	///* For.Prototype_Component_Enmu_Sleep_Break */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Enmu_Sleep_Break"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Enmu_Sleep_Break/"), 0, true))))
	//	return E_FAIL;

	///* For.Prototype_Component_Enmu_Sleep_ToolTip */
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Enmu_Sleep_ToolTip"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Enmu_Sleep_ToolTip/"), 0, true))))
	//	return E_FAIL;



	




	/* For.Prototype_Component_Transform */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		CTransform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_StateMachine */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_RigidBody */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		CRigidBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Sphere_Collider */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Sphere_Collider"),
		CCollider_Sphere::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_AABB_Collider */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_AABB_Collider"),
		CCollider_AABB::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_Component_OBB_Collider */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_OBB_Collider"),
		CCollider_OBB::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* Prototype_Component_PhysXController */
	if(FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"),
		CPhysX_Controller::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	

	return S_OK;
}

HRESULT CMainApp::Ready_UI_TextureComponent()
{
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Veil_Black"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Veil/Veil_Black.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Veil_White"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Veil/Veil_White.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_LoadingWindow_Temp.png")))))
		return E_FAIL;

	// Todo 2023.12.18
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_LoadingWindow_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Characters_New"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Character_New_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Imajinn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Imajinn/UI_Loading_Imajinn_%d.png"), 10))))
		return E_FAIL;
	//

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Characters"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Character_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_CharacterLogos"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Text_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Information"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Info_%d.png"), 3))))
		return E_FAIL;

//	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Logos"),
//		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Logo_%d.png"), 2))))
//		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Logos"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Logo.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Progress_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_ProgressBarBg.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Progress_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_ProgressBar.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Cursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Cursor.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Btn_Basic"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/UI_Btn_HighGreen.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LevelUp_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/LevelUp/UI_Img_LevelUp_Frame01.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_LevelUp_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/LevelUp/UI_Img_LevelUp_FrameBg.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_GamePlay_MapName"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MapName/MapName_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_Evermore"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MapName/Text/UI_MapName_Text_Evermore.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_KingdomHall"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MapName/Text/UI_MapName_Text_KingdomHall.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_IceLand"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MapName/Text/UI_MapName_Text_IceLand.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_WitchForest"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MapName/Text/UI_MapName_Text_WitchForest.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_BG_PCStatus_Resize.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowMenu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/Icon_MenuBar_MainMenu.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowInven"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/Icon_MenuBar_Inventory.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ChangeCamera"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/Btn_Camera_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ShowQuest"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/Btn_Quest_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestWindow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_QuestAcceptWindow.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Btn_Quest"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Btn_Green_Re.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text_QuestAccept"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Text_QuestAccept.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Text_QuestFinish"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Text_QuestFinish.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MainMenu_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MainMenu/UI_MainMenu_Background.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MainMenu_Button"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MainMenu/UI_MainMenu_Btn_%d.png"), 10))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_Close"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/Btn_Close.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SubMenu_CharacterTab"),
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Bin/Resources/UI/GamePlay/MainMenu/SubMenu/Left1/UI_SubMenu_Left1_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SubMenu_EquipmentTab"),
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Bin/Resources/UI/GamePlay/MainMenu/SubMenu/Left2/UI_SubMenu_Left2_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SubMenu_Imajinn"),
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Bin/Resources/UI/GamePlay/MainMenu/SubMenu/Left3/UI_SubMenu_Left3_%d.png"), 8))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SubMenu_Shop"),
		CTexture::Create(m_pDevice, m_pContext,
			TEXT("../Bin/Resources/UI/GamePlay/MainMenu/SubMenu/Right5/UI_SubMenu_Right5_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_BackBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Back.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_CloseBtn_WithOutline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Close_03.png"))))) // 작업 전
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SubMenu_Separator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MainMenu/UI_MainMenu_Separator.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/WorldMap/WorldMapBase.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/WorldMap/UI_WorldMap_Icon_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_BG_HPBar03.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/UI_Skillsection_Bg.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_Btn_Jump"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/UI_Skillsection_Btn_Jump.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_Btn_Roll"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/UI_Skillsection_Btn_Roll.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/UI_ImajinnSlot_BG.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Slot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/UI_Imajinn_Slot.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Emoticon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/Btn_BarterChat_Popup.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Vehicle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/Icon_Vehicle_Default_Ride_Re.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_DefaultBackground"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Default_Background_Deco.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_DefaultBackground_WithCloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Default_Background_Deco_1.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_DefaultBackground_StarEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Default_Background_Star_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental"), // 무기 속성 Texture 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Icon_Weapon_Elemental_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_DefaultAttack"), // 기본 공격용 Texture
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_DefaultAttack_Icon_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame"), // 속성에 따른 Frame Texture 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Weapon_Frame_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Slot"), // 무기 슬롯
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/Weapon_Slot_BG.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Selected"), // if Selected 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Weapon_Selected.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Focused"), // 스킬 쿨타임이 다 찼을 경우 사용됨(용도 변경될 수 있음)
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Weapon_Focused.png"))))) // 작업 전
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Icon_Setting.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Window_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Setting_OptionWindow.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Button"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Setting_Btn_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus_EXP_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/PlayerStatus/EXP/HP_EXPBackground.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus_EXP_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/PlayerStatus/EXP/HP_EXP.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Selected"), // 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Setting_Btn_Selected.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Slider"), // Setting Slider 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Setting_Slider.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Btn_Minus"), // 소리 세부조절 MinusBtn -> 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Btn_Setting_Minus.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Btn_Plus"), // 소리 세부조절 PlusBtn -> 작업 전
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Btn_Setting_Plus.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_Section"), // Audio Section Window
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Setting_Section_0.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_MinusBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Btn_Setting_Minus.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Setting_PlusBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Setting/UI_Btn_Setting_Plus.png")))))
		return E_FAIL;

	// MonsterHPBar
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Elemtental_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_Elemental_Frame.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_HP_Background.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_HPBar.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_Elemental"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_MonsterTarget_Elemental_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_TitleBar"), // 탭을 누르면 제목아래 깔리는 TitleBar
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_TitleBar.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_InteractionBtn"), // 상호작용 버튼
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/Interaction/UI_Btn_InteractionBg.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_InteractionIcon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/Interaction/UI_Icon_Interaction_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_TitleLine"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_TitleBar.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DefaultBackground_Cloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Default_Background_Cloud.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_Dialogue_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_MiniWindow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_miniDialogue_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_FlipArrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_Arrow_DialogFlip.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_FlipBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_Btn_DialogFlip.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_TitleText"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Title/UI_Text_%d.png"), 6))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_PortraitFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_MIniDialog_Portrait.png")))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Effect_TextureComponent()
{
	// Particle_Mouse_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Particle_Mouse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Mouse/"), 0, true))))
		return E_FAIL;


	// Aura_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Aura"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Aura/"), 0, true))))
		return E_FAIL;

	// Crack_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Crack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Crack/"), 0, true))))
		return E_FAIL;

	// Decal_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Decal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Decal/"), 0, true))))
		return E_FAIL;

	// Fire_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Fire"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Fire/"), 0, true))))
		return E_FAIL;

	// Flare_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Flare"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Flare/"), 0, true))))
		return E_FAIL;
 
	// Glow_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Glow/"), 0, true))))
		return E_FAIL;

	// Hit_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Hit"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Hit/"), 0, true))))
		return E_FAIL;

	// Ice_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Ice"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Ice/"), 0, true))))
		return E_FAIL;

	// Image_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Image"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Image/"), 0, true))))
		return E_FAIL;

	// Mask_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Mask/"), 0, true))))
		return E_FAIL;

	// MeshTrail_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_MeshTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/MeshTrail/"), 0, true))))
		return E_FAIL;

	// Noise_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Noise/"), 0, true))))
		return E_FAIL;

	// Object_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Object"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Object/"), 0, true))))
		return E_FAIL;

	// Ring_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Ring"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Ring/"), 0, true))))
		return E_FAIL;

	// Slash_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Slash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Slash/"), 0, true))))
		return E_FAIL;

	// Smoke_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Smoke"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Smoke/"), 0, true))))
		return E_FAIL;

	// SubUV_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_SubUV"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/SubUV/"), 0, true))))
		return E_FAIL;

	// SwordTrail_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_SwordTrail"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/SwordTrail/"), 0, true))))
		return E_FAIL;

	// UI_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_UI"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/UI/"), 0, true))))
		return E_FAIL;

	// Water_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Water"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Water/"), 0, true))))
		return E_FAIL;

	// Wind_Texture
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Wind"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Wind/"), 0, true))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{	
	__super::Free();
	Safe_Release(m_pRenderer_Com);

	CCamera_Manager::GetInstance()->DestroyInstance();
	CEffect_Manager::GetInstance()->DestroyInstance();
	CParticle_Manager::GetInstance()->DestroyInstance();
	CImGui_Manager::GetInstance()->DestroyInstance();
	CPicking_Manager::GetInstance()->DestroyInstance();
	CUI_Manager::GetInstance()->DestroyInstance();
	CPart_Manager::GetInstance()->DestroyInstance();
	CGameInstance::Release_Engine();
}
