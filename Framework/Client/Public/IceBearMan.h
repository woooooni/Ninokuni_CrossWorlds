#pragma once

#include "Monster.h"

BEGIN(Client)

class CIceBearMan final : public CMonster
{
	CIceBearMan(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CIceBearMan(const CIceBearMan& rhs);
	virtual ~CIceBearMan() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

	HRESULT Render_ShadowDepth();

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

private:
	class CUI_MonsterHP_World* m_pHPBar = { nullptr };

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

public:
	static CIceBearMan* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

