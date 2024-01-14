#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "MainQuest.h"
#include "SubQuest.h"

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
	void	Set_SubQuestRunning(CSubQuest::SUBQUEST_NAME eSQName, _bool bIsRunning);

	void	Set_Running(_bool bIsRunning) { m_bIsRunning = bIsRunning; }
	_bool	Get_IsReserve() { return m_bIsReserve; }

	void			Set_Target(CGameObject* pTarget) { m_pTarget = pTarget; }
	CGameObject*	Get_Target() { return m_pTarget; }

	// 퀘스트 완료 스택.
	_int	Get_QuestClearStack() { return m_iQuestClearStack; }
	void    Set_QuestClearStack(_int iStack) { m_iQuestClearStack += iStack; }
	void    Clear_QuestClearStack() { m_iQuestClearStack = 0; }

private:
	CMainQuest*  m_pMainQuest = nullptr;
	CSubQuest*   m_pSubQuest = nullptr;

	CGameObject* m_pTarget = nullptr;
	
	_bool		m_bIsRunning = false;
	_bool		m_bIsReserve = false;

	_int		m_iQuestClearStack = 0;

private:
	// Quest Object
	

public:
	virtual void Free() override;
};

END

