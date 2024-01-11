#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_Windmill08 final : public CQuestNode_Base
{
private:
	CSubQuestNode_Windmill08();
	virtual ~CSubQuestNode_Windmill08() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pVerde = nullptr;

public:
	static CSubQuestNode_Windmill08* Create();
	virtual void Free() override;
};

