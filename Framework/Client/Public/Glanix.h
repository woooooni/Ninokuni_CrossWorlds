#pragma once
#include "Boss.h"

BEGIN(Client)
class CGlanix final : public CBoss
{
public:
	enum GLANIX_STATE {
		GLANIX_INTRO_IDLE, GLANIX_INTRO_ROAR, GLANIX_INTRO_JUMP, GLANIX_INTRO_FINISH,
		GLANIX_SPAWN,
		GLANIX_COMBATIDLE,
		GLANIX_CHASE,
		GLANIX_ATTACK1, GLANIX_ATTACK2, GLANIX_SPINBOMB, GLANIX_JUMPSTAMP, GLANIX_QUADBLOW, GLANIX_ICEWAVE,
		GLANIX_CHARGE, GLANIX_SNOWBALL, GLANIX_SPINBOMBBOMB,
		GLANIX_BERSERK,
		/* 레이지1 */
		GLANIX_RAGESTART_TURN_OC, GLANIX_RAGESTART_JUMP, GLANIX_RAGESTART_TURN_OL,
		GLANIX_RAGESTART, GLANIX_RAGETURN, GLANIX_RAGEIDLE,
		GLANIX_RAGECHARGE, GLANIX_RAGECHARGE_END, 
		GLANIX_RAGERETURN, GLANIX_FINISHTURN_OL, GLANIX_RAGEPULL, GLANIX_RAGERISING, GLANIX_RAGESTAMP,
		GLANIX_RAGECRASH, GLANIX_RAGEJUMP_OFFSET,
		/* 레이지2 */
		GLANIX_RAGE2START_TURN_WP, GLANIX_RAGE2START_JUMP, GLANIX_RAGE2START_TURN_OL, GLANIX_RAGE2START,
		GLANIX_RAGE2WAVE, GLANIX_RAGE2IDLE,
		GLANIX_RAGE2RISING, GLANIX_RAGE2STAMP,

		GLANIX_TURN,
		GLANIX_STUN,
		GLANIX_COUNTERSTART, GLANIX_COUNTERLOOP, GLANIX_COUNTEREND,
		GLANIX_DEAD,
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
	virtual void Set_SkillTree() override;

	_bool	Get_IsCrash() { return m_bIsCrash; }
	void	Set_IsCrash(_bool bIsCrash) { m_bIsCrash = bIsCrash; }

	_vector	Get_WavePoint() { return m_vWavePoint; }

	/* Rage2 Stack */
	_uint*	Get_SlowStack() { return &m_iSlowStack; }
	void	Set_SlowStack(_int iAmount) { m_iSlowStack += iAmount; }

public:
	HRESULT Create_Pillars();
	HRESULT Delete_Pillar(const _int& iKey);
	HRESULT Clear_Pillars();

	class CGlanix_IcePillar_Controller* Get_PillarsController() const { return m_pPillarController; }

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

private:
	HRESULT Ready_Sockets();
	HRESULT Ready_Parts();

private:
	_float  m_fIntroTime = 0.f;

	_bool	m_bIsCrash = false;
	_vector	m_vWavePoint = {};

	_uint	m_iSlowStack = 0;

	class CGlanix_IcePillar_Controller* m_pPillarController = nullptr;
public:
	static CGlanix* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

