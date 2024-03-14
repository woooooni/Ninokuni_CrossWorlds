#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_KingCall05 final : public CQuestNode_Base
{
private:
	CMainQuestNode_KingCall05();
	virtual ~CMainQuestNode_KingCall05() = default;

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
	static CMainQuestNode_KingCall05* Create();
	virtual void Free() override;
};

