#pragma once

#include "State_CurlingGame_Base.h"

BEGIN(Client)

class CState_CurlingGame_Launch_Stone final : public CState_CurlingGame_Base
{
private:
	CState_CurlingGame_Launch_Stone(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Launch_Stone() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

public:
	static CState_CurlingGame_Launch_Stone* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END