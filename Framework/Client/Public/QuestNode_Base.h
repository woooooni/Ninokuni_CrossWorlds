#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "Quest_DestSpot.h"

class CQuestNode_Base abstract : public CBTNode_Leaf
{
protected:
	typedef struct tagTalkDesc
	{
		wstring strOwner = TEXT("");
		wstring strTalk = TEXT("");
	}TALK_DELS;

protected:
	CQuestNode_Base();
	virtual ~CQuestNode_Base() = default;

public:
	virtual HRESULT Initialize();
	virtual void	Start();
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);
	virtual void	LateTick(const _float& fTimeDelta);

protected:
	vector<TALK_DELS> m_vecTalkDesc;
	_bool m_bIsClear = false;

	_int m_iTalkIndex = 0;

	CQuest_DestSpot* m_pQuestDestSpot = nullptr;

public:
	virtual void Free() override; 
};

