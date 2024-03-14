#pragma once

#include "Client_Defines.h"
#include "State_Animals.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_BackFlip_02 : public CState_Animals
{
private:
	explicit CState_BackFlip_02(class CStateMachine* pMachine);
	virtual ~CState_BackFlip_02() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

public:
	static CState_BackFlip_02* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

