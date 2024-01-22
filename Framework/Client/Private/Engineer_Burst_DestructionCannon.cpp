#include "stdafx.h"
#include "GameInstance.h"
#include "Engineer_Burst_DestructionCannon.h"
#include "Utils.h"
#include "Effect_Manager.h"
#include "Particle_Manager.h"
#include "Character_Manager.h"
#include "Character.h"
#include "Effect.h"

CEngineer_Burst_DestructionCannon::CEngineer_Burst_DestructionCannon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CCharacter_Projectile(pDevice, pContext, L"Engineer_Burst_DestructionCannon")
{

}

CEngineer_Burst_DestructionCannon::CEngineer_Burst_DestructionCannon(const CEngineer_Burst_DestructionCannon& rhs)
	: CCharacter_Projectile(rhs)
{
}


HRESULT CEngineer_Burst_DestructionCannon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEngineer_Burst_DestructionCannon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pModelCom->Set_Animation(0);	

	return S_OK;
}

void CEngineer_Burst_DestructionCannon::Tick(_float fTimeDelta)
{

	if (false == m_pModelCom->Is_Tween() && true == m_pModelCom->Is_Finish())
	{
		if (false == m_bReserveDead)
		{
			Reserve_Dead(true);
		}
	}

	if (false == m_pModelCom->Is_Tween() && m_pModelCom->Get_CurrAnimationFrame() >= 44)
	{
		if (false == m_bShot)
		{
			Fire_Cannon();
			m_bShot = true;
		}
	}

	if (true == m_bReserveDead)
	{
		m_fDissolveWeight += m_fDissolveSpeed * fTimeDelta;
		if (m_fDissolveWeight >= m_fDissolveTotal)
			Set_Dead(true);

	}
}

void CEngineer_Burst_DestructionCannon::LateTick(_float fTimeDelta)
{
	m_pModelCom->LateTick(fTimeDelta);

	m_AnimInstanceDesc.fDissolveDuration = m_fDissolveDuration;
	m_AnimInstanceDesc.fDissolveSpeed = m_fDissolveDuration;
	m_AnimInstanceDesc.fDissolveTotal = m_fDissolveTotal;
	m_AnimInstanceDesc.fDissolveWeight = m_fDissolveWeight;
	m_AnimInstanceDesc.vDissolveColor = m_vDissolveColor;
	m_AnimInstanceDesc.vRimColor = { 0.f, 0.f, 0.f, 0.f };

	Compute_CamZ(m_pTransformCom->Get_Position());

	if (m_fCamDistance <= 20.f && true == GI->Intersect_Frustum_World(m_pTransformCom->Get_Position(), 5.f))
	{
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_NONBLEND, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
		m_pRendererCom->Add_RenderGroup_AnimInstancing(CRenderer::RENDER_SHADOW, this, m_pTransformCom->Get_WorldFloat4x4(), m_pModelCom->Get_TweenDesc(), m_AnimInstanceDesc);
	}
}

HRESULT CEngineer_Burst_DestructionCannon::Render_Instance(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices)
{
	return S_OK;
}

HRESULT CEngineer_Burst_DestructionCannon::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
{
	if (nullptr == pInstancingShader || nullptr == m_pTransformCom)
		return E_FAIL;


	if (FAILED(pInstancingShader->Bind_RawValue("g_WorldMatrix", &m_pTransformCom->Get_WorldFloat4x4_TransPose(), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ViewMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_VIEW), sizeof(_float4x4))))
		return E_FAIL;

	if (FAILED(pInstancingShader->Bind_RawValue("g_ProjMatrix", &GI->Get_TransformFloat4x4_TransPose(CPipeLine::D3DTS_PROJ), sizeof(_float4x4))))
		return E_FAIL;

	/*Matrix worldInvTranspose = m_pTransformCom->Get_WorldMatrixInverse();
	worldInvTranspose.Transpose();

	Matrix worldInvTransposeView = worldInvTranspose * GI->Get_TransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(pInstancingShader->Bind_Matrix("WorldInvTransposeView", &worldInvTransposeView)))
		return E_FAIL;*/

	if (FAILED(pInstancingShader->Bind_RawValue("g_TweenFrames_Array", TweenDesc.data(), sizeof(TWEEN_DESC) * TweenDesc.size())))
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
	if (FAILED(m_pDissolveTextureCom->Bind_ShaderResource(pInstancingShader, "g_DissolveTexture", 51)))
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

HRESULT CEngineer_Burst_DestructionCannon::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc)
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








HRESULT CEngineer_Burst_DestructionCannon::Ready_Components()
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), reinterpret_cast<CComponent**>(&m_pRendererCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Transform"), TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Engineer_BurstSkill_Cannon"), TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Effect_Noise"), TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pDissolveTextureCom))))
		return E_FAIL;

	


	return S_OK;
}

void CEngineer_Burst_DestructionCannon::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CEngineer_Burst_DestructionCannon::Fire_Cannon()
{
	CGameObject* pGameObject = nullptr;
	CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
	ProjectileDesc.pOwner = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE::ENGINEER);

	if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Engineer_Burst_CannonBomb", &ProjectileDesc, &pGameObject)))
	{
		MSG_BOX("Add GameObject Failed : CEngineer_Burst_DestructionCannon::Fire_Cannon");
		return;
	}

	CTransform* pTransform = pGameObject->Get_Component<CTransform>(L"Com_Transform");
	if (nullptr == pTransform)
	{
		MSG_BOX("Find Transform Failed : CEngineer_Burst_DestructionCannon::Fire_Cannon");
		return;
	}

	_matrix WorldMatrix = m_pTransformCom->Get_WorldMatrix();
	WorldMatrix.r[CTransform::STATE_POSITION] += (XMVector3Normalize(m_pTransformCom->Get_Look()) * 3.f);
	WorldMatrix.r[CTransform::STATE_POSITION] += XMVectorSet(0.f, 1.5f, 0.f, 0.f);
	pTransform->Set_WorldMatrix(WorldMatrix);

}


CEngineer_Burst_DestructionCannon* CEngineer_Burst_DestructionCannon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEngineer_Burst_DestructionCannon* pInstance = new CEngineer_Burst_DestructionCannon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Create Failed to ProtoType : CEngineer_Burst_DestructionCannon");
		Safe_Release<CEngineer_Burst_DestructionCannon*>(pInstance);
		return nullptr;
	}

	return pInstance;
}

CGameObject* CEngineer_Burst_DestructionCannon::Clone(void* pArg)
{
	CEngineer_Burst_DestructionCannon* pInstance = new CEngineer_Burst_DestructionCannon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Create Failed to Cloned : CEngineer_Burst_DestructionCannon");
		Safe_Release<CEngineer_Burst_DestructionCannon*>(pInstance);
	}

	return pInstance;
}

void CEngineer_Burst_DestructionCannon::Free()
{
	__super::Free();
	Safe_Release(m_pDissolveTextureCom);
}
