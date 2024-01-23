#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CEngineer_BurstCall_Missile final : public CCharacter_Projectile
{
private:
	explicit CEngineer_BurstCall_Missile(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CEngineer_BurstCall_Missile(const CEngineer_BurstCall_Missile& rhs);
	virtual ~CEngineer_BurstCall_Missile() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual HRESULT Ready_Components() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;

private:
	_float m_fAccOnOff = 0.f;
	_float m_fOnOffTime = 0.1f;


public:
	static CEngineer_BurstCall_Missile* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

