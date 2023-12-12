#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Camera_Tool.h"
#include "Camera_Main.h"
#include "FileUtils.h"
#include "Sword.h"

#include "Dummy.h"
#include "Terrain.h"
#include "BackGround.h"
#include "MapHeaderGroup.h"

#include <filesystem>
#include "Utils.h"
#include "ImGui_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character_Witch.h"

#include "UI_Logo_Background.h"
#include "UI_Flare.h"

#pragma region Kang
#include "Stellia.h"
#pragma endregion

#include "Particle.h"

_bool CLoader::g_bFirstLoading = false;
CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

// unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY ThreadEntry(void* pArg)
{
	
	CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel, const wstring& strFolderName)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;
	m_strFolderName = strFolderName;
	/* 새로운 스레드를 만들자. */
	/* 스레드 : 내 코드를 읽어주는 하나의 흐름? */
	/* 3 : 생성한 스레드가 호출해야하는 함수의 주소 */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;
	

	switch (m_eNextLevel)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;

	case LEVEL_TEST:
		hr = Loading_For_Level_Test();
		break;

	case LEVEL_TOOL:
		hr = Loading_For_Level_Tool();
		break;
	}

	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	if (FAILED(GI->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Background_Frames"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/Logo/LogoFrames/Logo_Texture_%d.jpg"), 274))))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo_Flare"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/UI/UI_Effect_Flare_Yellow.png"), 1))))
		return E_FAIL;

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo_Background"),
		CUI_Logo_Background::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_UI_Logo_Flare"),
		CUI_Flare::Create(m_pDevice, m_pContext), LAYER_UI)))
		return E_FAIL;



	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Test()
{
	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");


	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Background", CBackGround::Create(m_pDevice, m_pContext), LAYER_BACKGROUND)))
		return E_FAIL;
	
	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Witch", CCharacter_Witch::Create(m_pDevice, m_pContext, L"Witch_Character", CCharacter::CHARACTER_TYPE::WITCH), LAYER_CHARACTER)))
		return E_FAIL;

	CMonster::MONSTER_STAT statDesc;
	statDesc.fHp = 100;
	statDesc.fMaxHp = 100;
	statDesc.fMp= 100;
	statDesc.fMaxMp = 100;


	if (FAILED(GI->Add_Prototype(L"Prorotype_GameObject_Stellia", CStellia::Create(m_pDevice, m_pContext, TEXT("Stellia"), statDesc), LAYER_MONSTER)))
		return E_FAIL;

	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Witch", CModel::TYPE_ANIM, L"../Bin/Export/Temp/", L"Witch")))
		return E_FAIL;
	

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}





HRESULT CLoader::Loading_For_Level_Tool()
{
	/* For.ImGuiManager */
	m_strLoading = TEXT("툴 작업을 준비중입니다.");


	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.Prototype_Component_Shader_Model */


	m_strLoading = TEXT("객체 원형을 로딩 중 입니다.");


	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Dummy"),
		CDummy::Create(m_pDevice, m_pContext, TEXT("Dummy")), LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;

 	if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Terrain"),
		CTerrain::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_TERRAIN)))
		return E_FAIL;



	if (FAILED(Loading_Proto_AllObjects(L"../Bin/Export/NonAnimModel/Map/")))
		return E_FAIL;
		
	// 테스트
	 CParticle::PARTICLE_DESC ParticleInfo = {}; // 파일로 정보 로드?

	 ParticleInfo.iNumEffectCount = 50; // 개수

	 ParticleInfo.bLoop = false;

	 ParticleInfo.bSameRate = true; // 정비율
	 ParticleInfo.fScale = _float2(1.f, 2.f); // 스케일

	 ParticleInfo.fRange = _float3(0.f, 0.f, 0.f); // 분포범위

	 ParticleInfo.vVelocityMin = _float3(-1.0f, -1.0f, -1.0f);
	 ParticleInfo.vVelocityMax = _float3(1.0f, 1.0f, 1.0f);

	 ParticleInfo.fSpeed = _float2(3.f, 6.f);
	 ParticleInfo.fLifeTime = _float2(5.f, 10.f);
	 ParticleInfo.fBoxMin;
	 ParticleInfo.fBoxMax;
	 if(FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Particle"),
		 CParticle::Create(m_pDevice, m_pContext, TEXT("Particle"), TEXT("Particle_Test"), ParticleInfo), LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	m_strLoading = TEXT("모델을 로딩 중 입니다.");
	_matrix		PivotMatrix = XMMatrixIdentity();

#pragma region SJY_MapObject
	//if (FAILED(GI->Add_Prototype(TEXT("Prototype_GameObject_Building"),
	//	CBuilding::Create(m_pDevice, m_pContext, TEXT("Building"), OBJ_TYPE::OBJ_BUILDING), LAYER_TYPE::LAYER_BUILDING)))
	//	return E_FAIL;
#pragma endregion

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Load_Navi_Data(const wstring& strNaviFileName)
{
	const wstring strNaviFilePath = L"../Bin/DataFiles/Map/" + strNaviFileName + L"/" + strNaviFileName + L".nav";
	CNavigation* pPrototypeNavigation = dynamic_cast<CNavigation*>(GI->Find_Prototype_Component(LEVEL_STATIC, L"Prototype_Component_Navigation"));

	if (nullptr == pPrototypeNavigation)
		return E_FAIL;

	if (FAILED(pPrototypeNavigation->Load_NaviData(strNaviFilePath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Load_Map_Data(const wstring& strMapFileName)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strMapFileName + L"/" + strMapFileName + L".map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (i == LAYER_TYPE::LAYER_CAMERA
			|| i == LAYER_TYPE::LAYER_TERRAIN
			|| i == LAYER_TYPE::LAYER_BACKGROUND
			|| i == LAYER_TYPE::LAYER_SKYBOX
			|| i == LAYER_TYPE::LAYER_UI
			|| i == LAYER_TYPE::LAYER_PLAYER
			|| i == LAYER_TYPE::LAYER_PROJECTILE
			|| i == LAYER_TYPE::LAYER_EFFECT
			|| i == LAYER_TYPE::LAYER_TRAIL
			|| i == LAYER_TYPE::LAYER_NPC)
			continue;

		// 2. ObjectCount
		_uint iObjectCount = File->Read<_uint>();

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
			wstring strObjectTag = CUtils::ToWString(File->Read<string>());

			CGameObject* pObj = nullptr;
			if (FAILED(GI->Add_GameObject(m_eNextLevel, i, strPrototypeTag, nullptr, &pObj)))
			{
				MSG_BOX("Load_Objects_Failed.");
				return E_FAIL;
			}

			if (nullptr == pObj)
			{
				MSG_BOX("Add_Object_Failed.");
				return E_FAIL;
			}
			pObj->Set_ObjectTag(strObjectTag);

			CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Get_Transform_Failed.");
				return E_FAIL;
			}

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);

			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));


			for (_uint iCollider = 0; iCollider < CCollider::DETECTION_TYPE::DETECTION_END; iCollider++)
			{
				_uint iColliderCount = File->Read<_uint>();
				for (_uint iObejctColliderCount = 0; iObejctColliderCount < iColliderCount; ++iObejctColliderCount)
				{
					_uint iColliderType = File->Read<_uint>();
					_float3 vColliderOffset = File->Read<_float3>();

					if (iColliderType == CCollider::AABB)
					{
						BoundingBox tBox = File->Read<BoundingBox>();

						CCollider_AABB::AABB_COLLIDER_DESC tDesc;
						ZeroMemory(&tDesc, sizeof tDesc);

						if (nullptr == pObj->Get_Component<CModel>(L"Com_Model"))
							XMStoreFloat4x4(&tDesc.ModePivotMatrix, XMMatrixIdentity());
						else
							XMStoreFloat4x4(&tDesc.ModePivotMatrix, pObj->Get_Component<CModel>(L"Com_Model")->Get_PivotMatrix());

						tDesc.vOffsetPosition = vColliderOffset;
						tDesc.pOwnerTransform = pTransform;
						tDesc.pNode = nullptr;
						tDesc.tBox = tBox;

						pObj->Add_Collider(LEVEL_STATIC, iColliderType, iCollider, &tDesc);
					}
					else if (iColliderType == CCollider::SPHERE)
					{
						BoundingSphere tSphere = File->Read<BoundingSphere>();

						CCollider_Sphere::SPHERE_COLLIDER_DESC tDesc;
						ZeroMemory(&tDesc, sizeof tDesc);

						if (nullptr == pObj->Get_Component<CModel>(L"Com_Model"))
							XMStoreFloat4x4(&tDesc.ModePivotMatrix, XMMatrixIdentity());
						else
							XMStoreFloat4x4(&tDesc.ModePivotMatrix, pObj->Get_Component<CModel>(L"Com_Model")->Get_PivotMatrix());

						tDesc.vOffsetPosition = vColliderOffset;
						tDesc.pOwnerTransform = pTransform;
						tDesc.pNode = nullptr;
						tDesc.tSphere = tSphere;


						pObj->Add_Collider(LEVEL_STATIC, iColliderType, iCollider, &tDesc);
					}
				}
			}
		}
	}

	// MSG_BOX("Map_Loaded.");
	return S_OK;
}

HRESULT CLoader::Loading_Proto_AllObjects(const wstring& strPath)
{
	for (auto& p : std::filesystem::directory_iterator(strPath))
	{
		if (p.is_directory())
		{
			Loading_Proto_AllObjects(p.path().wstring());
		}

 		wstring strFilePath = CUtils::PathToWString(p.path().wstring());
		
		_tchar strFileName[MAX_PATH];
		_tchar strFolderName[MAX_PATH];
		_tchar strExt[MAX_PATH];

		_wsplitpath_s(strFilePath.c_str(), nullptr, 0, strFolderName, MAX_PATH, strFileName, MAX_PATH, strExt, MAX_PATH);

		if (0 == lstrcmp(TEXT(".fbx"), strExt) || 0 == lstrcmp(TEXT(".mesh"), strExt))
		{
			if (strFilePath.find(L"Buildings") != wstring::npos)
			{
				if (FAILED(GI->Add_Prototype(wstring(strFileName),
					CBuilding::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt, OBJ_TYPE::OBJ_BUILDING, CModel::TYPE_NONANIM), LAYER_TYPE::LAYER_BUILDING)))
				{
					return E_FAIL;
				}
			}
			//else if ((strFilePath.find(L"Objects") != wstring::npos) || (strFilePath.find(L"Prop") != wstring::npos))
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CProp::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_PROP)))
			//	{
			//		return E_FAIL;
			//	}
			//}
			//else if (strFilePath.find(L"Grass") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CGrass::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_GRASS)))
			//		return E_FAIL;
			//}
			//else if (strFilePath.find(L"Grounds") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CGround::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_GROUND)))
			//		return E_FAIL;
			//}
			//else if (strFilePath.find(L"Rocks") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CProp::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_GROUND)))
			//		return E_FAIL;
			//}
			//else if (strFilePath.find(L"Tree") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CTree::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_TREE)))
			//		return E_FAIL;
			//}
			//else if (strFilePath.find(L"Road") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CProp::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_ROAD)))
			//		return E_FAIL;
			//}

			//else if (strFilePath.find(L"Mountain") != wstring::npos)
			//{
			//	if (FAILED(GI->Add_Prototype(wstring(strFileName),
			//		CProp::Create(m_pDevice, m_pContext, wstring(strFileName), strFolderName, wstring(strFileName) + strExt), LAYER_TYPE::LAYER_MOUNTAIN)))
			//		return E_FAIL;
			//}
		}

	}
	return S_OK;
}


CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel, const wstring& strFolderName)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel, strFolderName)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);	

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}