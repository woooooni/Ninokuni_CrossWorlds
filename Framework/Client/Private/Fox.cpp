#include "stdafx.h"
#include "Fox.h"
#include "GameInstance.h"
#include "UI_World_Interaction.h"

#include "State_Animal_Idle.h"
#include "State_Animal_Run.h"
#include "State_Animal_Walk.h"
#include "State_Animal_Lift.h"

#include "UI_World_AnimalTag.h"

CFox::CFox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CAnimals(pDevice, pContext, strObjectTag, eType)
{
}

CFox::CFox(const CFox& rhs)
	: CAnimals(rhs)
{
}

HRESULT CFox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_Collider()))
		return E_FAIL;

	m_vCenter = m_pTransformCom->Get_Position();

	CGameObject* pBtn = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_World_Interaction_Btn"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pBtn)
		return E_FAIL;
	m_pInteractionBtn = dynamic_cast<CUI_World_Interaction*>(pBtn);
	m_pInteractionBtn->Set_Owner(this);
	m_pInteractionBtn->Set_Active(true);

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_Animal_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_AnimalTag*>(pTag);
	m_pTag->Set_Owner(this, TEXT("설원여우"), 0.65f);

	return S_OK;
}

void CFox::Tick(_float fTimeDelta)
{
	m_pStateMachineCom->Tick_State(fTimeDelta);


	__super::Tick(fTimeDelta);

	if (nullptr != m_pInteractionBtn)
		m_pInteractionBtn->Tick(fTimeDelta);
	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CFox::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (nullptr != m_pInteractionBtn)
		m_pInteractionBtn->LateTick(fTimeDelta);
	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);
}

HRESULT CFox::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr != m_pInteractionBtn)
		m_pInteractionBtn->Render();

	return S_OK;
}


HRESULT CFox::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;


	if (nullptr != pArg)
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, static_cast<OBJECT_INIT_DESC*>(pArg)->vStartPosition);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Animal_Fox"),
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

HRESULT CFox::Ready_State()
{
	list<wstring> strAnimationNames;

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|NightFox_Idle02");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_IDLE, CState_Animal_Idle::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|FennecFox_Run");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_RUN, CState_Animal_Run::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|FennecFox_Walk");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_WALK, CState_Animal_Walk::Create(m_pStateMachineCom, strAnimationNames));

	strAnimationNames.clear();
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|FennecFox_Lifted");
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|FennecFox_LiftedLoop");
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|FennecFox_LiftedLoop2");
	strAnimationNames.push_back(L"SKM_FennecFox.ao|SKM_FennecFox.ao|SKM_FennecFox.ao|FennecFox_LiftedFinish");
	m_pStateMachineCom->Add_State(CAnimals::STATE::STATE_LIFT, CState_Animal_Lift::Create(m_pStateMachineCom, strAnimationNames));


	m_pStateMachineCom->Change_State(CAnimals::STATE::STATE_IDLE);

	return S_OK;
}

HRESULT CFox::Ready_Collider()
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

void CFox::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CFox::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CFox::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

CFox* CFox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eObjType)
{
	CFox* pInstance = new CFox(pDevice, pContext, strObjectTag, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CFox");
		Safe_Release<CFox*>(pInstance);
	}

	return pInstance;
}

CGameObject* CFox::Clone(void* pArg)
{
	CFox* pInstance = new CFox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CFox");
		Safe_Release<CFox*>(pInstance);
	}

	return pInstance;
}

void CFox::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
	Safe_Release(m_pInteractionBtn);
}
