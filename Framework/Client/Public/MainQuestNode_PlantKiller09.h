#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller09 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller09();
	virtual ~CMainQuestNode_PlantKiller09() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pCyan = nullptr;

public:
	static CMainQuestNode_PlantKiller09* Create();
	virtual void Free() override;
};

