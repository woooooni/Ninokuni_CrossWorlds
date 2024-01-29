#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CManager_StateMachine;

class CManager_State abstract : public CBase
{
protected:
	CManager_State(CManager_StateMachine* pStateMachine);
	virtual ~CManager_State() = default;

public:
	virtual HRESULT Initialize();

public:
	virtual void Enter_State(void* pArg = nullptr) PURE;
	virtual void Tick_State(const _float& fTimeDelta) PURE;
	virtual void LateTick_State(const _float& fTimeDelta) PURE;
	virtual void Exit_State() PURE;

	virtual HRESULT Render() { return S_OK; }

public:
	const _uint& Get_StateTag() const { return m_iStateTag; }
	void Set_StateTag(const _uint& iStateTag) { m_iStateTag = iStateTag; }

protected:
	CManager_StateMachine* m_pManager_StateMachine = nullptr;

	_uint m_iStateTag = 0;

public:
	virtual void Free() override;
};

END

