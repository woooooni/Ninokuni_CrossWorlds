#pragma once

#include "QuestNode_Base.h"

class CMainQuestNode_FinalBattle06 final : public CQuestNode_Base
{
private:
	CMainQuestNode_FinalBattle06();
	virtual ~CMainQuestNode_FinalBattle06() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

private:
	CGameObject* m_pWitch = nullptr;
	_bool m_bIsFadeOut = false;

	_bool m_bCamSignal = false;
public:
	static CMainQuestNode_FinalBattle06* Create();
	virtual void Free() override;
};

