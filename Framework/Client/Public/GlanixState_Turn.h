#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_Turn final : public CGlanixState_Base
{
private:
	CGlanixState_Turn(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Turn() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_vector m_vDestPos = {};
	_float  m_fTurnSpeed = 0.f;
	_float  m_fWalkSpeed = 0.f;

	_bool   m_bIsRageInit = false;
	_bool   m_bIsRage2Init = false;


public:
	static CGlanixState_Turn* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

