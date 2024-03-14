#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_WitchForest05 final : public CQuestNode_Base
{
private:
	CMainQuestNode_WitchForest05();
	virtual ~CMainQuestNode_WitchForest05() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pCyan = nullptr;

public:
	static CMainQuestNode_WitchForest05* Create();
	virtual void Free() override;
};

