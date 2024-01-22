#include "stdafx.h"
#include "Engineer_Npc.h"

#include "GameInstance.h"

#include "NpcState_Idle.h"
#include "UniqueNpcState_Walk.h"
#include "UniqueNpcState_Run.h"
#include "UniqueNpcState_Talk.h"

#include "UI_World_NPCTag.h"

#include "Character_Manager.h"

CEngineer_Npc::CEngineer_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CGameNpc(pDevice, pContext, strObjectTag)
{
}

CEngineer_Npc::CEngineer_Npc(const CEngineer_Npc& rhs)
	: CGameNpc(rhs)
{
}

HRESULT CEngineer_Npc::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEngineer_Npc::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_strObjectTag = TEXT("Engineer_Dummy");

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	m_pTransformCom->FixRotation(0.f, -40.f, 0.f);

	m_pPartModels[PART_TYPE::HEAD] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::HEAD, L"Adventure");
	m_pPartModels[PART_TYPE::HAIR] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::HAIR, 0);
	m_pPartModels[PART_TYPE::FACE] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::FACE, 0);
	m_pPartModels[PART_TYPE::BODY] = CCharacter_Manager::GetInstance()->Get_PartModel(CHARACTER_TYPE::ENGINEER, PART_TYPE::BODY, L"Adventure");

	m_pModelCom->Set_Animation(TEXT("SKM_Engineer_SoulDiver.ao|Engineer_NeutralStand"));

	if (FAILED(Ready_States()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	// UI NameTag
	CGameObject* pTag = GI->Clone_GameObject(TEXT("Prototype_GameObject_UI_NPC_Tag"), LAYER_TYPE::LAYER_UI);
	if (nullptr == pTag)
		return E_FAIL;

	m_pTag = dynamic_cast<CUI_World_NPCTag*>(pTag);
	m_pTag->Set_Owner(this, m_strKorName, 1.8f, true);

	return S_OK;
}

void CEngineer_Npc::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (nullptr != m_pTag)
		m_pTag->Tick(fTimeDelta);
}

void CEngineer_Npc::LateTick(_float fTimeDelta)
{
	// __super::LateTick(fTimeDelta);

	if (nullptr == m_pRendererCom)
		return;

	if (nullptr != m_pControllerCom)
		m_pControllerCom->LateTick_Controller(fTimeDelta);

	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	Compute_CamZ(m_pTransformCom->Get_Position());

	if (m_fCamDistance <= 100.f && true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 4.f))
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	}

#ifdef _DEBUG
	//m_pRendererCom->Set_PlayerPosition(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	//for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	//{
	//	for (auto& pCollider : m_Colliders[i])
	//		m_pRendererCom->Add_Debug(pCollider);
	//}
	//m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG


	if (nullptr != m_pTag)
		m_pTag->LateTick(fTimeDelta);

#ifdef DEBUG
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG
}

HRESULT CEngineer_Npc::Render()
{
	if (FAILED(__super::Render()))
		return E_FAIL;

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

	_float4 vRimColor = { 0.f, 0.f, 0.f, 0.f };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;


	for (size_t i = 0; i < PART_TYPE::PART_END; i++)
	{
		if (nullptr == m_pPartModels[i])
			continue;

		const _uint		iNumMeshes = m_pPartModels[i]->Get_NumMeshes();

		for (_uint j = 0; j < iNumMeshes; ++j)
		{
			if (FAILED(m_pPartModels[i]->SetUp_OnShader(m_pShaderCom, m_pPartModels[i]->Get_MaterialIndex(j), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
				return E_FAIL;

			if (FAILED(m_pPartModels[i]->Render(m_pShaderCom, j)))
				return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CEngineer_Npc::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	return S_OK;
}

HRESULT CEngineer_Npc::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	return S_OK;
}

void CEngineer_Npc::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CEngineer_Npc::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CEngineer_Npc::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

void CEngineer_Npc::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CEngineer_Npc::Ready_States()
{
	return S_OK;
}

HRESULT CEngineer_Npc::Ready_Colliders()
{
	m_strKorName = TEXT("ÀÇ¹®ÀÇ ¼Ò³à");
	m_tStat.fSpeed = 0.5f;

	m_pStateCom->Set_Owner(this);

	list<wstring> strAnimationName;

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_NeutralStand");
	m_pStateCom->Add_State(NPC_IDLE, CNpcState_Idle::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_NeutralIdle01");
	m_pStateCom->Add_State(NPC_UNIQUENPC_TALK, CUniqueNpcState_Talk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_NeutralWalk");
	m_pStateCom->Add_State(NPC_UNIQUENPC_WALK, CUniqueNpcState_Walk::Create(m_pStateCom, strAnimationName));

	strAnimationName.clear();
	strAnimationName.push_back(L"SKM_Engineer_SoulDiver.ao|Engineer_NeutralRun");
	m_pStateCom->Add_State(NPC_UNIQUENPC_RUN, CUniqueNpcState_Run::Create(m_pStateCom, strAnimationName));

	m_pStateCom->Change_State(NPC_IDLE);

	return S_OK;
}

HRESULT CEngineer_Npc::Ready_Sockets()
{
	if (FAILED(__super::Ready_Colliders()))
		return E_FAIL;

	return S_OK;
}

CEngineer_Npc* CEngineer_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
{
	CEngineer_Npc* pInstance = new CEngineer_Npc(pDevice, pContext, strObjectTag);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Fail Create : CEngineer_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEngineer_Npc::Clone(void* pArg)
{
	CEngineer_Npc* pInstance = new CEngineer_Npc(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CEngineer_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEngineer_Npc::Free()
{
	__super::Free();

	Safe_Release(m_pTag);
}