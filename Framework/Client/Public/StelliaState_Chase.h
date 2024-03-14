#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Chase final : public CStelliaState_Base
{
private:
	CStelliaState_Chase(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Chase() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fChaseTime = 0.f;
	_float m_fTime = 0.f;

public:
	static CStelliaState_Chase* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

