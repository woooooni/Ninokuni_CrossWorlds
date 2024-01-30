#pragma once

#include "NpcState_Base.h"

BEGIN(Client)

class CUniqueNpcState_Seat final : public CNpcState_Base
{
private:
	enum SECTION_TYPE { SECTION_ONE, SECTION_TWO, SECTION_THREE, SECTION_FOUR, SECTION_END };
private:
	CUniqueNpcState_Seat(CStateMachine* pStateMachine);
	virtual ~CUniqueNpcState_Seat() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr);
	virtual void Tick_State(_float fTimeDelta);
	virtual void Exit_State();

private:
	Vec3 CubicBezier(Vec4 p0, Vec4 p1, Vec4 p2, Vec4 p3, _float fTime);

private:
	_float m_fTime = 0.0f;
	_bool m_bSection[SECTION_END] = { false, false, false, false };

public:
	static CUniqueNpcState_Seat* Create(CStateMachine* pStateMachine, const list<wstring>& AnimationList);
	virtual void Free() override;
};

END

