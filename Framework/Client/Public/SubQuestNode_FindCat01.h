#pragma once

#include "QuestNode_Base.h"

class CSubQuestNode_FindCat01 final : public CQuestNode_Base
{
private:
	CSubQuestNode_FindCat01();
	virtual ~CSubQuestNode_FindCat01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pChloe = nullptr;

public:
	static CSubQuestNode_FindCat01* Create();
	virtual void Free() override;
};

