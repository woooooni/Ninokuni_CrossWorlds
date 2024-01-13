#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Glanix01 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix01();
	virtual ~CMainQuestNode_Glanix01() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pAren = nullptr;

public:
	static CMainQuestNode_Glanix01* Create();
	virtual void Free() override;
};

