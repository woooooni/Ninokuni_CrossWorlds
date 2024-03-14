#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_BackFlip_01 : public CState_Animals
{
private:
	explicit CState_BackFlip_01(class CStateMachine* pMachine);
	virtual ~CState_BackFlip_01() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fMaxAngle = 200.0f;
	_float m_fAcc = 0.0f;
public:
	static CState_BackFlip_01* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

