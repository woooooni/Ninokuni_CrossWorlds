#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Invasion01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Invasion01();
	virtual ~CMainQuestNode_Invasion01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pRuslan = nullptr;
	CGameObject* m_pKuu = nullptr;

	_bool		m_bIsStart = false;

	_bool		m_bIsShake = false;

public:
	static CMainQuestNode_Invasion01* Create();
	virtual void Free() override;
};

