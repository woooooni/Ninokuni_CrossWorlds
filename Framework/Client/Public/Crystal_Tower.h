#pragma once

#include "Client_Defines.h"
#include "Defence_Tower.h"

BEGIN(Client)
class CCrystal_Tower final : public CDefence_Tower
{
private:
	CCrystal_Tower(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCrystal_Tower(const CCrystal_Tower& rhs);
	virtual ~CCrystal_Tower() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) override;
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) override;

protected:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

protected:
	virtual void Fire() override;


public:
	static CCrystal_Tower* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
