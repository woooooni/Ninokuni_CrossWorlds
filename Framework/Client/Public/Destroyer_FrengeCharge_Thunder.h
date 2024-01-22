#pragma once

#include "Client_Defines.h"
#include "Character_Projectile.h"

BEGIN(Client)

class CDestroyer_FrengeCharge_Thunder final : public CCharacter_Projectile
{
private:
	explicit CDestroyer_FrengeCharge_Thunder(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CDestroyer_FrengeCharge_Thunder(const CDestroyer_FrengeCharge_Thunder& rhs);
	virtual ~CDestroyer_FrengeCharge_Thunder() = default;

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
	static CDestroyer_FrengeCharge_Thunder* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

