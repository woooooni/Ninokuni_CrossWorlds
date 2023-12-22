#pragma once
#include "Boss.h"

BEGIN(Client)
class CDMWitch final : public CBoss
{
public:
	enum DMWITCH_STATE {
		DMWITCH_SPAWN,
		DMWITCH_CHASE, DMWITCH_TELEPORT,
		DMWITCH_ATTACK1, DMWITCH_ATTACK2,
		DMWITCH_QUADBOMB, DMWITCH_CHARGE, DMWITCH_TRAMPOLINE,
		DMWITCH_COLLFIRE, DMWITCH_VULCANBALL, 
		DMWITCH_PULLANDCOMBO, 
		DMWITCH_CHILDDREAM_START, DMWITCH_CHILDDREAM_VULCANBALL, DMWITCH_CHILDDREAM_END,
		DMWITCH_RAGE,
		DMWITCH_TURN,
		DMWITCH_DEAD,
		DMWITCH_END
	};

private:
	CDMWitch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CDMWitch(const CDMWitch& rhs);
	virtual ~CDMWitch() = default;

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
	static CDMWitch* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

