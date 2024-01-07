#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Character_Battle_Dash : public CState_Character
{

private:
	CState_Character_Battle_Dash(class CStateMachine* pMachine);
	~CState_Character_Battle_Dash() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	Vec3 m_vDir = {};
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};


public:
	static CState_Character_Battle_Dash* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END