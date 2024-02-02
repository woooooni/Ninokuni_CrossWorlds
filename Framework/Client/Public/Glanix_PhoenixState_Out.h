#pragma once

#include "Glanix_PhoenixState_Base.h"

BEGIN(Client)

class CGlanix_PhoenixState_Out final : public CGlanixPhoenixState_Base
{
private:
	CGlanix_PhoenixState_Out(CStateMachine* pStateMachine);
	virtual ~CGlanix_PhoenixState_Out() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_bool m_bEffectCreate = false;

public:
	static CGlanix_PhoenixState_Out* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

