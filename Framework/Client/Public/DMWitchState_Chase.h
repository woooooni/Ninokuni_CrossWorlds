#pragma once

#include "DMWitchState_Base.h"

BEGIN(Client)

class CDMWitchState_Chase final : public CDMWitchState_Base
{
private:
	CDMWitchState_Chase(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_Chase() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CDMWitchState_Chase* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

