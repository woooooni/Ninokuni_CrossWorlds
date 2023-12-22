#pragma once

#include "DMWitchState_Base.h"

BEGIN(Client)

class CDMWitchState_CDEnd final : public CDMWitchState_Base
{
private:
	CDMWitchState_CDEnd(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_CDEnd() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CDMWitchState_CDEnd* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

