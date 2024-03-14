#pragma once

#include "DMWitchState_Base.h"

BEGIN(Client)

class CDMWitchState_Attack1 final : public CDMWitchState_Base
{
private:
	CDMWitchState_Attack1(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_Attack1() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CDMWitchState_Attack1* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

