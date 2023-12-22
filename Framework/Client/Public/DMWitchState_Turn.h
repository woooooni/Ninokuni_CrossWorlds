#pragma once

#include "DMWitchState_Base.h"

BEGIN(Client)

class CDMWitchState_Turn final : public CDMWitchState_Base
{
private:
	CDMWitchState_Turn(CStateMachine* pStateMachine);
	virtual ~CDMWitchState_Turn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fTime = 0.f;
	_float m_fWaitTime = 0.f;

	_vector m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;

public:
	static CDMWitchState_Turn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

