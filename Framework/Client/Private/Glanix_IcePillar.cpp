#include "stdafx.h"
#include "Glanix_IcePillar.h"
#include "GameInstance.h"
#include "HierarchyNode.h"
#include "Trail.h"

#include "Glanix.h"

#include "Camera_Manager.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Utils.h"

CGlanix_IcePillar::CGlanix_IcePillar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameObject(pDevice, pContext, strObjectTag, LAYER_TYPE::LAYER_PROP)
{
}

CGlanix_IcePillar::CGlanix_IcePillar(const CGlanix_IcePillar& rhs)
	: CGameObject(rhs)
{

}

HRESULT CGlanix_IcePillar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CGlanix_IcePillar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	/* For.Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, *(_vector*)pArg);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

void CGlanix_IcePillar::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::PROP, this);

	if (m_tSpeedDesc.bActive)
		m_tSpeedDesc.Update(fTimeDelta);

	m_pTransformCom->RevolutionRotation(m_vRotateOriginPos.xyz(), Vec3::Up, m_tSpeedDesc.fCurValue * fTimeDelta);

	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	m_pControllerCom->Tick_Controller(fTimeDelta);
}

void CGlanix_IcePillar::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	m_pControllerCom->LateTick_Controller(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	// m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);

	m_pRendererCom->Add_RenderGroup_Instancing(CRenderer::RENDER_NONBLEND, CRenderer::INSTANCING_SHADER_TYPE::MODEL, this, m_pTransformCom->Get_WorldFloat4x4());
}

HRESULT CGlanix_IcePillar::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_IcePillar::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CGlanix_IcePillar::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	__super::Render();

	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint iPassIndex = 0;
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		//if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		//	iPassIndex = 0;
		//else
		//	iPassIndex++;
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CGlanix_IcePillar::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	if (nullptr == m_pModelCom || nullptr == pInstancingShader)
		return E_FAIL;
	_float4 vCamPosition = GI->Get_CamPosition();
	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &vCamPosition, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;
	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;
	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;
		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/
		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
			return E_FAIL;
	}

	return S_OK;
}

void CGlanix_IcePillar::Collision_Enter(const COLLISION_INFO& tInfo)
{
	int i = 0;
}

void CGlanix_IcePillar::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_BOSS && 
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		if (dynamic_cast<CGlanix*>(tInfo.pOther)->Get_Component<CStateMachine>(TEXT("Com_StateMachine"))->Get_CurrState() == CGlanix::GLANIX_RAGECHARGE)
		{
			dynamic_cast<CGlanix*>(tInfo.pOther)->Set_IsCrash(true);

			// Effect Create
			{
				_float fRandomXOffset = CUtils::Random_Float(-1.f, 1.f);
				_float fRandomYOffset = CUtils::Random_Float(-1.f, 1.f);

				CTransform* pOtherTransform = tInfo.pOther->Get_Component<CTransform>(L"Com_Transform");
				if (nullptr == pOtherTransform)
					return;

				Vec4 vMonsterPos = pOtherTransform->Get_Position();
				Vec4 vThisPos = m_pTransformCom->Get_Position();
				Vec4 vLook = XMVector4Normalize(vThisPos - vMonsterPos);

				_vector vLookPosition = pOtherTransform->Get_Position();
				vLookPosition += vLook * 0.5f;

				_matrix ThisWorldMat = pOtherTransform->Get_WorldMatrix();
				ThisWorldMat.r[CTransform::STATE_POSITION] = vLookPosition;

				GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Hit_Monster"), ThisWorldMat,
					_float3(fRandomXOffset, 3.f + fRandomYOffset, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f));

				GET_INSTANCE(CEffect_Manager)->Generate_Vfx(TEXT("Vfx_Glanix_Skill_IcePillar"), m_pTransformCom->Get_WorldMatrix(), this);
			}

			if (nullptr != m_pGlanix)
				m_pGlanix->Delete_Pillar(m_iKey);
			//Set_Dead(this);
		}
	}
}

void CGlanix_IcePillar::Collision_Exit(const COLLISION_INFO& tInfo)
{
	int i = 0;
}

void CGlanix_IcePillar::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	if (!m_bShake)
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake(0.15f, 17.f, 0.4f);
		m_bShake = true;
	}

	__super::Ground_Collision_Enter(tInfo);
	if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
	{
		m_pRigidBodyCom->Set_Ground(true);
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}
}

void CGlanix_IcePillar::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CGlanix_IcePillar::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	m_pRigidBodyCom->Set_Ground(false);
	m_pRigidBodyCom->Set_Use_Gravity(true);
}

HRESULT CGlanix_IcePillar::Ready_Components()
{
	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_GlanixPillar"), TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

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

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For. Com_RigidBody*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"), (CComponent**)&m_pRigidBodyCom, &RigidDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma region Ready_Colliders
HRESULT CGlanix_IcePillar::Ready_Colliders()
{
	CCollider_OBB::OBB_COLLIDER_DESC OBBDesc;
	ZeroMemory(&OBBDesc, sizeof OBBDesc);

	BoundingOrientedBox OBBBox;
	ZeroMemory(&OBBBox, sizeof(BoundingOrientedBox));

	XMStoreFloat4(&OBBBox.Orientation, XMQuaternionRotationRollPitchYaw(XMConvertToRadians(0.f), XMConvertToRadians(0.f), XMConvertToRadians(0.f)));
	OBBBox.Extents = { 200.f, 200.f, 250.f };

	OBBDesc.tBox = OBBBox;
	OBBDesc.pNode = nullptr;
	OBBDesc.pOwnerTransform = m_pTransformCom;
	OBBDesc.ModelPivotMatrix = m_pModelCom->Get_PivotMatrix();
	OBBDesc.vOffsetPosition = Vec3(0.f, 200.f, 0.f);

	/* Body */
	if (FAILED(__super::Add_Collider(LEVEL_STATIC, CCollider::COLLIDER_TYPE::OBB, CCollider::DETECTION_TYPE::BODY, &OBBDesc)))
		return E_FAIL;

	return S_OK;
}

#pragma endregion

CGlanix_IcePillar* CGlanix_IcePillar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CGlanix_IcePillar* pInstance = new CGlanix_IcePillar(pDevice, pContext, strObjectTag);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed : CGlanix_IcePillar");
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

CGameObject* CGlanix_IcePillar::Clone(void* pArg)
{
	CGlanix_IcePillar* pInstance = new CGlanix_IcePillar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CGlanix_IcePillar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGlanix_IcePillar::Free()
{
	__super::Free();

	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pControllerCom);
}

