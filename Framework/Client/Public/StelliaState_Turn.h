#pragma once

#include "StelliaState_Base.h"

BEGIN(Client)

class CStelliaState_Turn final : public CStelliaState_Base
{
private:
	CStelliaState_Turn(CStateMachine* pStateMachine);
	virtual ~CStelliaState_Turn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;

public:
	static CStelliaState_Turn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

