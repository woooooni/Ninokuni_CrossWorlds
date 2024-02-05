#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CState_RubyIdle final : public CNpcState_Base
{
private:
	CState_RubyIdle(CStateMachine* pStateMachine);
	virtual ~CState_RubyIdle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fTime = 0.f;
	class CRubyCarriage* m_pRubyCarriage = nullptr;

	_bool m_bFirst = false;

public:
	static CState_RubyIdle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

