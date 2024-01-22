#pragma once

#include "MonsterProjectile.h"

BEGIN(Client)

class CStellia_Crystal_Explosion final : public CMonsterProjectile
{
private:
	CStellia_Crystal_Explosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CStellia_Crystal_Explosion(const CStellia_Crystal_Explosion& rhs);
	virtual ~CStellia_Crystal_Explosion() = default;

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

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_Colliders();

private:
	class CStellia_Crystal_Destructible* m_pCrystal = nullptr;

	_float m_fTime = 0.f;
	_float m_fDelteTime = 0.f;

	_bool m_bFirst = true;

public:
	static CStellia_Crystal_Explosion* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

