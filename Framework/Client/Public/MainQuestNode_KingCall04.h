#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_KingCall04 final : public CQuestNode_Base
{
private:
	CMainQuestNode_KingCall04();
	virtual ~CMainQuestNode_KingCall04() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pRuslan = nullptr;

public:
	static CMainQuestNode_KingCall04* Create();
	virtual void Free() override;
};

