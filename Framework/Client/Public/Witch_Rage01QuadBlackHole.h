#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)
class CWitch_Rage01QuadBlackHole final : public CMonsterProjectile
{
private:
	CWitch_Rage01QuadBlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CWitch_Rage01QuadBlackHole(const CWitch_Rage01QuadBlackHole& rhs);
	virtual ~CWitch_Rage01QuadBlackHole() = default;

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
	class CVfx* m_pBlackHole = nullptr;
	class CVfx* m_pEffectBomb = nullptr;

	_bool m_bIsBlackHoleCreate = false;
	_bool m_bIsBombCreate = false;

	_float m_fAccTime = 0.f;
	_float m_fActiveTime = 0.f;

public:
	static CWitch_Rage01QuadBlackHole* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

