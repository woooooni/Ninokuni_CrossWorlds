#pragma once

#include "Client_Defines.h"
#include "Skill_Manager.h"
#include "UI_Manager.h"
#include "Camera_Manager.h"
#include "Camera_Action.h"
#include "Camera_Follow.h"
#include "State.h"


BEGIN(Client)

class CState_Vehicle abstract : public CState
{

protected:
	CState_Vehicle(class CStateMachine* pStateMachine);
	virtual ~CState_Vehicle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

	
protected:
	virtual void Reset_Camera_WideToDeafult();

protected:
	class CVehicle* m_pVehicle = nullptr;

public:
	virtual void Free() override;

};

END