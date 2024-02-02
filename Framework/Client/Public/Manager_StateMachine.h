#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Client)

class CManager_State;

class CManager_StateMachine : public CBase
{
private:
	CManager_StateMachine();
	virtual ~CManager_StateMachine() = default;
	
public:
	virtual HRESULT Initialize(void* pArg);
	void Tick(const _float& fTimeDelta);
	void LateTick(const _float& fTimeDelta);
	void Render();

public:
	HRESULT Add_State(const _uint eStateTag, CManager_State* pState);
	HRESULT Change_State(const _uint eStateTag);
	CManager_State* Get_CurState() const { return m_pCurState; }
	CManager_State* Get_State(const _uint eStateTag) { return Find_State(eStateTag); }

private:
	CManager_State* Find_State(const _uint eStateTag);

private:
	CManager_State* m_pCurState = nullptr;
	map<_uint, CManager_State*> m_pStates;

public:
	static CManager_StateMachine* Create(void* pArg = nullptr);
	virtual void Free() override;
};

END