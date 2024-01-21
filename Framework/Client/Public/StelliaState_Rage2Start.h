#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Rage2Start final : public CStelliaState_Base
{
private:
	CStelliaState_Rage2Start(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Rage2Start() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

public:
	static CStelliaState_Rage2Start* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

