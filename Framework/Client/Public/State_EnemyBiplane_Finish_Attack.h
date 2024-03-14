#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Finish_Attack : public CState_Vehicle
{
public:
	enum FINISH_ATTACK_TYPE { HOME, BURGER, TYPE_END };
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
	class CVehicle_Flying_Biplane* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = { nullptr };

	_bool m_bUpdate = { false };
	_float m_fMovingSpeed = { 5.f };

private:
	vector<class CGameObject*> m_Missiles = {};
	_float m_fAccGenMissile = 0.f;
	_float m_fGenMissileTime = 0.05f;

private:
	vector<_int> m_ComputedAttacks[FINISH_ATTACK_TYPE::TYPE_END];
	

private:
	ID3D11ComputeShader* m_pComputeShader = nullptr;


public:
	static CState_EnemyBiplane_Finish_Attack* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END