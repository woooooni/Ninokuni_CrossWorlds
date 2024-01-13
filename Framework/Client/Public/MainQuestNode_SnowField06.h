#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_SnowField06 final : public CQuestNode_Base
{
private:
	CMainQuestNode_SnowField06();
	virtual ~CMainQuestNode_SnowField06() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pJackson = nullptr;

public:
	static CMainQuestNode_SnowField06* Create();
	virtual void Free() override;
};

