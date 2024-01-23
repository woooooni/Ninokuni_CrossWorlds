#pragma once

#include "UI.h"

// Btn QuickQuest를 누르면 나오는 작은 퀘스트 팝업창(실제게임상 게임화면 왼쪽)

BEGIN(Client)
class CUI_PopupQuest final : public CUI
{
public:
	enum UI_QUESTPOPUP { POPUPFRAME_TOP, POPUPFRAME_BOTTOM, POPUP_SEPARATOR, POPUP_WINDOW, QUESTPOPUP_END };
	enum UI_POPUP_SEPARATOR { SEPARATOR_FIRST, SEPARATOR_SECOND, SEPARATOR_THIRD, SEPARATOR_END };

public:
	typedef struct tagQuestInformation
	{
		wstring strType;
		wstring strTitle;
		wstring strContents;

		_bool bCreateSpot = false;
		_float4 vDestPosition = _float4(0.f, 0.f, 0.f, 0.f);

	} QUEST_INFO;

protected:
	CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType, UI_POPUP_SEPARATOR eSeparatorType = SEPARATOR_END);
	CUI_PopupQuest(const CUI_PopupQuest& rhs);
	virtual ~CUI_PopupQuest() = default;

public:
	virtual void Set_Active(_bool bActive);
	void Set_Contents(const wstring& strQuestType, const wstring& strTitle, const wstring& strContents); // 병합시 오류 방지. 임시로 살려둠.
	void Set_Contents(const QUEST_INFO& tQuestInfo);
	void Update_QuestContents(const wstring& strPreTitle, const wstring& strQuestType, const wstring& strTitle, const wstring& strContents); // 병합시 오류 방지. 임시로 살려둠.
	void Update_QuestContents(const wstring& strPreTitle, const QUEST_INFO& tQuestInfo);
	void Clear_Quest(const wstring& strTitle);

	_int Get_NumOfQuest();

//	const QUEST_INFO& Get_QuestContents(_int iSlotNum);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;
	void Move_BottomFrame(_int iNumOfQuest);

private:
	UI_QUESTPOPUP		m_eType = { QUESTPOPUP_END };
	UI_POPUP_SEPARATOR	m_eSeparator = { SEPARATOR_END };
	_float				m_fAppearProg = { 0.f };
	_bool				m_bProgressing = { false };

	_float4 m_vTypeColor = _float4(0.f, 0.f, 0.f, 1.f);
	_float4 m_vTextColor = _float4(0.f, 0.f, 0.f, 1.f);

	vector<QUEST_INFO> m_Quest;

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_PopupQuest* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_QUESTPOPUP eType, UI_POPUP_SEPARATOR eSeparatorType = SEPARATOR_END);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
