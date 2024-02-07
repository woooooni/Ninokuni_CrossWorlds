#include "stdafx.h"
#include "GameInstance.h"
#include "Trigger.h"
#include "Utils.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "Whale.h"

#include "Camera_Manager.h"
#include "Camera_CutScene_Map.h"

#include "Game_Manager.h"
#include "Character.h"
#include "Player.h"
#include "WitchWood.h"
#include "Ruby.h"
#include "RubyCarriage.h"

#include "Quest_Manager.h"
#include "FileUtils.h"
#include "Utils.h"

CTrigger::CTrigger(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Trigger", OBJ_TYPE::OBJ_TRIGGER)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	: CGameObject(rhs)
{
}


HRESULT CTrigger::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTrigger::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	TRIGGER_DESC* pTriggerDesc = (TRIGGER_DESC*)pArg;

	m_eTriggerType = pTriggerDesc->eTriggerType;
	m_vExtents = pTriggerDesc->vExtents;
	m_strBGM = pTriggerDesc->strBGM;
	m_strMapName = pTriggerDesc->strMapName;
	m_vAt = pTriggerDesc->vAt;
	m_vEye = pTriggerDesc->vEye;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Collider()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, Vec4(pTriggerDesc->vStartPosition));

	return S_OK;
}

void CTrigger::Tick(_float fTimeDelta)
{
	GI->Add_CollisionGroup(COLLISION_GROUP::TRIGGER, this);
	__super::Tick(fTimeDelta);
}

void CTrigger::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])		
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif
}

HRESULT CTrigger::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}



void CTrigger::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
	if ((OBJ_TYPE::OBJ_CHARACTER == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY) || TEXT("Ruby_Carriage") == tInfo.pOther->Get_ObjectTag())
	{
		switch (m_eTriggerType)
		{
		case TRIGGER_TYPE::TRIGGER_MAP_NAME:
			GI->Set_ShadowLight(GI->Get_CurrentLevel(), m_vEye, m_vAt, Vec4(0.0f, 1.0f, 0.0f, 0.0f));

			if (m_strMapName == TEXT(""))
				return;
			CUI_Manager::GetInstance()->OnOff_MapName(true, m_strMapName);
			break;

		case TRIGGER_TYPE::TRIGGER_CHANGE_BGM:
			GI->Play_BGM(CUtils::ToWString(m_strBGM), 1.f, true);
			break;

		case TRIGGER_TYPE::TRIGGER_BOSS_GIANTY_ENTER:
			GI->Add_GameObject(GI->Get_CurrentLevel(), _uint(LAYER_MONSTER), TEXT("Prorotype_GameObject_Glanix"));
			Set_Dead(true);
			break;

		case TRIGGER_TYPE::TRIGGER_BOSS_STELLIA_ENTER:
			// TODO
			Set_Dead(true);
			break;
		case TRIGGER_TYPE::TRIGGER_WHALE_ENTER:
		{
			if (!m_bWhaleCutScene)
			{
				/* 컷신 카메라로 블렌딩 시작 */
				CCamera_CutScene_Map* pCutSceneCam = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
				if (nullptr != pCutSceneCam)
				{
					pCutSceneCam->Set_CutSceneTransform("Winter_Whale");
					CCamera_Manager::GetInstance()->Change_Camera(CAMERA_TYPE::CUTSCENE_MAP, 2.5f, LERP_MODE::SMOOTHER_STEP);

					/* 플레이어 인풋 막기 */
					CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Set_All_Input(false);

					/* 플레이어 스테이트 변경 */
					CGame_Manager::GetInstance()->Get_Player()->Get_Character()->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CCharacter::NEUTRAL_IDLE);
				}
				m_bWhaleCutScene = true;
			}
			break;
		}
		case TRIGGER_TYPE::TRIGGER_WITCH_WOOD_ENTER:
		{
			_uint iCurLevel = GI->Get_CurrentLevel();

			// TODO
			// 퀘스트 아이템이 없다면 Close, 있다면 Dissolve나 Open.
			list<CGameObject*>& pGameObjects = GI->Find_GameObjects(iCurLevel, LAYER_TYPE::LAYER_BUILDING);
			// Witch_Wood_Wall

			if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() != CQuest_Manager::GetInstance()->QUESTEVENT_FINALBATTLE)
			{
				for (auto& pObj : pGameObjects)
				{
					if (pObj->Get_ObjectTag() == TEXT("Witch_Wood_Wall"))
						static_cast<CWitchWood*>(pObj)->Set_Close(true);
				}
			}
			else
			{
				for (auto& pObj : pGameObjects)
				{
					if (pObj->Get_ObjectTag() == TEXT("Witch_Wood_Wall"))
						static_cast<CWitchWood*>(pObj)->Set_Close(false);
				}
			}
			break;
		}
		case TRIGGER_TYPE::TRIGGER_WITCH_ESCORT1:
			{
				if (TEXT("Ruby_Carriage") == tInfo.pOther->Get_ObjectTag())
				{
					CRuby* pRuby = static_cast<CRuby*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
					_bool bIsSection = pRuby->Get_QuestSection(CRuby::ESCORT_SECTION::SECTION1);

					if (false == bIsSection)
					{
						pRuby->Set_QuestSection(CRuby::ESCORT_SECTION::SECTION1, true);
						static_cast<CRubyCarriage*>(pRuby->Get_RidingObject())->Set_TakeTheCarriage(false);
						Load_Monster_Data(TEXT("Witch"), 1);
					}
					Set_Dead(true);
				}
			break;
			}
		case TRIGGER_TYPE::TRIGGER_WITCH_ESCORT2:
			{
				if (TEXT("Ruby_Carriage") == tInfo.pOther->Get_ObjectTag())
				{
					CRuby* pRuby = static_cast<CRuby*>(GI->Find_GameObject(LEVELID::LEVEL_WITCHFOREST, LAYER_TYPE::LAYER_NPC, TEXT("Ruby")));
					_bool bIsSection = pRuby->Get_QuestSection(CRuby::ESCORT_SECTION::SECTION2);

					if (false == bIsSection)
					{
						pRuby->Set_QuestSection(CRuby::ESCORT_SECTION::SECTION2, true);
						static_cast<CRubyCarriage*>(pRuby->Get_RidingObject())->Set_TakeTheCarriage(false);
						Load_Monster_Data(TEXT("Witch"), 2);
					}

					Set_Dead(true);
				}
				break;
			}
		}
	}
}
void CTrigger::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CTrigger::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

HRESULT CTrigger::Ready_Components()
{
	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;


	return S_OK;
}

HRESULT CTrigger::Ready_Collider()
{

	CCollider_AABB::AABB_COLLIDER_DESC AABBDesc;
	BoundingBox AABBBox;
	AABBBox.Extents = Vec3(1.0f, 1.0f, 1.0f);
	AABBBox.Center = Vec3(0.0f, 0.0f, 0.0f);
	AABBDesc.tBox = AABBBox;
	AABBDesc.pNode = nullptr;
	AABBDesc.pOwnerTransform = m_pTransformCom;
	AABBDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	AABBDesc.vOffsetPosition = { 0.f, 0.f, 0.f };

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::AABB, CCollider::DETECTION_TYPE::BODY, &AABBDesc)))
		return E_FAIL;

	return S_OK;
}

void CTrigger::Load_Monster_Data(const wstring& strFilePath, _uint Section)
{
	wstring strMapFilePath = L"../Bin/DataFiles/Map/" + strFilePath + L"/" + strFilePath + L"_Escort_" + to_wstring(Section) + L".map";

	shared_ptr<CFileUtils> File = make_shared<CFileUtils>();
	File->Open(strMapFilePath, FileMode::Read);

	for (_uint i = 0; i < LAYER_TYPE::LAYER_END; ++i)
	{
		if (LAYER_TYPE::LAYER_MONSTER != i)
			continue;

		_uint iObjectCount = File->Read<_uint>();

		for (_uint j = 0; j < iObjectCount; ++j)
		{
			// 3. Object_Prototype_Tag
			wstring strPrototypeTag = CUtils::ToWString(File->Read<string>());
			wstring strObjectTag = CUtils::ToWString(File->Read<string>());

			// 6. Obejct States
			_float4 vRight, vUp, vLook, vPos;

			File->Read<_float4>(vRight);
			File->Read<_float4>(vUp);
			File->Read<_float4>(vLook);
			File->Read<_float4>(vPos);


			OBJECT_INIT_DESC Init_Data = {};
			Init_Data.vStartPosition = vPos;
			CGameObject* pObj = nullptr;

			if (FAILED(GI->Add_GameObject(LEVELID::LEVEL_WITCHFOREST, i, strPrototypeTag, &Init_Data, &pObj, true)))
			{
				MSG_BOX("Load_Objects_Failed.");
				return;
			}

			if (nullptr == pObj)
			{
				MSG_BOX("Add_Object_Failed.");
				return;
			}
			pObj->Set_ObjectTag(strObjectTag);

			CTransform* pTransform = pObj->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Get_Transform_Failed.");
				return;
			}



			pTransform->Set_State(CTransform::STATE_RIGHT, XMLoadFloat4(&vRight));
			pTransform->Set_State(CTransform::STATE_UP, XMLoadFloat4(&vUp));
			pTransform->Set_State(CTransform::STATE_LOOK, XMLoadFloat4(&vLook));
			pTransform->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&vPos));
		}

	}
}


CTrigger* CTrigger::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTrigger* pInstance = new CTrigger(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTrigger::Clone(void* pArg)
{
	CTrigger* pInstance = new CTrigger(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CTrigger");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTrigger::Free()
{
	__super::Free();
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pRendererCom);
}
