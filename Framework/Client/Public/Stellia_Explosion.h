#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)
class CStellia_Explosion final : public CMonsterProjectile
{
private:
	CStellia_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CStellia_Explosion(const CStellia_Explosion& rhs);
	virtual ~CStellia_Explosion() = default;

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
	_float m_fTime = 0.f;
	_float m_fDeletionTime = 0.f;
	_bool  m_bIsExplosion = false;

	class CDecal* m_pDecal = nullptr;

public:
	static CStellia_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

