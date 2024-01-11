#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Windmill03 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Windmill03();
	virtual ~CSubQuestNode_Windmill03() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pBeard = nullptr;

public:
	static CSubQuestNode_Windmill03* Create();
	virtual void Free() override;
};

