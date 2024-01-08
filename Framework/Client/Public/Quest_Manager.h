#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "MainQuest.h"

BEGIN(Client)

class CQuest_Manager final : public CBase
{
	DECLARE_SINGLETON(CQuest_Manager)
private:
	CQuest_Manager();
	virtual ~CQuest_Manager() = default;

public:
	HRESULT Reserve_Manager();
	void	Tick(_float fTimeDelta);
	void    LateTick(_float fTimeDelta);

public:
	void	Set_Running(_bool bIsRunning) { m_bIsRunning = bIsRunning; }

	void			Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	CGameObject*	Get_Target() { return m_pTarget; }
private:
	CMainQuest*  m_pMainQuest = nullptr;
	CGameObject* m_pTarget = nullptr;
	
	_bool		m_bIsRunning = false;

public:
	virtual void Free() override;
};

END

