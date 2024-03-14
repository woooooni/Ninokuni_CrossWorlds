#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Closed : public CState
{
private:
	explicit CState_Closed(class CStateMachine* pMachine);
	virtual ~CState_Closed() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fTime = 0.0f;
public:
	static CState_Closed* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

