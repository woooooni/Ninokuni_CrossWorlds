#pragma once

#include "GlanixState_Base.h"
BEGIN(Client)

class CGlanixState_Rage2Wave final : public CGlanixState_Base
{
private:
	CGlanixState_Rage2Wave(CStateMachine* pStateMachine);
	virtual ~CGlanixState_Rage2Wave() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	_int	m_iWaveCount = 0;
	_int    m_iMaxCount = 0;

	_float m_fAccIcicleGen = 0.f;
	_float m_fGenTime = 0.5f;

private:
	void Generate_Icicle(_uint iCount);

public:
	static CGlanixState_Rage2Wave* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

