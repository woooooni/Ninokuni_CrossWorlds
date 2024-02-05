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

private:
	const _bool& Check_AllStoneStop();
	const _bool& Check_FinishGame();

	void Calculate_Score();
	void Send_To_Ui();


private:
	void Change_Turn();

private:
	_bool m_bResetTurn = false;
	_bool m_bSetNpcStoneTransform = false;

	const _float m_fTargetChangeLerpDuration = 0.75f;
	const _float m_fNpcStoneLimit = 0.5f;
	_float m_fAcc = 0.f;

	_bool	m_bChangeTarget = false;


	_bool	m_bCheckFinishGame = false;

public:
	static CState_CurlingGame_Launch_Stone* Create(CManager_StateMachine* pStateMachine);
	virtual void Free() override;

};

END
