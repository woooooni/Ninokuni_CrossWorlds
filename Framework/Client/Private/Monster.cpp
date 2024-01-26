#include "stdafx.h"
#include "GameInstance.h"
#include "Monster.h"
#include "HierarchyNode.h"
#include "Part.h"
#include "Effect_Manager.h"
#include "Utils.h"
#include "Camera.h"
#include "Particle_Manager.h"
#include "Camera_Manager.h"
#include "UI_Manager.h"
#include "UIDamage_Manager.h"
#include "BehaviorTree.h"
#include "Particle.h"

#include "Game_Manager.h"
#include "Character.h"

#include "Player.h"

#include "Quest_Manager.h"
#include "Character_Projectile.h"
USING(Client)

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CGameObject(pDevice, pContext, strObjectTag, OBJ_TYPE::OBJ_MONSTER)
{
	m_tStat = tStat;
	m_tStat.fMaxHp = m_tStat.fHp;
	m_tStat.fMaxMp = m_tStat.fMp;
}

CMonster::CMonster(const CMonster& rhs)
	: CGameObject(rhs)
	, m_tStat(rhs.m_tStat)
	, m_fDissolveWeight(1.f)

{

}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	// Dissolve Texture
	m_pDissoveTexture = static_cast<CTexture*>(GI->Clone_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise")));
	if (m_pDissoveTexture == nullptr)
		return E_FAIL;

	m_fStunTime = 3.f;
	m_fNearDist = 9999.f;
	m_fTargetSearchDist = 30.f;

	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* 타겟 설정 */
	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::GetInstance()->QUESTEVENT_INVASION)
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT] = true;

		if (m_tTargetDesc.pTarget == nullptr)
		{
			m_tTargetDesc.pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), (_uint)LAYER_NPC, TEXT("TreeGrandfa"));
		
			if (m_tTargetDesc.pTarget != nullptr)
				m_tTargetDesc.pTragetTransform = m_tTargetDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
		}

		m_iRemainsTowerCount = 0;

		/* 모든 타워 및 가을 할아범 불러오기 */
		for (auto iter : GI->Find_GameObjects(GI->Get_CurrentLevel(), (_uint)LAYER_ETC))
		{
			// 가을 할아범
			CGameObject* pTree = GI->Find_GameObject(GI->Get_CurrentLevel(), (_uint)LAYER_NPC, TEXT("TreeGrandfa"));
			if (pTree != nullptr)
			{
				Vec4 vToTree = pTree->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Position() - m_pTransformCom->Get_Position();
				m_fDistToTree = vToTree.Length();
			}

			// 타워
			if (iter->Get_ObjectType() == OBJ_DEFENCE_TOWER)
			{
				m_iRemainsTowerCount += 1;
				Vec4 vToTower = iter->Get_Component<CTransform>(TEXT("Com_Transform"))->Get_Position() - m_pTransformCom->Get_Position();
				_float fToTowerDist = vToTower.Length();

				// 가을 할아범 광장에 들어왔을 때
				if (fabs(m_fDistToTree) < m_fTargetSearchDist)
				{
					// 가까운 타워 찾아서 공격.
					if (fabs(fToTowerDist) < m_fNearDist)
					{
						// 가을 할아범이 더 가깝다면 가을 할아범한테(경로상에 타워가 없다면)
						if (fabs(m_fDistToTree) < fabs(fToTowerDist))
						{
							m_tTargetDesc.pTarget = pTree;
							if (m_tTargetDesc.pTarget != nullptr)
								m_tTargetDesc.pTragetTransform = iter->Get_Component<CTransform>(TEXT("Com_Transform"));
						}
						else
						{
							m_fNearDist = fabs(fToTowerDist);
							m_tTargetDesc.pTarget = iter;
							if (m_tTargetDesc.pTarget != nullptr)
								m_tTargetDesc.pTragetTransform = iter->Get_Component<CTransform>(TEXT("Com_Transform"));
						}
					}
				}

			}
		}

		// 남아있는 타워가 없다면.
		if (m_iRemainsTowerCount == 0)
		{
			m_tTargetDesc.pTarget = GI->Find_GameObject(GI->Get_CurrentLevel(), (_uint)LAYER_NPC, TEXT("Com_Transform"));

			if (m_tTargetDesc.pTarget != nullptr)
				m_tTargetDesc.pTragetTransform = m_tTargetDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
		}
	}
	else
	{
		m_tTargetDesc.pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
		m_tTargetDesc.pTragetTransform = m_tTargetDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
	}


	if (m_bInfinite)
	{
		m_fAccInfinite += fTimeDelta;
		if (m_fAccInfinite >= m_fInfiniteTime)
		{
			m_bInfinite = false;
			m_fAccInfinite = 0.f;

			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, true);
		}
	}

	if (m_pBTCom != nullptr)
		m_pBTCom->Tick(fTimeDelta);

	if (m_pRigidBodyCom != nullptr)
		m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	if (m_pControllerCom != nullptr)
		m_pControllerCom->Tick_Controller(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);

	if (m_bIsRimUse) // RimLight
	{
		m_vRimLightColor = { 1.f, 0.f, 0.f, 1.f };
		Tick_RimLight(fTimeDelta);
	}
	else
	{
		m_vRimLightColor = { 0.f, 0.f, 0.f, 0.f };
	}

		

	if (m_bReserveDead) // Dissolve
	{
		if (!m_bDissolveEffect)
		{
			m_bDissolveEffect = true;
			GET_INSTANCE(CParticle_Manager)->Generate_Particle(TEXT("Particle_Monster_Dissolve"), m_pTransformCom->Get_WorldMatrix(), _float3(0.f, 0.f, 0.f), _float3(1.f, 1.f, 1.f), _float3(0.f, 0.f, 0.f), nullptr, &m_pDissolveObject);
		}
		else if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
			if (nullptr != m_pDissolveObject)
			{
				if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() == CQuest_Manager::QUESTEVENT_MONSTER_KILL)
					CQuest_Manager::GetInstance()->Set_MonsterKillCount(1);

				m_pDissolveObject->Set_Dead(true);
				m_pDissolveObject = nullptr;
			}
			return;
		}
		else
			m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
	}
}

void CMonster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	
	if(m_pControllerCom != nullptr)
		m_pControllerCom->LateTick_Controller(fTimeDelta);
	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);*/
	// m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc());
	
	m_AnimInstanceDesc.fDissolveDuration = m_fDissolveDuration;
	m_AnimInstanceDesc.fDissolveSpeed = m_fDissolveDuration;
	m_AnimInstanceDesc.fDissolveTotal = m_fDissolveTotal;
	m_AnimInstanceDesc.fDissolveWeight = m_fDissolveWeight;
	m_AnimInstanceDesc.vDissolveColor = m_vDissolveColor;
	m_AnimInstanceDesc.vRimColor = m_vRimLightColor;

	Compute_CamZ(m_pTransformCom->Get_Position());
	if (m_fCamDistance <= 50.f && true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 10.f))
	{
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
	}



#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	m_pRendererCom->Add_Debug(m_pControllerCom);
#endif // DEBUG


	if (m_pBTCom != nullptr)
		m_pBTCom->LateTick(fTimeDelta);

	
}

HRESULT CMonster::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	__super::Render();

	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if(FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(pInstancingShader)))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_vCamPosition", &GI->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_AnimInstancingDesc", AnimModelDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * AnimModelDesc.size())))
		return E_FAIL;

	// Bloom -----------------
	if (FAILED(pInstancingShader->Bind_RawValue("g_vBloomPower", &m_vBloomPower, sizeof(_float3))))
		return E_FAIL;
	// ----------------- Bloom

// RimLight -----------------

// ----------------- RimLight

// Dissolve -----------------
if (FAILED(m_pDissoveTexture->Bind_ShaderResource(pInstancingShader, "g_DissolveTexture", 51)))
return E_FAIL;
// ----------------- Dissolve

_uint iNumMeshes = m_pModelCom->Get_NumMeshes();
_uint iPassIndex = 0;

for (_uint i = 0; i < iNumMeshes; ++i)
{
	if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
		return E_FAIL;

	if (m_bReserveDead)
		iPassIndex = 2;
	else if (FAILED(m_pModelCom->SetUp_OnShader(pInstancingShader, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
		iPassIndex = 0;
	else
		iPassIndex++;

	if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, iPassIndex)))
		return E_FAIL;
}


return S_OK;
}

HRESULT CMonster::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_AnimInstancingDesc", AnimModelDesc.data(), sizeof(ANIMODEL_INSTANCE_DESC) * AnimModelDesc.size())))
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

		if (FAILED(m_pModelCom->Render_Instancing(pInstancingShader, i, pInstancingBuffer, WorldMatrices, 10)))
			return E_FAIL;
	}


	return S_OK;
}

void CMonster::Collision_Enter(const COLLISION_INFO& tInfo)
{
	// 퀘스트 단계가 아닐 때 (플레이어 공격)
	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() != CQuest_Manager::GetInstance()->QUESTEVENT_INVASION)
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_DEFENCE_TOWER)
		{
			if (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY
				&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
			{
				if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
				{
					// 둘 다 켜줘야 함.
					// ATKAROUND는 공격 동작이 끝나고 추적할 것인지 결정하기 위함.
					// 범위내에 없으면 ATK을 false 시켜주기 위해.
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = true;
				}
			}
		}
	}

	// 퀘스트 단계일 때 (플레이어 공격 x)
	else
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_NPC && tInfo.pOther->Get_PrototypeTag() == TEXT("Prorotype_GameObject_TreeGrandfa"))
		{
			if (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY
				&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
			{
				if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
				{
					// 둘 다 켜줘야 함.
					// ATKAROUND는 공격 동작이 끝나고 추적할 것인지 결정하기 위함.
					// 범위내에 없으면 ATK을 false 시켜주기 위해.
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = true;
				}
			}
		}

		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_DEFENCE_TOWER && fabs(m_fDistToTree) < m_fTargetSearchDist)
		{
			if (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY
				&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
			{
				if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
				{
					// 둘 다 켜줘야 함.
					// ATKAROUND는 공격 동작이 끝나고 추적할 것인지 결정하기 위함.
					// 범위내에 없으면 ATK을 false 시켜주기 위해.
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = true;
				}
			}
		}
	}
}

void CMonster::Collision_Continue(const COLLISION_INFO& tInfo)
{
	// 퀘스트 단계가 아닐 때 (플레이어 공격)
	if (CQuest_Manager::GetInstance()->Get_CurQuestEvent() != CQuest_Manager::GetInstance()->QUESTEVENT_INVASION)
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
		{
			if (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY
				&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
			{
				if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
				}
			}
		}
	}

	// 퀘스트 단계일 때 (플레이어 공격 x)
	else
	{
		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_NPC && tInfo.pOther->Get_PrototypeTag() == TEXT("Prorotype_GameObject_TreeGrandfa"))
		{
			if (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY
				&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
			{
				if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
				}
			}
		}

		if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_DEFENCE_TOWER && fabs(m_fDistToTree) < m_fTargetSearchDist)
		{
			if (tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY
				&& tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
			{
				if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
				{
					m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
				}
			}
		}
	}
}

void CMonster::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_DEFENCE_TOWER)
	{
		if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY &&
			tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
		{
			m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = false;
		}
	}
}

void CMonster::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
	{
		m_pRigidBodyCom->Set_Ground(true);
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}
}

void CMonster::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
	{
		m_pRigidBodyCom->Set_Ground(true);
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}
}

void CMonster::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
	m_pRigidBodyCom->Set_Ground(false);
	m_pRigidBodyCom->Set_Use_Gravity(true);
}


void CMonster::On_Damaged(const COLLISION_INFO& tInfo)
{
	m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ISHIT] = true;

	_bool bIsBoss = false;

	if (TEXT("Glanix") == Get_ObjectTag() || TEXT("Stellia") == Get_ObjectTag() || TEXT("DreamerMazeWitch") == Get_ObjectTag())
		bIsBoss = true;

	CCharacter* pCharacter = nullptr;
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER_PROJECTILE)
	{
		CCharacter_Projectile* pProjectile = dynamic_cast<CCharacter_Projectile*>(tInfo.pOther);
		if (nullptr == pProjectile)
		{
			MSG_BOX("CCharacter_Projectile Cast Failed.");
			return;
		}
		pCharacter = pProjectile->Get_Owner();
	}
	else if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_DEFENCE_TOWER)
	{
		pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
	}
	else
	{
		pCharacter = dynamic_cast<CCharacter*>(tInfo.pOther);
	}
	if (nullptr == pCharacter)
		return;

	_float fElementalWeight = CGame_Manager::GetInstance()->Calculate_Elemental(tInfo.pOtherCollider->Get_ElementalType(), m_tStat.eElementType);
	_int iDamage = (pCharacter->Get_Stat().iAtt * CUtils::Random_Float(0.5f, 1.5f)) - (m_tStat.iDef * 0.2f) * fElementalWeight;


	if (m_eDamagedElemental == ELEMENTAL_TYPE::BASIC)
		m_eDamagedElemental = tInfo.pOtherCollider->Get_ElementalType();
	else
		m_eDamagedElemental = ELEMENTAL_TYPE::BASIC;

	if (fElementalWeight > 1.f)
	{
		CUIDamage_Manager::GetInstance()->Create_MonsterDamageNumber(m_pTransformCom, bIsBoss, CUIDamage_Manager::UI_DAMAGETYPE::STRENGTH, iDamage);
	}
	else if (fElementalWeight < 1.f)
	{
		CUIDamage_Manager::GetInstance()->Create_MonsterDamageNumber(m_pTransformCom, bIsBoss, CUIDamage_Manager::UI_DAMAGETYPE::WEAKNESS, iDamage);
	}
	else
	{
		CUIDamage_Manager::GetInstance()->Create_MonsterDamageNumber(m_pTransformCom, bIsBoss, CUIDamage_Manager::UI_DAMAGETYPE::NONE, iDamage);
	}

	
	m_tStat.fHp = max(0, m_tStat.fHp - iDamage);

	Start_RimLight();
	Start_MonsterHittedEvent(tInfo.pOther);
}

HRESULT CMonster::Ready_RoamingPoint()
{
	_float4 vOriginPos = {};
	XMStoreFloat4(&vOriginPos, m_vOriginPos);

	_float fX = GI->RandomFloat(-6.f, 6.f);
	_float fZ = GI->RandomFloat(-6.f, 6.f);

	m_vecRoamingArea.push_back({ vOriginPos.x + fX, vOriginPos.y, vOriginPos.z + fZ, 1.f });

	fX = GI->RandomFloat(-6.f, 6.f);
	fZ = GI->RandomFloat(-6.f, 6.f);

	m_vecRoamingArea.push_back({ vOriginPos.x + fX, vOriginPos.y, vOriginPos.z + fZ, 1.f });

	return S_OK;
}

void CMonster::Start_MonsterHittedEvent(CGameObject* pPlayer)
{
	/* 몬스터가 플레이어에게 피격 당했을시 타격감을 발생시키기 위한 요소들 */

	if (nullptr == pPlayer)
		return;

	/* Camera */
	{
		CCamera_Manager::GetInstance()->Start_Action_Shake_Default_Attack();
	}

	/* Animation */
	{
		CModel* pModelCom = pPlayer->Get_Component<CModel>(L"Com_Model");
		if (nullptr != pModelCom)
		{

		}
	}

	/* Sound */
	{
		/* 몬스터의 피격 사운드는 플레이어의 공격 종류에 따라 달라진다. -> 플레이어 쪽에서 결정 */
		/* 일단 디폴트로 하나 박아둔다. */
	}
}


void CMonster::LookAt_DamagedObject(CGameObject* pAttacker)
{
	CTransform* pOtherTransform = pAttacker->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pOtherTransform)
		return;

	m_pTransformCom->LookAt_ForLandObject(pOtherTransform->Get_State(CTransform::STATE_POSITION));
}

void CMonster::Play_DamagedSound()
{
	TCHAR strSoundFileName[MAX_PATH] = L"";

	//switch (m_eMonsterType)
	//{
	//case Client::CMonster::NORMAL_0:
	//	lstrcatW(strSoundFileName, L"Voice_Normal_Monster_0_Damanged_Basic_");
	//	lstrcatW(strSoundFileName, to_wstring(CUtils::Random_Int(0, 4)).c_str());
	//	lstrcatW(strSoundFileName, L".wav");
	//  GI->Play_Sound(strSoundFileName, CHANNELID::SOUND_VOICE_MONSTER1, 1.f);
	//	break;
	//case Client::CMonster::NORMAL_1:
	//	lstrcatW(strSoundFileName, L"Voice_Normal_Monster_1_Damanged_Basic_");
	//	lstrcatW(strSoundFileName, to_wstring(CUtils::Random_Int(0, 5)).c_str());
	//	lstrcatW(strSoundFileName, L".wav");
	//	GI->Play_Sound(strSoundFileName, CHANNELID::SOUND_VOICE_MONSTER2, 1.f);
	//	break;
	//case Client::CMonster::NORMAL_2:
	//	lstrcatW(strSoundFileName, L"Voice_Normal_Monster_2_Damaged");
	//	lstrcatW(strSoundFileName, L".wav");
	//	GI->Play_Sound(strSoundFileName, CHANNELID::SOUND_VOICE_MONSTER3, 1.f);
	//	break;
	//case Client::CMonster::ENMU:
	//	lstrcatW(strSoundFileName, L"Voice_Enmu_Damaged_Basic_");
	//	lstrcatW(strSoundFileName, to_wstring(CUtils::Random_Int(0, 1)).c_str());
	//	lstrcatW(strSoundFileName, L".wav");
	//	GI->Play_Sound(strSoundFileName, CHANNELID::SOUND_VOICE_MONSTER1, 1.f, true);
	//	break;
	//case Client::CMonster::AKAZA:
	//	lstrcatW(strSoundFileName, L"Voice_Akakza_Damaged_Basic_");
	//	lstrcatW(strSoundFileName, to_wstring(CUtils::Random_Int(0, 3)).c_str());
	//	lstrcatW(strSoundFileName, L".wav");
	//	GI->Play_Sound(strSoundFileName, CHANNELID::SOUND_VOICE_MONSTER1, 1.f, true);
	//	break;
	//default:
	//	break;
	//}


}

void CMonster::Start_RimLight()
{
	m_bIsRimUse = true;
	m_fRimTimeAcc = 0.f;
}

void CMonster::Tick_RimLight(_float fTimeDelta)
{
	m_fRimTimeAcc += fTimeDelta;
	if (m_fRimTimeAcc >= m_fRimDuration)
		m_bIsRimUse = false;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pBTCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pControllerCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pDissoveTexture);
	Safe_Release(m_pDissolveObject);
	Safe_Release(m_pTransformCom);
		
}
