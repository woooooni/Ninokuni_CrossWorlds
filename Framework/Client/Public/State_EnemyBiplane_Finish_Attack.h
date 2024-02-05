#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Finish_Attack : public CState_Vehicle
{
private:
	CState_EnemyBiplane_Finish_Attack(class CStateMachine* pMachine);
	~CState_EnemyBiplane_Finish_Attack() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = { nullptr };

	_bool m_bUpdate = { false };
	_float m_fMovingSpeed = { 5.f };

private:
	vector<class CGameObject*> m_Missiles = {};
	_float m_fAccGenMissile = 0.f;
	_float m_fGenMissileTime = 0.05f;

private:
	vector<_int> m_HamburgerColors;
	vector<_int> m_HomeColors;
	vector<_int> m_NyahColors;
	

private:
	class CShader* m_pComputeShader = nullptr;


public:
	static CState_EnemyBiplane_Finish_Attack* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END