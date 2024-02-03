#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Ending05 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Ending05();
	virtual ~CMainQuestNode_Ending05() = default;

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

public:
	static CMainQuestNode_Ending05* Create();
	virtual void Free() override;
};

