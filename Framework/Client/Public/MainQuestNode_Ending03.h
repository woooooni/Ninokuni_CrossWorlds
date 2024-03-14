#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Ending03 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Ending03();
	virtual ~CMainQuestNode_Ending03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pRuslan = nullptr;

public:
	static CMainQuestNode_Ending03* Create();
	virtual void Free() override;
};

