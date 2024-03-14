#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Glanix10 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix10();
	virtual ~CMainQuestNode_Glanix10() = default;

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
	static CMainQuestNode_Glanix10* Create();
	virtual void Free() override;
};

