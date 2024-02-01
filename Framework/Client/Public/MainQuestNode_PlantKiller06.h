#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller06 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller06();
	virtual ~CMainQuestNode_PlantKiller06() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pRuby = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CMainQuestNode_PlantKiller06* Create();
	virtual void Free() override;
};

