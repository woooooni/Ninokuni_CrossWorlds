#pragma once

#include "Client_Defines.h"
#include "BTNode_Leaf.h"

#include "GameNpc.h"
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
	virtual Vec4    Set_DestSpot(CGameObject* pGameObject);
	virtual void    TalkEvent() {};

	virtual CQuest_DestSpot* Get_QuestDestSpot() { return m_pQuestDestSpot; }
	virtual void			 Delete_QuestDestSpot();

	const _bool		Is_Finish_LevelEnterCameraAction();

protected:
	vector<TALK_DELS> m_vecTalkDesc;
	_bool m_bIsClear = false;
	_bool m_bIsRewarding = false;

	_int m_iTalkIndex = 0;

	CQuest_DestSpot* m_pQuestDestSpot = nullptr;

	wstring m_strQuestTag = TEXT("");
	wstring m_strQuestName = TEXT("");
	wstring m_strQuestContent = TEXT("");

	wstring m_strNextQuestTag = TEXT("");
	wstring m_strNextQuestName = TEXT("");
	wstring m_strNextQuestContent = TEXT("");

	TCHAR* m_szpOwner = nullptr;
	TCHAR* m_szpTalk = nullptr;

	vector<CGameObject*> m_vecTalker;

public:
	virtual void Free() override; 
};

