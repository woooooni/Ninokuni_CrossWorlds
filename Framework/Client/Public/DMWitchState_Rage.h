#pragma once

#include "DMWitchState_Base.h"

BEGIN(Client)

class CDMWitchState_Rage final : public CDMWitchState_Base
{
private:
	CDMWitchState_Rage(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_Rage() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CDMWitchState_Rage* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

