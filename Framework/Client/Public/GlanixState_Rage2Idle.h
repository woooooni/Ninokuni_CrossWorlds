#pragma once

#include "GlanixState_Base.h"

BEGIN(Client)

class CGlanixState_Rage2Idle final : public CGlanixState_Base
{
private:
	CGlanixState_Rage2Idle(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Rage2Idle() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_float m_fTime = 0.f;
	_float m_fWaitTime = 0.f;

	_int   m_iWaveCount = 0;
	_int   m_iMaxCount = 0;

	_float m_fAccIcicleGen = 0.f;
	_float m_fGenTime = 0.5f;

private:
	void Generate_Icicle(_uint iCount);

public:
	static CGlanixState_Rage2Idle* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

