#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller03 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller03();
	virtual ~CMainQuestNode_PlantKiller03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pRuby = nullptr;

public:
	static CMainQuestNode_PlantKiller03* Create();
	virtual void Free() override;
};

