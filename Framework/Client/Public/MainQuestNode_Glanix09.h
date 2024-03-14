#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Glanix09 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix09();
	virtual ~CMainQuestNode_Glanix09() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pRuslan = nullptr;

public:
	static CMainQuestNode_Glanix09* Create();
	virtual void Free() override;
};

