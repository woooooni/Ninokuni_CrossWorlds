#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)
class CWitch_BlackHole final : public CMonsterProjectile
{
private:
	CWitch_BlackHole(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CWitch_BlackHole(const CWitch_BlackHole& rhs);
	virtual ~CWitch_BlackHole() = default;

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
	_float m_fAccTime = 0.f;

public:
	static CWitch_BlackHole* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

