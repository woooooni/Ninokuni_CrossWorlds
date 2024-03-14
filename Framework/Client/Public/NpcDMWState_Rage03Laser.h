#pragma once

#include "NpcDMWState_Base.h"

BEGIN(Client)

class CNpcDMWState_Rage03Laser final : public CNpcDMWState_Base
{
private:
	CNpcDMWState_Rage03Laser(CStateMachine* pStateMachine);
	virtual ~CNpcDMWState_Rage03Laser() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int m_iCount = 0;
	_int m_iCurCount = 0;
	_bool m_bIsAttack = false;

	_float m_fSpawnTime = 0.f;
	_float m_fAccTime = 0.f;

public:
	static CNpcDMWState_Rage03Laser* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

