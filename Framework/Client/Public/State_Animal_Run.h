#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Run : public CState_Animals
{
private:
	explicit CState_Animal_Run(class CStateMachine* pMachine);
	virtual ~CState_Animal_Run() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	void SelectSound();

private:
	_bool m_bIsRun = true;
	Vec4 m_vDirection;

public:
	static CState_Animal_Run* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

