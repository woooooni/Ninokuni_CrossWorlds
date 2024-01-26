#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Animal_Stand : public CState_Animals
{
private:
	explicit CState_Animal_Stand(class CStateMachine* pMachine);
	virtual ~CState_Animal_Stand() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_bool m_bLifting = false;

	_bool m_bExit = false;
	_int m_iPrevAnim = 0;

public:
	static CState_Animal_Stand* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};


END

