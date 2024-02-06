#pragma once

#include "Client_Defines.h"
#include "State_Vehicle.h"

BEGIN(Engine)
class CStateMachine;
END

BEGIN(Client)

class CState_EnemyBiplane_Stand : public CState_Vehicle
{
private:
	CState_EnemyBiplane_Stand(class CStateMachine* pMachine);
	~CState_EnemyBiplane_Stand() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) override;
	virtual void Tick_State(_float fTimeDelta) override;
	virtual void Exit_State() override;


private:
	class CCharacter* m_pTarget = { nullptr };
	class CVehicle_Flying_EnemyBiplane* m_pEngineerPlane = nullptr;

private:
	_bool m_bLaunch_Pattern1 = false;
	_bool m_bLaunch_Pattern2 = false;
	_bool m_bLaunch_Pattern3 = false;

public:
	static CState_EnemyBiplane_Stand* Create(class CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};
END