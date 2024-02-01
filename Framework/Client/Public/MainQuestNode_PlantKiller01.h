#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_PlantKiller01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_PlantKiller01();
	virtual ~CMainQuestNode_PlantKiller01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pCyan = nullptr;

public:
	static CMainQuestNode_PlantKiller01* Create();
	virtual void Free() override;
};

