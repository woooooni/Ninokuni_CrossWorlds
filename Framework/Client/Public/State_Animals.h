#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Client)

class CState_Animals abstract : public CState
{
protected:
	explicit CState_Animals(class CStateMachine* pStateMachine);
	virtual ~CState_Animals() = default;

public:
	virtual HRESULT Initialize(const list<wstring>& AnimationList);

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(_float fTimeDelta) PURE;
	virtual void Exit_State() PURE;

protected:
	_bool ReturnToCenter(_float fTimeDelta, Vec4& vDir);
	Vec4 Get_Point_In_Bezier(const vector<Vec4>* points, const _float& fRatio);

protected:
	class CAnimals* m_pAnimal = nullptr;
public:
	virtual void Free() override;
};

END

