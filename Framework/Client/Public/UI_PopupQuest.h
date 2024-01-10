#pragma once

#include "UI.h"

// Btn QuickQuest를 누르면 나오는 작은 퀘스트 팝업창(실제게임상 게임화면 왼쪽)

BEGIN(Client)
class CUI_PopupQuest final : public CUI
{
public:
	enum UI_QUESTPOPUP { POPUPFRAME_TOP, POPUPFRAME_BOTTOM, POPUPWINDOW, QUESTPOPUP_END };

protected:
	CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUESTPOPUP eType);
	CUI_PopupQuest(const CUI_PopupQuest& rhs);
	virtual ~CUI_PopupQuest() = default;

public:
	virtual void Set_Active(_bool bActive);
	void Set_Contents(const wstring& strQuestType, const wstring& strTitle, const wstring& strContents);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

private:
	UI_QUESTPOPUP m_eType = { QUESTPOPUP_END };
	_float m_fAppearProg = { 0.f };
	_bool m_bProgressing = { false };

	wstring m_strType;
	wstring m_strTitle;
	wstring m_strContents;

	_float4 m_vTypeColor = _float4(0.f, 0.f, 0.f, 1.f);
	_float4 m_vTextColor = _float4(0.f, 0.f, 0.f, 1.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_PopupQuest* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_QUESTPOPUP eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
