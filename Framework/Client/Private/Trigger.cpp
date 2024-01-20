#include "stdafx.h"
#include "GameInstance.h"
#include "Trigger.h"
#include "Utils.h"
#include "Level_Loading.h"
#include "UI_Manager.h"
#include "Whale.h"

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

	if (KEY_TAP(KEY::INSERT))
	{
		_uint iCutLevel = GI->Get_CurrentLevel();
		CGameObject* pObject = GI->Find_GameObject(iCutLevel, LAYER_TYPE::LAYER_DYNAMIC, TEXT("Animal_Whale"));
		if (nullptr != pObject)
		{
			CWhale* pWhale = static_cast<CWhale*>(pObject);
	
			pWhale->Get_Component<CTransform>(L"Com_Transform")->Set_State(CTransform::STATE_POSITION, pWhale->Get_RomingPoints()->front());
			pWhale->Get_Component<CStateMachine>(L"Com_StateMachine")->Change_State(CAnimals::STATE_SWIM);
			pWhale->Set_Flip(true);
		}
	}
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
	if (OBJ_TYPE::OBJ_CHARACTER == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		switch (m_eTriggerType)
		{
		case TRIGGER_TYPE::TRIGGER_MAP_NAME:
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
		case TRIGGER_TYPE::TRIGER_WHALE_ENTER:
			{
			_uint iCutLevel = GI->Get_CurrentLevel();
			CGameObject* pObject = GI->Find_GameObject(iCutLevel, LAYER_TYPE::LAYER_DYNAMIC, TEXT("Animal_Whale"));
			if (nullptr == pObject)
				break;

			static_cast<CWhale*>(pObject)->Set_Flip(true);
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
	AABBBox.Extents = m_vExtents;
	AABBDesc.tBox = AABBBox;
	AABBDesc.pNode = nullptr;
	AABBDesc.pOwnerTransform = m_pTransformCom;
	AABBDesc.ModelPivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
	AABBDesc.vOffsetPosition = { 0.f, m_vExtents.y, 0.f };

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::AABB, CCollider::DETECTION_TYPE::BODY, &AABBDesc)))
		return E_FAIL;

	return S_OK;
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
