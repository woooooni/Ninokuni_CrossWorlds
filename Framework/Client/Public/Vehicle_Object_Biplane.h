#pragma once

#include "Vehicle_Flying.h"

BEGIN(Client)
class CVehicle_Object_Biplane final : public CVehicle_Flying
{
protected:
	CVehicle_Object_Biplane(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVehicle_Object_Biplane(const CVehicle_Object_Biplane& rhs);
	virtual ~CVehicle_Object_Biplane() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	HRESULT Render_ShadowDepth();

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT	Ready_Colliders();
	virtual HRESULT Ready_States();

public:
	static CVehicle_Object_Biplane* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END

