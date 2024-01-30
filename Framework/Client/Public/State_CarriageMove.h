#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_CarriageMove : public CState
{
private:
	explicit CState_CarriageMove(class CStateMachine* pMachine);
	virtual ~CState_CarriageMove() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	_float m_fTime = 0.0f;
	_bool m_bFirst = false;
	class CRuby* m_pRuby = nullptr;

public:
	static CState_CarriageMove* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

