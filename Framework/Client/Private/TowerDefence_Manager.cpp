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
	if (nullptr != m_pPicked_Object)
	{
		m_pPicked_Object = nullptr;
		Safe_Release(m_pPicked_Object);
		m_pPicked_ObjectTransform = nullptr;
	}

	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_PREPARE;
	CCamera_Manager::GetInstance()->Set_CurCamera(CAMERA_TYPE::FREE);

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

	

	CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);
}

void CTowerDefence_Manager::Start_Defence()
{
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

void CTowerDefence_Manager::Set_PickObject(CGameObject* pPickObject)
{
	if (nullptr == pPickObject)	
		return;

	if (nullptr != m_pPicked_Object)
	{
		Safe_Release(m_pPicked_Object);
		m_pPicked_Object = nullptr;
	}

	m_pPicked_Object = pPickObject->Clone(nullptr);

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
	if (nullptr != m_pPicked_Object)
		m_pPicked_Object->Tick(fTimeDelta);

	Picking_Position();

	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::LBTN))
	{
		Picking_Object();
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

	if (KEY_TAP(KEY::F5))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Cannon"));
		return;
	}

	if (KEY_TAP(KEY::F6))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Crystal"));
		return;
	}

	if (KEY_TAP(KEY::F7))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Flame"));
		return;
	}

	if (KEY_TAP(KEY::F8))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Shadow"));
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
 
	list<CGameObject*>& PickingObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_GROUND);
	for (auto& pObject : PickingObjects)
	{
		Vec4 vPickingPos = {};
		CTransform* pTransform = pObject->Get_Component<CTransform>(L"Com_Transform");
		CModel* pModel = pObject->Get_Component<CModel>(L"Com_Model");

		for (auto& pMesh : pModel->Get_Meshes())
		{
			if (true == CPicking_Manager::GetInstance()->Is_Picking(pTransform, pMesh, true, &vPickingPos))
			{
				if (nullptr != m_pPicked_ObjectTransform)
				{
					m_pPicked_ObjectTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vPickingPos, 1.f));
					return;
				}
			}
		}

	}
}

void CTowerDefence_Manager::Picking_Object()
{
	list<CGameObject*>& PickingObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_ETC);

	for (auto& pObject : PickingObjects)
	{
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

					
					m_pPicked_ObjectTransform = m_pPicked_Object->Get_Component<CTransform>(L"Com_Transform");
					if (nullptr == m_pPicked_ObjectTransform)
						return;

					m_pPicked_Object = pObject;
					return;
				}
			}
		}
	}
}



HRESULT CTowerDefence_Manager::Create_Defence_Object()
{
	if (nullptr == m_pPicked_Object)
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
		
	}

}



