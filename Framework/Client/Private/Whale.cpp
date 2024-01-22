#include "stdafx.h"
#include "Whale.h"
#include "GameInstance.h"
#include "UI_World_Interaction.h"

#include "State_Animal_Swimming.h"
#include "State_BackFlip_01.h"
#include "State_BackFlip_02.h"

#include "Camera_Manager.h"
#include "Camera_CutScene_Map.h"

CWhale::CWhale(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CAnimals(pDevice, pContext, strObjectTag, eType)
{
}

CWhale::CWhale(const CWhale& rhs)
	: CAnimals(rhs)
{
}

HRESULT CWhale::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWhale::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	m_iObjectType = OBJ_TYPE::OBJ_ANIMAL;
	
	m_vCenter = m_pTransformCom->Get_Position();

	return S_OK;
}

void CWhale::Tick(_float fTimeDelta)
{
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

	if(true == m_bIsFlip)
		m_pStateMachineCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);
}

void CWhale::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CWhale::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CWhale::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	OBJECT_INIT_DESC Init_Data = {};

	if (pArg != nullptr)
		Init_Data.vStartPosition = static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition;


	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &Init_Data)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE::STATE_POSITION, Init_Data.vStartPosition);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Animal_Whale"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		TEXT("Com_StateMachine"), reinterpret_cast<CComponent**>(&m_pStateMachineCom))))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"),
		reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidDesc)))
		return E_FAIL;



	m_pModelCom->Set_Animation(0);

	return S_OK;
}

HRESULT CWhale::Ready_State()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"Armature|Armature|Animation_Whale|BaseLayer");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_SWIM, CState_Animal_Swimming::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"Armature|Armature|Animation_Whale|BaseLayer");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_BACKFILP01, CState_BackFlip_01::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"Armature|Armature|Animation_Whale|BaseLayer");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_BACKFILP02, CState_BackFlip_02::Create(m_pStateMachineCom, strAnimationNames));

	m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_SWIM);

	return S_OK;
}

HRESULT CWhale::Ready_Collider()
{
	CCollider_Sphere::SPHERE_COLLIDER_DESC SphereDesc;
	::ZeroMemory(&SphereDesc, sizeof(SphereDesc));

	BoundingSphere Sphere;
	ZeroMemory(&Sphere, sizeof(BoundingSphere));

	Sphere.Center = Vec3(0.f, 0.f, 0.f);
	Sphere.Radius = 1.f;

	SphereDesc.tSphere = Sphere;
	SphereDesc.pNode = nullptr;
	SphereDesc.pOwnerTransform = m_pTransformCom;
	SphereDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	SphereDesc.vOffsetPosition = Vec3::Zero;

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, CCollider::DETECTION_TYPE::BODY, &SphereDesc)))
		return E_FAIL;


	return S_OK;
}

void CWhale::Set_Flip(_bool Flip)
{
	m_bIsFlip = Flip;

	if (m_bIsFlip)
	{
		/* 카메라 컷신 시작*/
		CCamera_CutScene_Map* pCutSceneMap = dynamic_cast<CCamera_CutScene_Map*>(CCamera_Manager::GetInstance()->Get_Camera(CAMERA_TYPE::CUTSCENE_MAP));
		if (nullptr != pCutSceneMap)
		{
			const _uint iCurLevel = GI->Get_CurrentLevel();
			{
				if (LEVELID::LEVEL_ICELAND == iCurLevel || LEVELID::LEVEL_TOOL == iCurLevel)
				{
					pCutSceneMap->Start_CutScene("Winter_Whale", true);
				}
			}
		}

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, Get_RomingPoints()->front());
		m_pStateMachineCom->Change_State(CAnimals::STATE_SWIM);
	}
}

void CWhale::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CWhale::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CWhale::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

CWhale* CWhale::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CWhale* pInstance = new CWhale(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CWhale");
		Safe_Release<CWhale*>(pInstance);
	}

	return pInstance;
}

CGameObject* CWhale::Clone(void* pArg)
{
	CWhale* pInstance = new CWhale(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CWhale");
		Safe_Release<CWhale*>(pInstance);
	}

	return pInstance;
}

void CWhale::Free()
{
	__super::Free();
}
