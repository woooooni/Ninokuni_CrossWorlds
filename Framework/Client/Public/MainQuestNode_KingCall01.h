#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_KingCall01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_KingCall01();
	virtual ~CMainQuestNode_KingCall01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pSanson = nullptr;

public:
	static CMainQuestNode_KingCall01* Create();
	virtual void Free() override;
};

