#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Stand : public CState_Vehicle
{
	enum SHOOT_STATE { NORMAL, PATTERN_0, PATTERN_1, PATTERN_2, PATTERN_3, SHOOT_STATE_END };
private:
	CState_EnemyBiplane_Stand(class CStateMachine* pMachine);
	~CState_EnemyBiplane_Stand() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	void Shoot_Normal(_float fTimeDelta);
	void Shoot_Pattern_0(_float fTimeDelta);
	void Shoot_Pattern_1(_float fTimeDelta);
	void Shoot_Pattern_2(_float fTimeDelta);
	void Shoot_Pattern_3(_float fTimeDelta);

private:
	_bool m_bFirstEnter = true;

	


private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = nullptr;
	

private:
	SHOOT_STATE m_eShootState = SHOOT_STATE::NORMAL;
	_int m_iShootCounts[SHOOT_STATE::SHOOT_STATE_END] = {};

private:
	_float m_fAccRecoveryTime = 0.f;
	_float m_fRecoveryTime = 3.f;

private:
	_float m_fAccCoolDown = 0.f;
	_float m_fCoolDownTime = 3.f;
	_bool m_bCoolDown = false;

private:
	_float m_fAccShootTime = 0.f;
	_float m_fShootTime = 0.2f;

private:
	Vec4 m_vInitialPositon = {};

private:
	class CCamera_Follow* m_pFollowCamera = nullptr;

	

public:
	static CState_EnemyBiplane_Stand* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END