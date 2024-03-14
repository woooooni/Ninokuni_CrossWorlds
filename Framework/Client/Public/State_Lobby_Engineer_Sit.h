#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)
class CState_Lobby_Engineer_Sit : public CState_Character
{
private:
	CState_Lobby_Engineer_Sit(class CStateMachine* pMachine);
	~CState_Lobby_Engineer_Sit() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	class CUI_Dummy_Engineer* m_pEngineer = { nullptr };
	float m_fSitTimeAcc = 0.f;
	float m_fSitMotionTime = 5.f;

public:
	static CState_Lobby_Engineer_Sit* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};
END