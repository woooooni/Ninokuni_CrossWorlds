#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CNpcDestroyerState_Noisy final : public CNpcState_Base
{
private:
	CNpcDestroyerState_Noisy(CStateMachine* pStateMachine);
	virtual ~CNpcDestroyerState_Noisy() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int m_iAnimIndex = 0;
	vector<wstring> m_vecSound;

public:
	static CNpcDestroyerState_Noisy* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

