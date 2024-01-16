#pragma once
#include "Boss.h"

BEGIN(Client)
class CStellia final : public CBoss
{
public:
	enum STELLIA_STATE {
		STELLIA_SPAWN,
		STELLIA_COMBATIDLE,
		STELLIA_CHASE,
		STELLIA_ATTACK1, STELLIA_ATTACK2, STELLIA_SPINTAIL, STELLIA_JUMPSTAMP, STELLIA_LASER, 
		STELLIA_TRIPLELASER, STELLIA_CHARGE, STELLIA_BIGBANG,
		STELLIA_CHASEJUMPSTAMP,
		STELLIA_COUNTERSTART, STELLIA_COUNTERLOOP, STELLIA_COUNTEREND,
		STELLIA_BERSERK,
		STELLIA_TURN,
		STELLIA_DEAD,
		STELLIA_END
	};

private:
	CStellia(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CStellia(const CStellia& rhs);
	virtual ~CStellia() = default;

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
	virtual void Set_SkillTree() override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

public:
	static CStellia* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

