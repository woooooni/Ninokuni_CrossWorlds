#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_Glanix02 final : public CQuestNode_Base
{
private:
	CMainQuestNode_Glanix02();
	virtual ~CMainQuestNode_Glanix02() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	virtual void    TalkEvent();

private:
	CGameObject* m_pAren = nullptr;
	CGameObject* m_pKuu = nullptr;

public:
	static CMainQuestNode_Glanix02* Create();
	virtual void Free() override;
};

