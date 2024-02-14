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
#include "Item_Manager.h"
#include "Skill_Manager.h"
#include "UIDamage_Manager.h"
#include "Grandprix_Manager.h"
#include "Quest_Manager.h"
#include "TowerDefence_Manager.h"
#include "UIMinigame_Manager.h"
#include "Buff_Manager.h"
#include "Riding_Manager.h"
#include "Inventory_Manager.h"
#include "CurlingGame_Manager.h"
#include "Grandprix_Manager.h"

#include "Game_Manager.h"
#include "Character_Manager.h"
#include "Weapon_Manager.h"

#include "Camera_Group.h"

#include "Pool.h"
#include "Character_Biplane_Bullet.h"
#include "Enemy_Biplane_Bullet.h"
#include "Enemy_GuidedMissile.h"
#include "Enemy_Biplane_Feather.h"

#ifdef _DEBUG
// #include <vld.h>
#endif

CMainApp::CMainApp()	
{
}


HRESULT CMainApp::Initialize()
{
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
	
	// Set Start Type
	{
		g_eStartLevel = LEVELID::LEVEL_LOGO;					/* 시작 레벨 타입 */

		g_iStartQuestLevel = QUEST_LEVEL::QL_1_INTRO_TOUR;		/* 시작 퀘스트 레벨 */

		g_eLoadCharacter = LOAD_CHARACTER_TYPE::ALL_CH;			/* 모델 로드할 캐릭터 타입 */

		g_ePlayCharacter = LOAD_CHARACTER_TYPE::SWORDMAN_CH;	/* 게임 플레이 캐릭터 타입 */
	}

	// Open Level
	if (FAILED(Open_Level((LEVELID)g_eStartLevel, L"Final_Boss")))
		return E_FAIL;

	// Set UI Cursor
	if (LEVELID::LEVEL_TOOL != (LEVELID)g_eStartLevel) 
	{
		CUI_Manager::GetInstance()->Ready_Cursor();
		ShowCursor(false);
	}

	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if(LEVELID::LEVEL_ICELAND == GI->Get_CurrentLevel())
		CCurlingGame_Manager::GetInstance()->Tick(fTimeDelta);

	CQuest_Manager::GetInstance()->Tick(fTimeDelta);
	CUI_Manager::GetInstance()->Tick(fTimeDelta);
	GI->Priority_Tick(fTimeDelta); /* 카메라에서 플레이어 포지션 세팅하기 위해, 틱에서만 GI를 게임매니저보다 먼저 업데이트*/

	CBuff_Manager::GetInstance()->Tick(fTimeDelta);
	CGame_Manager::GetInstance()->Tick(fTimeDelta);
	CTowerDefence_Manager::GetInstance()->Tick(fTimeDelta);
	GI->Tick(fTimeDelta); 

	/* ====================================================== */

	if (LEVELID::LEVEL_ICELAND == GI->Get_CurrentLevel())
		CCurlingGame_Manager::GetInstance()->LateTick(fTimeDelta);

	CQuest_Manager::GetInstance()->LateTick(fTimeDelta);
	CUI_Manager::GetInstance()->LateTick(fTimeDelta);
	CGame_Manager::GetInstance()->LateTick(fTimeDelta);
	CTowerDefence_Manager::GetInstance()->LateTick(fTimeDelta);
	GI->LateTick(fTimeDelta);
	
	
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
	if (FAILED(CParticle_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext, L"../Bin/DataFiles/Vfx/Particle/")))
		return E_FAIL;

	if (FAILED(CEffect_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext, L"../Bin/Export/NonAnimModel/Effect/", L"../Bin/DataFiles/Vfx/Effect/", L"../Bin/DataFiles/Vfx/Decal/")))
		return E_FAIL;

	if (FAILED(CItem_Manager::GetInstance()->Reserve_Manager(L"../Bin/DataFiles/Item/")))
		return E_FAIL;

	if (FAILED(CBuff_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
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

	if (FAILED(CUIDamage_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CGrandprix_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CUIMinigame_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CRiding_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	if (FAILED(CInventory_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;


	if (FAILED(Ready_CameraObject()))
		return E_FAIL;	

	LIGHTDESC LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vTempDirection = Vec3(-0.45f, -0.445f, -0.745f);
	LightDesc.vTempColor = Vec3(1.f, 1.f, 1.f);
	LightDesc.vAmbientLowerColor = Vec3(0.5f, 0.5f, 0.5f);
	LightDesc.vAmbientUpperColor = Vec3(1.0f, 1.0f, 1.0f);
	//LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(GI->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	// ShadowLight
	{
		// ShadowLightView
		{
			_vector vEye = XMVectorSet(30.f, 50.f, 0.f, 1.f);
			_vector vAt = XMVectorSet(3.f, 0.f, 0.f, 1.f);
			_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (FAILED(GI->Add_ShadowLight(LEVEL_LOBBY, vEye, vAt, vUp)))
				return E_FAIL;

			vEye = XMVectorSet(30.f, 250.f, 0.f, 1.f);
			vAt = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (FAILED(GI->Add_ShadowLight(LEVEL_EVERMORE, vEye, vAt, vUp)))
				return E_FAIL;

			vEye = XMVectorSet(1.f, 150.f, 0.f, 1.f);
			vAt = XMVectorSet(0.f, 0.f, 0.f, 1.f);
			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (FAILED(GI->Add_ShadowLight(LEVEL_KINGDOMHALL, vEye, vAt, vUp)))
				return E_FAIL;

			vEye = XMVectorSet(-383.130f, 302.242f, -54.922f, 1.f);
			vAt = XMVectorSet(1667.000f, -1171.000f, 578.000f, 1.f);
			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (FAILED(GI->Add_ShadowLight(LEVEL_ICELAND, vEye, vAt, vUp)))
				return E_FAIL;

			vEye = XMVectorSet(1.f, 300.f, 0.f, 1.f);
			vAt = XMVectorSet(3.f, 0.f, 0.f, 1.f);
			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (FAILED(GI->Add_ShadowLight(LEVEL_WITCHFOREST, vEye, vAt, vUp)))
				return E_FAIL;

			// 
			vEye = XMVectorSet(-215.f, 400.f, 200.f, 1.f);
			vAt = XMVectorSet(590.f, -1200.f, 3.f, 1.f);
			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
			if (FAILED(GI->Add_ShadowLight(LEVEL_TOOL, vEye, vAt, vUp)))
				return E_FAIL;
		}

		// UIMesh Light
		LIGHTDESC LightUIDesc;
		ZeroMemory(&LightUIDesc, sizeof(LIGHTDESC));
		LightUIDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
		LightUIDesc.vTempDirection = Vec3(-0.45f, -0.445f, -0.745f);
		LightUIDesc.vTempColor = Vec3(1.f, 1.f, 1.f);
		LightUIDesc.vAmbientLowerColor = Vec3(0.5f, 0.5f, 0.5f);
		LightUIDesc.vAmbientUpperColor = Vec3(1.0f, 1.0f, 1.0f);
		if (FAILED(GI->Add_Light_UI(m_pDevice, m_pContext, LightUIDesc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	/*if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ScreenEffect"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/ScreenEffect/Destroyer_Break/"), 0, true))))
		return E_FAIL;*/


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

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Ocean_Terrain"),
		CVIBuffer_Ocean::Create(m_pDevice, m_pContext, 88, 88, 1.0f))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Sky_Plane*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Sky_Plane"),
		CVIBuffer_SkyPlane::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_LensFlare*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_LensFlare"),
		CVIBuffer_LensFlare::Create(m_pDevice, m_pContext))))
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

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Grass"),
		CVIBuffer_Grass::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_UI*/
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Grass"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Grass.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
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
	/* For. Prototype_Component_Shader_Water */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Water"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Water.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_Skydome */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Skydome"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Skydome.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_SkyPlane */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Plane"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SkyPlane.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_SkySun */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Sun"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Sun.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	/* For. Prototype_Component_Shader_Ocean */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Ocean"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Ocean.hlsl"), VTXNORTEX_DECLARATION::Elements, VTXNORTEX_DECLARATION::iNumElements))))
		return E_FAIL;


	/* For. Prototype_Component_Shader_Aurora */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Aurora"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Aurora.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPos"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements))))
		return E_FAIL;

	// Mirror
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Mirror"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Mirror.hlsl"), VTXMODEL_DECLARATION::Elements, VTXMODEL_DECLARATION::iNumElements))))
		return E_FAIL;
	// LensFlare
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_LensFlare"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_LensFlare.hlsl"), VTXPOS_DECLARATION::Elements, VTXPOS_DECLARATION::iNumElements))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DissolveWeapon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/Noise"), 0, true))))
		return E_FAIL;

	/////////////
	// Texture //
	///////////// For UI

	if (FAILED(Ready_UI_TextureComponent()))
		return E_FAIL;

	if (FAILED(CUI_Manager::GetInstance()->Ready_UIPrototypes(LEVELID::LEVEL_STATIC)))
		return E_FAIL;
	if (FAILED(CUIDamage_Manager::GetInstance()->Ready_DamageNumberPrototypes()))
		return E_FAIL;

	// Particle Texture && Effect Texture
	if (FAILED(Ready_Effect_TextureComponent()))
		return E_FAIL;


	// Texture
	/* For.Prototype_Component_WaterNormal */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/Water_normal1.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/others_0020_normal_directx_2k.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/T_WaterNormal2.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/T_WaterNormal_2.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Normal5"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/T_WaterNormal.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Wave_Normal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/T_SmallWaves_N.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Wave_Normal2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/waves2.dds")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Wave_Height"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/T_LargeWaves_H.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/others_0020_color_2k.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_HeightMap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/others_0020_height_2k.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Water_CubeMap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/AnimModel/Map/Water/CloudyHillsCubemap2.dds")))))
		return E_FAIL;

	// Grass Shade && Wind
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Grass_WindMap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/GrassPlane/Grass_WindMap.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Grass_ShadeMap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/GrassPlane/Blade_Shade.png")))))
		return E_FAIL;

	// Moon
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Moon_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/Skydom/T_Nightmare_Moon_01_D.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Moon_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/Skydom/T_Archipelago_Moon_01_S.png")))))
		return E_FAIL;

	// Grass Mask
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Grass_MaskMap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/GrassPlane/Witch_Grass_MaskMap.dds")))))
		return E_FAIL;

	// FireTex
	//if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FireTex"),
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/InvasionEvermore/Firetex.dds")))))
	//	return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FireDiffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/Fire/FireDiffuse.dds")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FireNoise"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/Fire/FireNoise.dds")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FireAlpha"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/Fire/FireAlpha.dds")))))
		return E_FAIL;

	/* For.Prototype_Component_Sky */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Cloud"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/SkyDom/Cloud3.dds")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Cloud2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/SkyDom/Cloud4.dds")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Aurora"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/SkyDom/aurora.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Sky_Aurora2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/SkyDom/aurora2.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Gradation_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/SkyDom/GradationDissolve.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_LensFlare_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/SkyDom/LensFlare/flare%d.png"), 7))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_GrassMask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Export/NonAnimModel/Map/Common/Plants/SM_Common_grass_01_Mask.png")))))
		return E_FAIL;



	/* For.Prototype_Component_Trail */
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Diffuse"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/TrailEffect/Diffuse/"), 0, true))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Alpha"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/TrailEffect/Alpha/"), 0, true))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Trail_Distiortion"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/Effect/TrailEffect/Distortion/"), 0, true))))
		return E_FAIL;

	
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_PixelArts"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Texture/PixelArt/"), 0, true))))
		return E_FAIL;
	



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
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Cursor"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Cursor_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Veils"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Veil/Veil_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_LoadingWindow_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Characters_New"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Character_New_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Imajinn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/Imajinn/UI_Loading_Imajinn_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_CharacterLogos"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Text_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Progress_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_ProgressBarBg.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Progress_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_ProgressBar.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Loading_Logos"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Loading/UI_Loading_Logo.png")))))
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

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_PlayerStatus"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_BG_PCStatus_Resize_Temp.png")))))
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

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WorldMap_WorldMapPortrait"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/WorldMap/UI_WorldMap_Character_Icon_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_BG_HPBar03_Resize.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_PlayerHP_GaugeBar_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_BG_Under_HPBar03_Resize.png")))))
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/Btn_BarterChat_Popup_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Vehicle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/Icon_Vehicle.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_Vehicle_FXTexture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/Icon_Vehicle_FxTexture.png")))))
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

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Icon_Weapon_Elemental_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_DefaultAttack"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_DefaultAttack_Icon_%d.png"), 3))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Weapon_Frame_%d.png"), 5))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Elemental_Frame_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Weapon_Frame_Mask.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Slot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/Weapon_Slot_BG.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Selected"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/UI_Weapon_Frame_Usable.png")))))
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_HPBar_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MonsterHP_WhiteBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_Back_HPBar.png")))))
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
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_InteractionWorld"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/Interaction/UI_Btn_InteractionIcon_%d.png"), 3))))
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
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_miniDialogue_Resize_%d.png"), 3))))
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
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_PortraitCharacter"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_MiniDialog_Portrait_%d.png"), 2))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_LineBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Coustume_WindowBackground.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Clicked_Buttons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Btn_Clicked_%d.png"), 6))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Unclicked_Buttons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Btn_Unclicked_TextVer_%d.png"), 6))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_ChangeBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_ChangeBtn_%d.png"), 2))))
		return E_FAIL;

	// CoolTime용 MaskTex
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_CoolTime_MaskTexture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/CoolTime/UI_CoolTime_0.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_ClassicSkills"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/ClassicSkill/UI_Icon_Skill_Normal_%d.png"), 9))))
		return E_FAIL;
	// ClassicSkill용 MaskTex
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_ClassicSkill_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/ClassicSkill/UI_Icon_Skill_Mask_Temp.png")))))
		return E_FAIL;

	// SpecialSkill
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_SpecialSkills"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/SpecialSkill/UI_Icon_Special_Skill_%d.png"), 9))))
		return E_FAIL;
	// SpecialSkill용 MaskTex
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSection_SpecialSkill_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/SpecialSkill/UI_Icon_Skill_Mask.png")))))
		return E_FAIL;  

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuickSlot_Item_Bg"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Inven_QuickSlot.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuickSlot_Item"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/UI_Btn_Inven_QuickSlot_Item_%d.png"), 3))))
		return E_FAIL;

	// Quest Frame
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Quest_MiniFrame_Resize_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Finished"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Quest_Finish_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_QuestPopUp_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Quest_MiniPopupWindow.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dialogue_FlipBtn_Selected"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_Btn_DialogFlip_Selected.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSeection_Classic_ElementalFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/ClassicSkill/UI_Icon_ClssicSkill_ElementalFrame.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillSeection_Special_ElementalFrame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/SpecialSkill/UI_Icon_SpecialSkill_ElementalFrame.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_CameraPopup"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Camera/UI_Notice_Camera_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillPopup"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Announce/UI_Notice_Skill_0.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Effect_CameraPopup"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Camera/UI_Effect_Flare_Blue.png")))))
		return E_FAIL;

	// DamageNum Tex
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Blue"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_Blue_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Gold"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_Gold_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Red"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_Red_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_White"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_White_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_WhiteGold"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_WhiteYellow_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Yellow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_Yellow_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Green"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_Green_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_DamageNumber_Purple"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Num_Purple_%d.png"), 10))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Critical"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Img_Font_Critical_%d.png"), 4))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Miss"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Img_Font_Miss.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Damage_Dodge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/DamageNum/Img_Font_Dodge.png")))))
		return E_FAIL;

	// Monster WorldHP
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHPBars"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_WorldUI_HPBar_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_BackBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_WorldUI_BackHPBar.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_WorldUI_HPBar_Frame.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_TargetArrow_Left"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_Target_Arrow_Left.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_WorldHP_TargetArrow_Right"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/UI_Monster_Target_Arrow_Right.png")))))
		return E_FAIL;

	// Emoticon
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/UI_Emoticon_Window_Temp.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_Button"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Icon_Emoticon_FrameMini_%d.png"), 8))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_SpeechBalloon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/UI_SpeechBalloon.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_1"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_0_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_2"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_1_%d.png"), 14))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_3"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_2_%d.png"), 14))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_4"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_3_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_5"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_4_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_6"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_5_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_7"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_6_%d.png"), 16))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Emoticon_BalloonEmoticon_8"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/Emoticons/Img_Emoticon_7_%d.png"), 16))))
		return E_FAIL;

	// Inventory
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_LineBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Inventory_WindowBackground.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Decoline"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Inventory_Decoline.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_UnSelectedTab"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Tab_Unselected_%d.png"), 6))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_SelectedTab"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Tab_Selected_%d.png"), 6))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_TabFXTexture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Fx_Selected.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_InvenSlots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Inventory_ItemSlot_%d.png"), 12))))
		return E_FAIL;

	// SkillWindow
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_SubMenu"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/Skill_SubMenuSection.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_LineBox"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_LineBox.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Select_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_Select_Arrow.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_UnSelected_SubMenuBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_TitleBtn_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Selected_SubMenuBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_TitleBtn_Selected_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Titles"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/Skill_Title_%d.png"), 4))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_ClassSkillSlot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_SkllSlot_Class_%d.png"), 9))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_BurstSkillSlot"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_SkillSlot_Burst_%d.png"), 15))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_ClassSkill_Separator"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_ClassSkill_Separator.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_ActiveSkillSlot"), // 0, 1, 2 소드맨 3, 4, 5 ~
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_SkllSlot_Active_%d.png"), 9))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_SkillSlot_FXTexture"), // 선택시 Glow Tex
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillSlot__Glow_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Desc_Swordsman"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_Swordsman_SkillDesc_%d.png"), 7))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Desc_Destroyer"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_Destroyer_SkillDesc_%d.png"), 7))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SkillWindow_Desc_Engineer"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Skill/UI_SkillWindow_Engineer_SkillDesc_%d.png"), 7))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Swordsman_Clothes"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Swordsman_Cloth_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Engineer_Clothes"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Engineer_Cloth_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Destroyer_Clothes"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Destroyer_Cloth_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Swordsman_HairAcc"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Swordsman_HairAcc_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Engineer_HairAcc"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Engineer_HairAcc_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Destroyer_HairAcc"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Destroyer_HairAcc_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Swordsman_Weapon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Swordsman_Weapon_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Engineer_Weapon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Engineer_Weapon_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Destroyer_Weapon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Slot_Destroyer_Weapon_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Slot_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Coustume_Slot_Glow.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Costume_Announce_Install"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Costume/UI_Costume_Announce.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Common_Btn_Show_Minimap"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Common/Btn_Map_%d.png"), 2))))
		return E_FAIL;

	// Boss HPBar
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_BossInfo"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_Boss_HPBar_Info_RightAlignment_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_BossHPBar_Background.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Gauge_Back"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_BossHPBar_BackGauge.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Gauge_Glow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_BossHPBar_Gauge_FX_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_BossHPBar_Gauge"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_BossHPBar_Gauge_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Dissolve"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/DissolveTex.dds")))))
		return E_FAIL;

	// Milepost
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Flag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Etc/UI_Target_Position.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_TargetPosition_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Etc/UI_Target_Rotation_1.png")))))
		return E_FAIL;

	// Minimap
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_Frame"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Minimap/UI_Frame_MiniMap.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_FrameArrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Minimap/UI_Icon_Minimap_TrackDirectionArrow.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_Mask"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Minimap/UI_Minimap_MaskTexture.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Minimap/UI_MiniMap_Background.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Map_Minimap_Icons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Map/Minimap/UI_Icon_Minimap_%d.png"), 5))))
		return E_FAIL;

	// NameTag
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Lobby_NameTag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/NameTag/UI_Lobby_NameTag_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Lobby_NameTag_FX"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Lobby/NameTag/UI_Lobby_NameTag_FX_%d.png"), 3))))
		return E_FAIL;

	// Weapons for Slot
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_SKillSection_Weapon_Icons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/WeaponSlot/Weapon/UI_Weapon_%d.png"), 9))))
		return E_FAIL;

	// Boss NameTag
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_NameTag"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_Boss_NameTag_%d.png"), 7))))
		return E_FAIL;
//	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_NameTag_Stellia"),
//		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_Boss_NameTag_Stellia.png")))))
//		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_NameTag_Stellia"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/UI_Boss_NameTag_Stellia_%d.png"), 20))))
		return E_FAIL;

	// NPC SpeechBalloon
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_NPC_Default_SpeechBalloon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/SpeechBalloon/UI_SpeechBalloon_Resize.png")))))
		return E_FAIL;

	// 퀘스트 완료창(보상)
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Quest_Finish_Popup"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_Quest_Finish_%d.png"), 3))))
		return E_FAIL;

	// KeyInput Tutorial Window
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_KeyInput_TutorialWindow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/UI_Default_KeyInput_Tutorial.png")))))
		return E_FAIL;

	// QuestRewards Icon
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Quest_Rewards_Icons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Quest/UI_QuestRewards_%d.png"), 4))))
		return E_FAIL;

	// MapName Text(Renewal)
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MapName_Text_Renewal"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MapName/Text_Rework/UI_MapName_Text_%d.png"), 16))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_MiniDialogue_Battle"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Contents/Dialogue/UI_Battle_Dialogue_Renewal_1.png")))))
		return E_FAIL;

	// Vignette
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Vignette_Ice"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Vignette/UI_Vignette_Icy_Blur_%d.png"), 5))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Vignette_Fire"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Default/Vignette/UI_Vignette_Fire.png")))))
		return E_FAIL;

	// SettingWindow
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_OpenBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Btn.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Window"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Window_Menu.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Tab_Audio"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Menu_Audio_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Tab_Game"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Menu_Game_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Tab_Graphic"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Menu_Graphic_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Graphic_Slots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Graphic_Slot_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Camera_Slots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Camera_Slot_%d.png"), 6))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_RadioBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_CheckBox_%d.png"), 2))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Buttons"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Button_Unselected_%d.png"), 4))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Button_FX"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Button_Selected_FX.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Slider"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Slider.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Arrow"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Select_Arrow.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_InGame_Setting_Camera_ResetBtn"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Setting/UI_Setting_InGame_Reset.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_PlayerSlots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/UI_Imajinn_PlayerSlot_Renewal_%d.png"), 3))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_ImajinnSection_PlayerSlot_Selected"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/ImajinnSlot/UI_ImajinnSelect.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_AddItem_Popups"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/PlayerStatus/Item/UI_AddItem_PopUp_%d.png"), 7))))
		return E_FAIL;

//	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Recommend"),
//		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/UI_Recommend_Arrow.png")))))
//		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_WeaponSection_Recommend"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/SkillSection/UI_Recommend_Arrow_1.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Timer"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_Timer_0.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Timer_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_Timer_1.png")))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_DamageBar_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_DamageBar_0.png")))))
		return E_FAIL;
//	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_DamageBar"),
//		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_Damage_Gradient_%d.png"), 8))))
//		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_DamageBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_DamageBar_1.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Gauge_Background"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_Gauge_Background_2.png")))))
		return E_FAIL;
	if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Boss_Stellia_Gauge_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/MonsterStatus/Boss/Stellia/UI_Stellia_Gauge_Bar_2.png")))))
		return E_FAIL;

	/*if (FAILED(GI->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Inventory_Etc_Slots"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/GamePlay/Tab/Tab_Inventory/UI_Item_Slot_Etc_%d.png"), 5))))
		return E_FAIL;*/

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

HRESULT CMainApp::Ready_CameraObject()
{
	/* Free */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::FREE;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Free_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_Free::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);
			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
		pCamera->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		pCamera->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* Follow */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::FOLLOW;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Follow_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_Follow::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);
			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
		pCamera->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		pCamera->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* CutScene Map */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::CUTSCENE_MAP;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_CutScene_Map_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_CutScene_Map::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
		pCamera->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		pCamera->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* CutScene Boss */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::CUTSCENE_BOSS;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_CutScene_Boss_Default;
			tDesc.fAspect = (_float)g_iWinSizeX /(_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_CutScene_Boss::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
		pCamera->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		pCamera->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* Action */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::ACTION;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Action_Lobby;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_Action::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
	}

	/* Top */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::TOP;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_Top::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);

			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
	}

	/* Quater */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::QUATER;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_Quater::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);
			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
		pCamera->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		pCamera->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}

	/* CurlingGame */
	{
		CCamera* pCamera = nullptr;
		CAMERA_TYPE eType = CAMERA_TYPE::CAMERA_CURLING;
		CCamera::PROJ_DESC tDesc;
		{
			tDesc.tLerpFov.fCurValue = Cam_Fov_CurlingGame_Default;
			tDesc.fAspect = (_float)g_iWinSizeX / (_float)g_iWinSizeY;
			tDesc.fNear = 0.2f;
			tDesc.fFar = 1000.f;

			pCamera = CCamera_CurlingGame::Create(m_pDevice, m_pContext, CameraWstringNames[eType]);
			if (nullptr == pCamera)
				return E_FAIL;

			CCamera_Manager::GetInstance()->Add_Camera(eType, pCamera);

			if (FAILED(pCamera->Initialize(&tDesc)))
			{
				return E_FAIL;
			}
		}
		pCamera->Set_Key(eType);
		pCamera->Get_Transform()->Set_State(CTransform::STATE::STATE_POSITION, Vec4(0.f, 10.f, -10.f, 1.f));
		pCamera->Get_Transform()->LookAt(Vec4{ 0.f, 0.f, 0.f, 1.f });
	}
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
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pRenderer_Com);

	CPool<CCharacter_Biplane_Bullet>::Free();
	CPool<CEnemy_Biplane_Bullet>::Free();
	CPool<CEnemy_GuidedMissile>::Free();
	CPool<CEnemy_Biplane_Feather>::Free();

	CRiding_Manager::GetInstance()->DestroyInstance();
	CGrandprix_Manager::GetInstance()->DestroyInstance();
	CTowerDefence_Manager::GetInstance()->DestroyInstance();
	CQuest_Manager::GetInstance()->DestroyInstance();
	CEffect_Manager::GetInstance()->DestroyInstance();
	CParticle_Manager::GetInstance()->DestroyInstance();
	CImGui_Manager::GetInstance()->DestroyInstance();
	CPicking_Manager::GetInstance()->DestroyInstance();
	CUIDamage_Manager::GetInstance()->DestroyInstance();
	CUI_Manager::GetInstance()->DestroyInstance();
	CUIMinigame_Manager::GetInstance()->DestroyInstance();
	CItem_Manager::GetInstance()->DestroyInstance();
	CInventory_Manager::GetInstance()->DestroyInstance();
	CWeapon_Manager::GetInstance()->DestroyInstance();
	CSkill_Manager::GetInstance()->DestroyInstance();
	CBuff_Manager::GetInstance()->DestroyInstance();
	CCharacter_Manager::GetInstance()->DestroyInstance();
	CGame_Manager::GetInstance()->DestroyInstance();
	CCurlingGame_Manager::GetInstance()->DestroyInstance();
	CGameInstance::Release_Engine();
}
