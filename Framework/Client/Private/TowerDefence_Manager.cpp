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
}

void CTowerDefence_Manager::Start_Defence()
{
	m_eCurrentPhase = TOWER_DEFENCE_PHASE::DEFENCE_PROGRESS;
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

	if (KEY_TAP(KEY::LBTN))
		Create_Defence_Object();		

	if (KEY_TAP(KEY::RBTN))
	{
		Safe_Release(m_pPicked_Object);
		m_pPicked_Object = nullptr;
		m_pPicked_ObjectTransform = nullptr;
	}

	if (KEY_TAP(KEY::F5))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Cannon"));
	}

	if (KEY_TAP(KEY::F6))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Crystal"));
	}

	if (KEY_TAP(KEY::F7))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Flame"));
	}

	if (KEY_TAP(KEY::F8))
	{
		Set_PickObject(GI->Find_Prototype_GameObject(LAYER_TYPE::LAYER_ETC, L"Prototype_GameObject_DefenceTower_Shadow"));
	}

	if (KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::X))
	{
		Finish_Defence();
	}

}

void CTowerDefence_Manager::Tick_Defence_Progress(_float fTimeDelta)
{
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
	list<CGameObject*> PickingObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_GROUND);
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

	PickingObjects = GI->Find_GameObjects(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_GROUND);
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
	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_PROP, pNewGameObject)))
	{
		Safe_Release(pNewGameObject);
		MSG_BOX("pNewGameObject Add Failed. : CTowerDefence_Manager");
		return E_FAIL;
	}

	Safe_AddRef(pNewGameObject);
	m_DefenceObjects.push_back(pNewGameObject);
		
	

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
	}

}



