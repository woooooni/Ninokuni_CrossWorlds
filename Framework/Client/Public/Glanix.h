#pragma once
#include "Boss.h"

BEGIN(Client)
class CGlanix final : public CBoss
{
public:
	enum GLANIX_STATE {
		GLANIX_SPAWN,
		GLANIX_COMBATIDLE,
		GLANIX_CHASE,
		GLANIX_ATTACK1, GLANIX_ATTACK2,
		GLANIX_END
	};

private:
	CGlanix(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CGlanix(const CGlanix& rhs);
	virtual ~CGlanix() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;


public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

public:
	static CGlanix* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

