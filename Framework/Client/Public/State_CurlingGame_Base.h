#pragma once

#include "Client_Defines.h"
#include "Manager_State.h"

BEGIN(Client)

class CState_CurlingGame_Base abstract : public CManager_State
{
protected:
	CState_CurlingGame_Base(CManager_StateMachine* pStateMachine);
	virtual ~CState_CurlingGame_Base() = default;

public:
	virtual HRESULT Initialize() override;

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(const _float& fTimeDelta) override;
	virtual void LateTick_State(const _float& fTimeDelta) override;
	virtual void Exit_State() override;
	virtual HRESULT Render() override;

protected:
	class CCurlingGame_Manager* m_pManager = nullptr;

public:
	virtual void Free() override;

};

END
