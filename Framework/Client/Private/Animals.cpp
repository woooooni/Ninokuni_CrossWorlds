#include "stdafx.h"
#include "..\Public\Animals.h"
#include "GameInstance.h"
#include "Game_Manager.h"
#include "Player.h"

CAnimals::CAnimals(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType)
	: CDynamicObject(pDevice, pContext, strObjectTag, eType)
{
}

CAnimals::CAnimals(const CAnimals& rhs)
	: CDynamicObject(rhs)
{

}

HRESULT CAnimals::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimals::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_eType = DYNAMIC_TYPE::DYNAMIC_ANIMAL;
	

	return S_OK;
}

void CAnimals::Tick(_float fTimeDelta)
{
	if (m_pRigidBodyCom != nullptr && m_pControllerCom != nullptr)
	{
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
		m_pControllerCom->Tick_Controller(fTimeDelta);
	}

	__super::Tick(fTimeDelta);
	GI->Add_CollisionGroup(COLLISION_GROUP::ANIMAL, this);
}

void CAnimals::LateTick(_float fTimeDelta)
{

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	if(nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	__super::LateTick(fTimeDelta);

	if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 30.0f))
	{
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc());
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc());

	}

}

HRESULT CAnimals::Render()
{


	return S_OK;
}

HRESULT CAnimals::Render_ShadowDepth()
{
	if (FAILED(__super::Render_ShadowDepth()))
		return E_FAIL;

	return S_OK;
}

HRESULT CAnimals::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
{
	if (FAILED(__super::Render()))
		return E_FAIL;

	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex = 0;

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CAnimals::Render_Instance_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
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

	if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
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

void CAnimals::Collision_Enter(const COLLISION_INFO& tInfo)
{

}

void CAnimals::Collision_Continue(const COLLISION_INFO& tInfo)
{

	if (OBJ_TYPE::OBJ_CHARACTER == tInfo.pOther->Get_ObjectType() && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		if (KEY_TAP(KEY::E))
		{
			if (true == m_bLift)
			{
				m_bLift = false;
				return;
			}

			Find_MinObject();
		}
	}
}
	
void CAnimals::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CAnimals::Find_MinObject()
{
	CPlayer* pPlayerObj = CGame_Manager::GetInstance()->Get_Player();
	if (nullptr == pPlayerObj)
		return;

	CTransform* pTransform = pPlayerObj->Get_Character()->Get_Component<CTransform>(TEXT("Com_Transform"));

	const _uint iCurrenLevel = GI->Get_CurrentLevel();
	list<CGameObject*>& GameObjList = GI->Find_GameObjects(iCurrenLevel, LAYER_TYPE::LAYER_DYNAMIC);

	_float fMaxDistance = FLT_MAX;

	CGameObject* pMinObj = nullptr;

	for (auto& pObj : GameObjList)
	{
		DYNAMIC_TYPE eType = static_cast<CDynamicObject*>(pObj)->Get_DynamicType();

		if (DYNAMIC_TYPE::DYNAMIC_ANIMAL == eType)
		{
			Vec4 vPlayerPos = pTransform->Get_Position();
			Vec4 vThisPos = pObj->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Position();

			Vec4 vDir = vPlayerPos - vThisPos;
			_float vDist = vDir.Length();

			if (vDist < fMaxDistance)
			{
				fMaxDistance = vDist;
				pMinObj = pObj;
			}
		}
	}

	static_cast<CAnimals*>(pMinObj)->m_bLift = true;
}

HRESULT CAnimals::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		TEXT("Com_AnimShader"), reinterpret_cast<CComponent**>(&m_pAnimShaderCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + m_strDynamicName,
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	CRigidBody::RIGID_BODY_DESC RigidDesc;
	RigidDesc.pTransform = m_pTransformCom;

	/* For.Com_RigidBody */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"), TEXT("Com_RigidBody"),
		reinterpret_cast<CComponent**>(&m_pRigidBodyCom), &RigidDesc)))
		return E_FAIL;

	return S_OK;
}
