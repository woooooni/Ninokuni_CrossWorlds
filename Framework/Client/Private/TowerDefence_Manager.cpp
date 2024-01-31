#include "stdafx.h"
#include "GameInstance.h"
#include "TowerDefence_Manager.h"
#include "Utils.h"

#include "Cannon_Tower.h"
#include "Crystal_Tower.h"
#include "Flame_Tower.h"
#include "Shadow_Tower.h"

#include "Cannon_Ball.h"
#include "Crystal_Ball.h"
#include "Shadow_Ball.h"


#include "Camera_Manager.h"

#include "Picking_Manager.h"
#include "Mesh.h"

#include "Game_Manager.h"
#include "Player.h"
#include "Character.h"
#include "GameObject.h"
#include "DefenceInvasion_Portal.h"

#include "UIMinigame_Manager.h"
#include "Light_Manager.h"
#include "Light.h"

#include "UI_Manager.h"
#include "SkyDome.h"


IMPLEMENT_SINGLETON(CTowerDefence_Manager)

CTowerDefence_Manager::CTowerDefence_Manager()
{

}

HRESULT CTowerDefence_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


	if (FAILED(Ready_Defence_Models()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Defence_Objects()))
		return E_FAIL;

	if (FAILED(Ready_Picking_Texture()))
		return E_FAIL;


	m_bReserved = true;
	m_eCurrentPhase = DEFENCE_NO_RUN;

	return S_OK;
}

void CTowerDefence_Manager::Tick(_float fTimeDelta)
{
	
	switch (m_eCurrentPhase)
	{
	case DEFENCE_NO_RUN:
		Tick_Defence_No_Run(fTimeDelta);
		break;
	case DEFENCE_PREPARE:
		Tick_Defence_Prepare(fTimeDelta);
		break;
	case DEFENCE_PROGRESS:
		Tick_Defence_Progress(fTimeDelta);
		break;
	case DEFENCE_FINISH:
		Tick_Defence_Finish(fTimeDelta);
		break;

	default:
		break;
	}

	
}

void CTowerDefence_Manager::LateTick(_float fTimeDelta)
{
	switch (m_eCurrentPhase)
	{
	case DEFENCE_NO_RUN:
		LateTick_Defence_No_Run(fTimeDelta);
		break;
	case DEFENCE_PREPARE:
		LateTick_Defence_Prepare(fTimeDelta);
		break;
	case DEFENCE_PROGRESS:
		LateTick_Defence_Progress(fTimeDelta);
		break;
	case DEFENCE_FINISH:
		LateTick_Defence_Finish(fTimeDelta);
		break;

	default:
		break;
	}
}

void CTowerDefence_Manager::Prepare_Defence()
{
	// Change_Light Color
	list<CLight*>* pLightLists = GI->Get_LightList();
	for (auto& pLight : *pLightLists)
	{

		LIGHTDESC* pDesc = pLight->Get_ModifyLightDesc();
		m_OriginLights.push_back(*pDesc);

		pDesc->vTempColor = Vec3(0.729f, 0.431f, 1.f);
	}

	// Change SkyDomeColor
	list<CGameObject*>& SkyDomes = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_SKYBOX);
	for (auto& pSkyDome : SkyDomes)
	{
		if (wstring::npos != pSkyDome->Get_PrototypeTag().find(L"Skydome"))
		{
			CSkyDome* pCastSky =  dynamic_cast<CSkyDome*>(pSkyDome);
			if (nullptr != pCastSky)
			{
				m_vOriginSkyCenterColor = pCastSky->Get_CenterColor();
				m_vOriginSkyApexColor = pCastSky->Get_ApexColor();

				pCastSky->Set_CenterColor(Vec4(0.871f, 0.78f, 1.0f, 1.0f));
				pCastSky->Set_ApexColor(Vec4(0.224f, 0.306f, 0.788f, 1.0f));
			}
		}
	}
	
	// Change Particle

	// Change FogColor

	// Off GodRay

	// Off LensFlare



	// Clear Npc
	//list<CGameObject*>& NpcList = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_NPC);
	//for (auto& pNpc : NpcList)
	//{
	//	if (pNpc->Get_PrototypeTag().find(L"TreeGrandfa") != wstring::npos)
	//		continue;
	//
	//	pNpc->Set_Dead(true);
	//}
		

	list<CGameObject*>& AnimalList = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_DYNAMIC);
	for (auto& pAnimal : AnimalList)
		pAnimal->Set_Dead(true);


	if (nullptr != m_pPicked_Object)
	{
		m_pPicked_Object = nullptr;
		Safe_Release(m_pPicked_Object);
		m_pPicked_ObjectTransform = nullptr;
	}

	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_PREPARE;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::QUATER);

	for (auto& pDefenceObject : m_DefenceObjects)
	{
		pDefenceObject->Set_Dead(true);
		Safe_Release(pDefenceObject);
	}
	m_DefenceObjects.clear();

	
	for (auto& m_InvasionPortals : m_InvasionPortals)
	{
		m_InvasionPortals->Set_Dead(true);
		Safe_Release(m_InvasionPortals);
	}
	m_InvasionPortals.clear();

	if (FAILED(Prepare_Portals(XMVectorSet(0.f, 2.f, 0.f, 1.f))))
		return;
	if (FAILED(Prepare_Portals(XMVectorSet(-95.573f, 2.f, 111.546f, 1.f))))
		return;
	if (FAILED(Prepare_Portals(XMVectorSet(105.247f, 2.f, 111.456f, 1.f))))
		return;

	
	m_fAccPrepare = m_fPrepareTime;


	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
	CUIMinigame_Manager::GetInstance()->OnOff_TowerDefence_Select(true);
}

void CTowerDefence_Manager::Start_Defence()
{
	CUIMinigame_Manager::GetInstance()->OnOff_TowerDefence_Select(false);

	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_PROGRESS;
	if (nullptr != m_pPicked_Object)
	{
		m_pPicked_Object = nullptr;
		Safe_Release(m_pPicked_Object);
		m_pPicked_ObjectTransform = nullptr;
	}

	for (auto& pPortal : m_InvasionPortals)
	{
		pPortal->Start_Invasion();
	}
		


	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(true);
}

void CTowerDefence_Manager::Finish_Defence()
{

	// Change_Light

	list<CLight*>* pLightLists = GI->Get_LightList();
	_uint iIndex = 0;

	for (auto& pLight : *pLightLists)
	{

		LIGHTDESC* pDesc = pLight->Get_ModifyLightDesc();
		pDesc->vTempColor = m_OriginLights[iIndex].vTempColor;
		iIndex++;
	}

	// Recover SkyDome Color
	list<CGameObject*>& SkyDomes = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_SKYBOX);
	for (auto& pSkyDome : SkyDomes)
	{
		if (wstring::npos != pSkyDome->Get_PrototypeTag().find(L"Skydome"))
		{
			CSkyDome* pCastSky = dynamic_cast<CSkyDome*>(pSkyDome);
			if (nullptr != pCastSky)
			{
				pCastSky->Set_CenterColor(m_vOriginSkyCenterColor);
				pCastSky->Set_ApexColor(m_vOriginSkyApexColor);
			}
		}
	}

	if (nullptr != m_pPicked_Object)
	{
		m_pPicked_Object = nullptr;
		Safe_Release(m_pPicked_Object);
		m_pPicked_ObjectTransform = nullptr;
	}

	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_FINISH;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FOLLOW);

	for (auto& pDefenceObject : m_DefenceObjects)
	{
		pDefenceObject->Set_Dead(true);
		Safe_Release(pDefenceObject);
	}
	m_DefenceObjects.clear();

	for (auto& pMonster : m_DefenceMonsters)
	{
		pMonster->Set_Dead(true);
		Safe_Release(pMonster);
	}
	m_DefenceMonsters.clear();
	

	for (auto& pPortal : m_InvasionPortals)
	{
		pPortal->Stop_Invasion();
		pPortal->Set_Dead(true);
		Safe_Release(pPortal);
	}
	m_InvasionPortals.clear();

}

void CTowerDefence_Manager::End_Defence()
{
	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_END;

	for (auto& pDefenceObject : m_DefenceObjects)
	{
		pDefenceObject->Set_Dead(true);
		Safe_Release(pDefenceObject);
	}
	m_DefenceObjects.clear();
}

void CTowerDefence_Manager::Set_PickObject(TOWER_TYPE eTowerType)
{
	if (eTowerType >= TOWER_TYPE::TOWER_TYPE_END)	
		return;

	if (nullptr != m_pPicked_Object)
	{
		Safe_Release(m_pPicked_Object);
		m_pPicked_Object = nullptr;
	}

	m_eCurrentTowerType = eTowerType;

	if (TOWER_TYPE::CANNON == eTowerType)
		m_pPicked_Object = dynamic_cast<CDefence_Tower*>(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Cannon")->Clone(nullptr));
	else if(TOWER_TYPE::CRYSTAL == eTowerType)
		m_pPicked_Object = dynamic_cast<CDefence_Tower*>(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Crystal")->Clone(nullptr));	
	else if(TOWER_TYPE::FLAME == eTowerType)	
		m_pPicked_Object = dynamic_cast<CDefence_Tower*>(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Flame")->Clone(nullptr));
	else if(TOWER_TYPE::SHADOW == eTowerType)	
		m_pPicked_Object = dynamic_cast<CDefence_Tower*>(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Shadow")->Clone(nullptr));

	if (nullptr == m_pPicked_Object)
	{
		MSG_BOX("Picked_Object is Null");
		return;
	}

	m_pPicked_ObjectTransform = m_pPicked_Object->Get_Component<CTransform>(L"Com_Transform");


	if(nullptr == m_pPicked_ObjectTransform)
	{
		MSG_BOX("Picked_Object Transform is Null");
		return;
	}

	m_pPicked_Object->Set_Preview(true);
}

void CTowerDefence_Manager::Tick_Defence_No_Run(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::X))
	{
		Prepare_Defence();
	}
}

void CTowerDefence_Manager::Tick_Defence_Prepare(_float fTimeDelta)
{
	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);

	if (nullptr != m_pPicked_Object)
		m_pPicked_Object->Tick(fTimeDelta);

	Picking_Position();

	m_fAccPrepare -= fTimeDelta;
	if (0.f > m_fAccPrepare)
	{
		Start_Defence();
		m_fAccPrepare = m_fPrepareTime;
		return;
	}

	
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::LBTN))
	{
		Picking_Tower();
		return;
	}
		
	if (KEY_TAP(KEY::LBTN))
	{
		Create_Defence_Object();
		return;
	}
		

	if (KEY_TAP(KEY::RBTN))
	{
		Safe_Release(m_pPicked_Object);
		m_pPicked_Object = nullptr;
		m_pPicked_ObjectTransform = nullptr;
		return;
	}

	if (KEY_TAP(KEY::R))
	{
		if (nullptr == m_pPicked_Object || nullptr == m_pPicked_ObjectTransform)
			return;

		m_pPicked_ObjectTransform->Rotation_Acc(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));
		return;
	}


	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::X))
	{
		Start_Defence();
		return;
	}
}

void CTowerDefence_Manager::Tick_Defence_Progress(_float fTimeDelta)
{
	list<CGameObject*>& Monsters = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_MONSTER);

	if (0 >= Monsters.size())
	{
		Finish_Defence();
	}

	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::X))
	{
		Finish_Defence();
	}
}

void CTowerDefence_Manager::Tick_Defence_Finish(_float fTimeDelta)
{
	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::X))
	{
		Prepare_Defence();
	}
}

void CTowerDefence_Manager::LateTick_Defence_No_Run(_float fTimeDelta)
{
}

void CTowerDefence_Manager::LateTick_Defence_Prepare(_float fTimeDelta)
{
	if (nullptr != m_pPicked_Object)
		m_pPicked_Object->LateTick(fTimeDelta);
}

void CTowerDefence_Manager::LateTick_Defence_Progress(_float fTimeDelta)
{
}

void CTowerDefence_Manager::LateTick_Defence_Finish(_float fTimeDelta)
{

}


void CTowerDefence_Manager::Picking_Position()
{
	if (nullptr == m_pPicked_Object || nullptr == m_pPicked_ObjectTransform)
		return;

	GI->Clear_DepthStencil_View();

	if (FAILED(GI->Begin_MRT(m_pContext, L"MRT_FastPicking")))
	{
		MSG_BOX("Begin_MRT Failed. : CTowerDefence_Manager::Picking_Position");
		return;
	}
	
	list<CGameObject*>& PickingObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_GROUND);
	for (auto& pObject : PickingObjects)
	{
		if (FAILED(pObject->Render_Picking()))
		{
			wstring strErrMsg = L"Render Picking Failed. : ";
			strErrMsg += pObject->Get_PrototypeTag();
			MessageBox(nullptr, strErrMsg.c_str(), L"System Message", MB_OK);
			return;
		}	
	}

	
	if (FAILED(GI->End_MRT(m_pContext)))
	{
		MSG_BOX("End_MRT Failed. : CTowerDefence_Manager::Picking_Position");
		return;
	}

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	ZeroMemory(&MappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	Vec2 vMousePos = Vec2(GI->GetMousePos().x, GI->GetMousePos().y);
	m_tPickingBox.left = vMousePos.x;
	m_tPickingBox.right = vMousePos.x + 1;
	m_tPickingBox.top = vMousePos.y;
	m_tPickingBox.bottom = vMousePos.y + 1;
	m_tPickingBox.front = 0;
	m_tPickingBox.back = 1;

	ID3D11Texture2D* pPickingTarget = GI->Get_Texture_FromRenderTarget(L"Target_FastPicking");
	if (nullptr == pPickingTarget)
	{
		MSG_BOX("Target_FastPicking Is Null. : CTowerDefence_Manager::Picking_Position");
		return;
	}

	_int iHash = 0;
	m_pContext->CopySubresourceRegion(m_pPickingTexture, 0, 0, 0, 0, pPickingTarget, 0, &m_tPickingBox);
	m_pContext->Map(m_pPickingTexture, 0, D3D11_MAP_READ, 0, &MappedResource);

	if (MappedResource.pData == nullptr)
	{
		return;
	}
		
	iHash = ((_int*)MappedResource.pData)[0];
	m_pContext->Unmap(m_pPickingTexture, 0);

	
	for (auto& pGround : PickingObjects)
	{
		_int iObjectHash = GI->To_Hash(pGround->Get_ObjectID());
		if (iObjectHash == iHash)
		{
			CTransform* pTransform = pGround->Get_Component<CTransform>(L"Com_Transform");
			CModel* pModel = pGround->Get_Component<CModel>(L"Com_Model");

			if (nullptr == pTransform || nullptr == pModel)
				continue;

			_uint iNumMeshes = pModel->Get_NumMeshes();
			for (auto& pMesh : pModel->Get_Meshes())
			{
				Vec4 vPosition = {};
				if (true == CPicking_Manager::GetInstance()->Is_DefencePicking(pTransform, pMesh, true, &vPosition))
				{
					m_pPicked_ObjectTransform->Set_State(CTransform::STATE_POSITION, vPosition);
					m_pPicked_Object->Set_Install_Possible(true);
					return;
				}
			}
		}
	}
	m_pPicked_Object->Set_Install_Possible(false);
}

void CTowerDefence_Manager::Picking_Tower()
{
	list<CGameObject*>& PickingObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC);

	for (auto& pObject : PickingObjects)
	{
		if (pObject->Get_ObjectType() != OBJ_TYPE::OBJ_DEFENCE_TOWER)
			continue;

		Vec4 vPickingPos = {};
		CTransform* pTransform = pObject->Get_Component<CTransform>(L"Com_Transform");
		CModel* pModel = pObject->Get_Component<CModel>(L"Com_Model");

		for (auto& pMesh : pModel->Get_Meshes())
		{
			if (true == CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, true, &vPickingPos))
			{
				if (nullptr != m_pPicked_ObjectTransform)
				{
					if (nullptr != m_pPicked_Object)
					{
						Safe_Release(m_pPicked_Object);
						m_pPicked_Object = nullptr;
					}
				}

				m_pPicked_Object = dynamic_cast<CDefence_Tower*>(pObject);
				if (nullptr == m_pPicked_Object)
					continue;

				m_pPicked_ObjectTransform = m_pPicked_Object->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == m_pPicked_ObjectTransform)
					continue;

				m_eCurrentTowerType = TOWER_TYPE(m_pPicked_Object->Get_TowerType());
				m_pPicked_Object->Set_Preview(true);
			}
		}
	}
}



HRESULT CTowerDefence_Manager::Create_Defence_Object()
{
	if (nullptr == m_pPicked_Object)
		return S_OK;

	if (false == m_pPicked_Object->Is_Install_Possible())
		return S_OK;

	
	_bool bCanBuy = false;
	switch (m_eCurrentTowerType)
	{
	case TOWER_TYPE::CANNON:
		bCanBuy = CGame_Manager::GetInstance()->Get_Player()->Decrease_Gold(1000);
		break;
	case TOWER_TYPE::CRYSTAL:
		bCanBuy = CGame_Manager::GetInstance()->Get_Player()->Decrease_Gold(1200);
		break;
	case TOWER_TYPE::FLAME:
		bCanBuy = CGame_Manager::GetInstance()->Get_Player()->Decrease_Gold(2000);
		break;
	case TOWER_TYPE::SHADOW:
		bCanBuy = CGame_Manager::GetInstance()->Get_Player()->Decrease_Gold(3000);
		break;
	}
	
	if (false == bCanBuy)
		return S_OK;

	CGameObject* pNewGameObject = m_pPicked_Object->Clone(nullptr);
	if (nullptr == pNewGameObject)
	{
		MSG_BOX("Picked_Object Clone Failed. : CTowerDefence_Manager");
		return E_FAIL;
	}

	CTransform* pNewObjectTransform = pNewGameObject->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pNewObjectTransform)
	{
		Safe_Release(pNewGameObject);
		MSG_BOX("Create_Defence_Object Failed. : CTowerDefence_Manager");
		return E_FAIL;
	}

	pNewObjectTransform->Set_WorldMatrix(m_pPicked_ObjectTransform->Get_WorldMatrix());
	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC, pNewGameObject)))
	{
		Safe_Release(pNewGameObject);
		MSG_BOX("pNewGameObject Add Failed. : CTowerDefence_Manager");
		return E_FAIL;
	}

	Safe_AddRef(pNewGameObject);
	m_DefenceObjects.push_back(pNewGameObject);
		
	

	return S_OK;
}


HRESULT CTowerDefence_Manager::Prepare_Portals(Vec4 vInitializePosition)
{
	CDefenceInvasion_Portal* pInvasionPortal = dynamic_cast<CDefenceInvasion_Portal*>(GI->Clone_GameObject(L"Prototype_GameObject_DefenceInvasion_Portal", LAYER_TYPE::LAYER_ETC));

	if (nullptr == pInvasionPortal)
	{
		MSG_BOX("pInvasionPortal is Null : CTowerDefence_Manager::Prepare_Defence");
		return E_FAIL;
	}

	CTransform* pPortalTransform = pInvasionPortal->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pPortalTransform)
	{
		MSG_BOX("pPortalTransform is Null : CTowerDefence_Manager::Prepare_Portals");
		return E_FAIL;
	}

	pPortalTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vInitializePosition, 1.f));
	pPortalTransform->LookAt_ForLandObject(XMVectorSet(-0.012f, 9.72f, 111.459f, 1.f));

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC, pInvasionPortal)))
	{
		MSG_BOX("Add_GameObject Failed : CTowerDefence_Manager::Prepare_Portals");
		return E_FAIL;
	}

	Safe_AddRef(pInvasionPortal);
	m_InvasionPortals.push_back(pInvasionPortal);

	return S_OK;
}

HRESULT CTowerDefence_Manager::Spawn_Defence_Monsters()
{
	

	return S_OK;
}

HRESULT CTowerDefence_Manager::Ready_Defence_Models()
{
	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Cannon_Tower_Barrel", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Cannon/", L"Cannon_Tower_Barrel")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Cannon_Tower_Base", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Cannon/", L"Cannon_Tower_Base")))
		return E_FAIL;



	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Crystal_Tower_Barrel", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Crystal/", L"Crystal_Tower_Barrel")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Crystal_Tower_Base", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Crystal/", L"Crystal_Tower_Base")))
		return E_FAIL;



	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Flame_Tower_Barrel", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Flame/", L"Flame_Tower_Barrel")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Flame_Tower_Base", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Flame/", L"Flame_Tower_Base")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Shadow_Tower_Barrel", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Shadow/", L"Shadow_Tower_Barrel")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Shadow_Tower_Base", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/Tower/Shadow/", L"Shadow_Tower_Base")))
		return E_FAIL;


	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_InvasionPortal", CModel::TYPE::TYPE_ANIM, L"../Bin/Export/AnimModel/TowerDefence/InvasionPortal/", L"InvasionPortal")))
		return E_FAIL;

	

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Cannon_Ball", CModel::TYPE::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/TowerDefence/Projectile/", L"Cannon_Ball")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Crystal_Ball", CModel::TYPE::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/TowerDefence/Projectile/", L"Crystal_Ball")))
		return E_FAIL;

	if (FAILED(GI->Import_Model_Data(LEVEL_STATIC, L"Prototype_Component_Model_Shadow_Ball", CModel::TYPE::TYPE_NONANIM, L"../Bin/Export/NonAnimModel/TowerDefence/Projectile/", L"Shadow_Ball")))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CTowerDefence_Manager::Ready_Prototype_Defence_Objects()
{
	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_DefenceTower_Cannon", CCannon_Tower::Create(m_pDevice, m_pContext, L"DefenceTower_Cannon"), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_DefenceTower_Crystal", CCrystal_Tower::Create(m_pDevice, m_pContext, L"DefenceTower_Crystal"), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_DefenceTower_Flame", CFlame_Tower::Create(m_pDevice, m_pContext, L"DefenceTower_Flame"), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_DefenceTower_Shadow", CShadow_Tower::Create(m_pDevice, m_pContext, L"DefenceTower_Shadow"), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_DefenceInvasion_Portal", CDefenceInvasion_Portal::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;




	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Cannon_Ball", CCannon_Ball::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Crystal_Ball", CCrystal_Ball::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	if (FAILED(GI->Add_Prototype(L"Prototype_GameObject_Shadow_Ball", CShadow_Ball::Create(m_pDevice, m_pContext), LAYER_TYPE::LAYER_ETC, true)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTowerDefence_Manager::Ready_Picking_Texture()
{
	D3D11_TEXTURE2D_DESC PickingTextureDesc;
	ZeroMemory(&PickingTextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	PickingTextureDesc.Width = 1;
	PickingTextureDesc.Height = 1;
	PickingTextureDesc.MipLevels = 1;
	PickingTextureDesc.ArraySize = 1;
	PickingTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	PickingTextureDesc.SampleDesc.Count = 1;
	PickingTextureDesc.SampleDesc.Quality = 0;
	PickingTextureDesc.Usage = D3D11_USAGE_STAGING;
	PickingTextureDesc.BindFlags = 0;
	PickingTextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	PickingTextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&PickingTextureDesc, nullptr, &m_pPickingTexture)))
		return E_FAIL;

	return S_OK;
}




void CTowerDefence_Manager::Free()
{
	__super::Free();
	if (true == m_bReserved)
	{
		Safe_Release(m_pDevice);
		Safe_Release(m_pContext);
		Safe_Release(m_pPicked_Object);

		for (_uint i = 0; i < m_DefenceObjects.size(); ++i)
			Safe_Release(m_DefenceObjects[i]);
		m_DefenceObjects.clear();

		for (_uint i = 0; i < m_DefenceMonsters.size(); ++i)
			Safe_Release(m_DefenceMonsters[i]);
		m_DefenceMonsters.clear();

		for (_uint i = 0; i < m_InvasionPortals.size(); ++i)
			Safe_Release(m_InvasionPortals[i]);
		m_InvasionPortals.clear();

		Safe_Release(m_pPickingTexture);
	}


}



