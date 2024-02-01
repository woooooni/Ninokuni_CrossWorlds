#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller08 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller08();
	virtual ~CMainQuestNode_PlantKiller08() = default;

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
	static CMainQuestNode_PlantKiller08* Create();
	virtual void Free() override;
};

