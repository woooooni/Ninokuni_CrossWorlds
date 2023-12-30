#pragma once

#include "Client_Defines.h"
#include "Weapon.h"

BEGIN(Client)

class CSword final : public CWeapon
{

private:
	CSword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual ~CSword() = default;

public:
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_Colliders();
	HRESULT Bind_ShaderResources();

public:
	static CSword* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END