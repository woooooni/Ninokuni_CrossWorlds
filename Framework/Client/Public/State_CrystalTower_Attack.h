#pragma once
#include "Client_Defines.h"
#include "State_DefenceTower.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_CrystalTower_Attack : public CState_DefenceTower
{

private:
	CState_CrystalTower_Attack(class CStateMachine* pMachine);
	~CState_CrystalTower_Attack() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void Fire();

private:
	_bool m_bShoot = false;

public:
	static CState_CrystalTower_Attack* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END