#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_SnowField04 final : public CQuestNode_Base
{
private:
	CMainQuestNode_SnowField04();
	virtual ~CMainQuestNode_SnowField04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pJackson = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CMainQuestNode_SnowField04* Create();
	virtual void Free() override;
};

