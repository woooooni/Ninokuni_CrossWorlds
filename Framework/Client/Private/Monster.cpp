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

	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	/* 최초 타겟 설정 */
	if (m_tTargetDesc.pTarget == nullptr)
	{
		m_tTargetDesc.pTarget = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
		m_tTargetDesc.pTragetTransform = m_tTargetDesc.pTarget->Get_Component<CTransform>(L"Com_Transform");
	}

	for (auto& pPart : m_Parts)
		pPart->Tick(fTimeDelta);

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

	m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
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
		else if (m_pDissolveObject != nullptr && m_fDissolveWeight >= (m_fDissolveTotal - 3.f))
		{
			m_pDissolveObject->Set_Dead(true);
			m_pDissolveObject = nullptr;
		}
		else if (m_fDissolveWeight >= m_fDissolveTotal)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
			return;
		}
		else
			m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
	}
}

void CMonster::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

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


	if (true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 120.f))
	{
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
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

HRESULT CMonster::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
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
	/* 공격 */
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY &&
		tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY)
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

void CMonster::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
		tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
		{
			m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
		}
	}
}

void CMonster::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if(tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BOUNDARY &&
	   tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER &&
	   tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY)
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = false;
	}
}

void CMonster::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);

	if (m_pRigidBodyCom->Get_Velocity().y <= 0.f)
	{
		m_pRigidBodyCom->Set_Ground(true);
		m_pRigidBodyCom->Set_Use_Gravity(false);
	}
}

void CMonster::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
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


CHierarchyNode* CMonster::Get_Socket(const wstring& strSocketName)
{
	for (auto& pSocket : m_Sockets)
	{
		if (pSocket->Get_Name() == strSocketName)
			return pSocket;
	}
	return nullptr;
}

void CMonster::On_Damaged(const COLLISION_INFO& tInfo)
{
	m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ISHIT] = true;

	CUIDamage_Manager::GetInstance()->Create_MonsterDamageNumber(m_pTransformCom, CUIDamage_Manager::UI_DAMAGETYPE::NONE, dynamic_cast<CCharacter*>(tInfo.pOther)->Get_Stat().iAtt);
	m_tStat.fHp -= dynamic_cast<CCharacter*>(tInfo.pOther)->Get_Stat().iAtt;

	Start_RimLight();
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

	for (auto& pSocket : m_Sockets)
		Safe_Release(pSocket);

	for (auto& pPart : m_Parts)
		Safe_Release(pPart);

	m_Parts.clear();

	Safe_Release(m_pBTCom);
	
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pRigidBodyCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pDissoveTexture);
	Safe_Release(m_pControllerCom);

}
