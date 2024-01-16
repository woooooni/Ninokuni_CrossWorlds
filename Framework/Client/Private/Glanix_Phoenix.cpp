#include "stdafx.h"
#include "Glanix_Phoenix.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Glanix_PhoenixState_Spawn.h"
#include "Glanix_PhoenixState_Idle.h"
#include "Glanix_PhoenixState_Out.h"

#include "Glanix.h"
#include "Character_Manager.h"
#include "UI_Manager.h"

CGlanix_Phoenix::CGlanix_Phoenix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, LAYER_TYPE::LAYER_PROP)
{
}

CGlanix_Phoenix::CGlanix_Phoenix(const CGlanix_Phoenix& rhs)
	: CGameObject(rhs)
{

}

HRESULT CGlanix_Phoenix::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlanix_Phoenix::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	
	Vec4* pInitializePosition = (Vec4*)pArg;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(*pInitializePosition, 1.f));
	m_pTransformCom->RevolutionRotation(Vec3(pInitializePosition->x, pInitializePosition->y, pInitializePosition->z), XMVectorSet(0.f, 1.f, 0.f, 0.f), GI->RandomFloat(-90.f, 90.f));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CGlanix_Phoenix::Tick(_float fTimeDelta)
{
	m_pStateCom->Tick_State(fTimeDelta);

	__super::Tick(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::PROP, this);

	m_pControllerCom->Tick_Controller(fTimeDelta);

}

void CGlanix_Phoenix::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pControllerCom->LateTick_Controller(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	// m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
#endif // _DEBUG

}

HRESULT CGlanix_Phoenix::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	_float4 vRimColor = { 1.f, 0.f, 0.f, 1.f };
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CGlanix_Phoenix::Render_ShadowDepth()
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;


	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint iPassIndex = 0;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
			return E_FAIL;
	}

	return S_OK;
}



void CGlanix_Phoenix::Collision_Enter(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY && tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
	{
		if (nullptr != CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN))
		{
			CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN)->Set_Speed_Weight(CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::SWORD_MAN)->Get_Speed_Weight() + 0.1f);
			CUI_Manager::GetInstance()->On_FireVignette();
		}

		if (nullptr != CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER))
		{
			CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER)->Set_Speed_Weight(CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::DESTROYER)->Get_Speed_Weight() + 0.1f);
			CUI_Manager::GetInstance()->On_FireVignette();
		}

		if (nullptr != CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER))
		{
			CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER)->Set_Speed_Weight(CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER)->Get_Speed_Weight() + 0.1f);
			CUI_Manager::GetInstance()->On_FireVignette();
		}
		m_bIsOut = true;
	}
}

void CGlanix_Phoenix::Collision_Continue(const COLLISION_INFO& tInfo)
{

}

void CGlanix_Phoenix::Collision_Exit(const COLLISION_INFO& tInfo)
{
	int i = 0;
}

HRESULT CGlanix_Phoenix::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Phoenix"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_StateMachine */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"), TEXT("Com_StateMachine"), (CComponent**)&m_pStateCom)))
		return E_FAIL;

	m_pStateCom->Set_Owner(this);


	/* For.Com_PhysXBody */
	CPhysX_Controller::CONTROLLER_DESC ControllerDesc;

	ControllerDesc.eType = CPhysX_Controller::CAPSULE;
	ControllerDesc.pTransform = m_pTransformCom;
	ControllerDesc.vOffset = { 0.f, 1.125f, 0.f };
	ControllerDesc.fHeight = 1.f;
	ControllerDesc.fMaxJumpHeight = 10.f;
	ControllerDesc.fRaidus = 1.f;
	ControllerDesc.pOwner = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_PhysXController"), TEXT("Com_Controller"), (CComponent**)&m_pControllerCom, &ControllerDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_Phoenix::Ready_States()
{
	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Phoenix.ao|Phoenix_Spawn");
	m_pStateCom->Add_State(PHOENIX_SPAWN, CGlanix_PhoenixState_Spawn::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Phoenix.ao|Phoenix_Stand");
	m_pStateCom->Add_State(PHOENIX_IDLE, CGlanix_PhoenixState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Phoenix.ao|Phoenix_Success");
	m_pStateCom->Add_State(PHOENIX_OUT, CGlanix_PhoenixState_Out::Create(m_pStateCom, strAnimationName));
	
	m_pStateCom->Change_State(PHOENIX_SPAWN);

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CGlanix_Phoenix::Ready_Colliders()
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

	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider_Sphere::SPHERE, CCollider::DETECTION_TYPE::BOUNDARY, &SphereDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CGlanix_Phoenix* CGlanix_Phoenix::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGlanix_Phoenix* pInstance = new CGlanix_Phoenix(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGlanix_Phoenix");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGlanix_Phoenix::Clone(void* pArg)
{
	CGlanix_Phoenix* pInstance = new CGlanix_Phoenix(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGlanix_Phoenix");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_Phoenix::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pStateCom);
}

