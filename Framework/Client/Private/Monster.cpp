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
#include "BehaviorTree.h"

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

	m_fDissolveWeight = 1.f;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	GI->Add_CollisionGroup(COLLISION_GROUP::MONSTER, this);

	for (auto& pPart : m_Parts)
		pPart->Tick(fTimeDelta);

	if (m_bInfinite)
	{
		m_fAccInfinite += fTimeDelta;
		if (m_fAccInfinite >= m_fInfiniteTime)
		{
			m_bInfinite = false;
			m_fAccInfinite = 0.f;

			Set_ActiveColliders(CCollider::DETECTION_TYPE::HEAD, true);
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, true);
		}

	}

	if (m_bReserveDead)
	{
		m_fDissolveWeight += 0.2f * fTimeDelta;
		if (m_fDissolveWeight >= 2.f)
		{
			Set_ActiveColliders(CCollider::DETECTION_TYPE::HEAD, false);
			Set_ActiveColliders(CCollider::DETECTION_TYPE::BODY, false);
			Set_Dead(true);
		}
	}

	if(m_pBTCom != nullptr)
		m_pBTCom->Tick(fTimeDelta);
}

void CMonster::LateTick(_float fTimeDelta)
{
	if (nullptr != m_pModelCom)
		m_pModelCom->LateTick(fTimeDelta);

	for (auto& pPart : m_Parts)
		pPart->LateTick(fTimeDelta);

	for (auto& pPart : m_Parts)
	{
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, pPart);
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, pPart);
	}

	__super::LateTick(fTimeDelta);
	/*m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_SHADOW, this);*/
	// m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc());
	m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc());



#ifdef _DEBUG
	for (_uint i = 0; i < CCollider::DETECTION_TYPE::DETECTION_END; ++i)
	{
		for (auto& pCollider : m_Colliders[i])
			m_pRendererCom->Add_Debug(pCollider);
	}
	//m_pRendererCom->Add_Debug(m_pRigidBodyCom);
#endif // DEBUG

	
	//m_pRigidBodyCom->Update_RigidBody(fTimeDelta);
	if (m_pBTCom != nullptr)
		m_pBTCom->LateTick(fTimeDelta);
}

HRESULT CMonster::Render()
{
	// __super::Render();

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
	if (m_bInfinite)
		vRimColor = { 1.f, 0.f, 0.f, 1.f };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vRimColor", &vRimColor, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();


	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		_uint		iPassIndex = 0;
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			iPassIndex = 0;
		else
			iPassIndex++;

		if (true == m_bReserveDead)
		{
			iPassIndex = 2;
			if (FAILED(m_pShaderCom->Bind_RawValue("g_fDissolveWeight", &m_fDissolveWeight, sizeof(_float))))
				return E_FAIL;

			if (FAILED(m_pDissoveTexture->Bind_ShaderResource(m_pShaderCom, "g_DissolveTexture")))
				return E_FAIL;
		}

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, iPassIndex)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CMonster::Render_ShadowDepth()
{

	if (nullptr == m_pShaderCom || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &GI->Get_ShadowViewMatrix(GI->Get_CurrentLevel()))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;



	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(0), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 10)))
			return E_FAIL;
	}


	return S_OK;
}

HRESULT CMonster::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
{
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

	if (FAILED(m_pModelCom->SetUp_VTF(m_pShaderCom)))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();
	_uint iPassIndex	= 0;

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
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
	{
		if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
		{
			// 둘 다 켜줘야 함.
			// ATKAROUND는 공격 동작이 끝나고 추적할 것인지 결정하기 위함.
			// 범위내에 없으면 ATK를 false 시켜주기 위해.
			m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
			m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = true;
		}
	}

	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER || tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_WEAPON)
	{
		if (tInfo.pMyCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::BODY && tInfo.pOtherCollider->Get_DetectionType() == CCollider::DETECTION_TYPE::ATTACK)
		{
			On_Damaged(tInfo);
		}
	}
}

void CMonster::Collision_Continue(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
	{
		if (m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_COMBAT])
		{
			m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = true;
			// m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATK] = true;
		}
	}
}

void CMonster::Collision_Exit(const COLLISION_INFO& tInfo)
{
	if (tInfo.pOther->Get_ObjectType() == OBJ_TYPE::OBJ_CHARACTER)
	{
		m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_ATKAROUND] = false;
	}
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
	// m_bBools[(_uint)MONSTER_BOOLTYPE::MONBOOL_HITANIM] = true;
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

}
