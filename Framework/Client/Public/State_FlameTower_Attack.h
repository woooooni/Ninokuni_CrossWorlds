#pragma once
#include "Client_Defines.h"
#include "State_DefenceTower.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_FlameTower_Attack : public CState_DefenceTower
{

private:
	CState_FlameTower_Attack(class CStateMachine* pMachine);
	~CState_FlameTower_Attack() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Fire();

private:
	_float m_fAccOnOffAttack = 0.f;
	_float m_fOnOffAttackTime = 0.05f;

private:
	class CEffect* m_pEffect_InBound = nullptr;
	class CEffect* m_pEffect_OutBound = nullptr;

private:
	_bool m_bFire = false;

public:
	static CState_FlameTower_Attack* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END