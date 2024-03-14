#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)
class CWitch_Laser final : public CMonsterProjectile
{
private:
	CWitch_Laser(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CWitch_Laser(const CWitch_Laser& rhs);
	virtual ~CWitch_Laser() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	class CVfx* m_pLaserWarning = nullptr;
	class CVfx* m_pLaser = nullptr;

	_bool m_bIsLaserWarningCreate = false;
	_bool m_bIsLaserCreate = false;

	_float m_fAccTime = 0.f;
	_float m_fActiveTime = 0.f;

public:
	static CWitch_Laser* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

