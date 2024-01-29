#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_AfterSpinTail final : public CStelliaState_Base
{
private:
	CStelliaState_AfterSpinTail(CStateMachine* pStateMachine);
	virtual ~CStelliaState_AfterSpinTail() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bIsSkipFrame = false;

public:
	static CStelliaState_AfterSpinTail* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

