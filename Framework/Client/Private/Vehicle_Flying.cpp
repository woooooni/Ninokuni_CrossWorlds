#include "stdafx.h"
#include "GameInstance.h"
#include "Vehicle_Flying.h"

CVehicle_Flying::CVehicle_Flying(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag)
	: CVehicle(pDevice, pContext, strObjectTag)
{
}

CVehicle_Flying::CVehicle_Flying(const CVehicle_Flying& rhs)
	: CVehicle(rhs)
{
}

HRESULT CVehicle_Flying::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Colliders()))
		return E_FAIL;

	if (FAILED(Ready_States()))
		return E_FAIL;

	return S_OK;
}

void CVehicle_Flying::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CVehicle_Flying::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CVehicle_Flying::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CVehicle_Flying::Render_ShadowDepth()
{
	__super::Render_ShadowDepth();

	return S_OK;
}

void CVehicle_Flying::Collision_Enter(const COLLISION_INFO& tInfo)
{
	__super::Collision_Enter(tInfo);
}

void CVehicle_Flying::Collision_Continue(const COLLISION_INFO& tInfo)
{
	__super::Collision_Continue(tInfo);
}

void CVehicle_Flying::Collision_Exit(const COLLISION_INFO& tInfo)
{
	__super::Collision_Exit(tInfo);
}

void CVehicle_Flying::Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Enter(tInfo);
}

void CVehicle_Flying::Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Continue(tInfo);
}

void CVehicle_Flying::Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo)
{
	__super::Ground_Collision_Exit(tInfo);
}

HRESULT CVehicle_Flying::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVehicle_Flying::Ready_States()
{

	return S_OK;
}

HRESULT CVehicle_Flying::Ready_Colliders()
{

	return S_OK;
}

void CVehicle_Flying::Free()
{
	__super::Free();
}
