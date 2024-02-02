#pragma once
#include "Client_Defines.h"
#include "State_Character.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_Destroyer_Skill_LeafSlam : public CState_Character
{

private:
	CState_Destroyer_Skill_LeafSlam(class CStateMachine* pMachine);
	~CState_Destroyer_Skill_LeafSlam() = default;


public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);


public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;

private:
	MOTION_TRAIL_DESC m_MotionTrailDesc = {};
	_bool m_bTrailStart = false;

private:
	_float m_fRadialBlurPower = 0.1f;
public:
	static CState_Destroyer_Skill_LeafSlam* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;

};

END