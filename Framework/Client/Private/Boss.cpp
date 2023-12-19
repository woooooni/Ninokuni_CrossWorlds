#include "stdafx.h"
#include "Boss.h"

CBoss::CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat)
	: CMonster(pDevice, pContext, strObjectTag, tStat)
{
}

CBoss::CBoss(const CBoss& rhs)
	: CMonster(rhs)
{
}

HRESULT CBoss::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CBoss::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CBoss::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CBoss::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CBoss::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CBoss::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

HRESULT CBoss::Render_Instance_AnimModel(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
{
	__super::Render_Instance_AnimModel(pInstancingShader, pInstancingBuffer, WorldMatrices, TweenDesc);

	return S_OK;
}

HRESULT CBoss::Render_Instance_AnimModel_Shadow(CShader* pInstancingShader, CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc)
{
	__super::Render_Instance_AnimModel_Shadow(pInstancingShader, pInstancingBuffer, WorldMatrices, TweenDesc);

	return S_OK;
}

void CBoss::Collision_Enter(const COLLISION_INFO& tInfo)
{
}

void CBoss::Collision_Continue(const COLLISION_INFO& tInfo)
{
}

void CBoss::Collision_Exit(const COLLISION_INFO& tInfo)
{
}

CHierarchyNode* CBoss::Get_Socket(const wstring& strSocketName)
{
	return nullptr;
}

void CBoss::On_Damaged(const COLLISION_INFO& tInfo)
{
}

HRESULT CBoss::Ready_Components()
{
	return S_OK;
}

HRESULT CBoss::Ready_States()
{
	return S_OK;
}

HRESULT CBoss::Ready_Colliders()
{
	return S_OK;
}

void CBoss::LookAt_DamagedObject(CGameObject* pAttacker)
{
}

void CBoss::Play_DamagedSound()
{
}

void CBoss::Free()
{
	__super::Free();
}
