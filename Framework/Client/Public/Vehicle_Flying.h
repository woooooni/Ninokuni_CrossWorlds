#pragma once

#include "Vehicle.h"

BEGIN(Client)
class CVehicle_Flying abstract : public CVehicle
{
protected:
	CVehicle_Flying(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CVehicle_Flying(const CVehicle_Flying& rhs);
	virtual ~CVehicle_Flying() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT Render_ShadowDepth();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

protected:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();

public:
	virtual void Free() override;
};
END

