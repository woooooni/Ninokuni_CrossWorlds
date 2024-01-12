#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_SnowField03 final : public CQuestNode_Base
{
private:
	CMainQuestNode_SnowField03();
	virtual ~CMainQuestNode_SnowField03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pJackson = nullptr;

public:
	static CMainQuestNode_SnowField03* Create();
	virtual void Free() override;
};

