#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Emoticon_Button final : public CUI
{
public:
	enum UI_EMOTICON_BTN { EMOTIONBTN_FIRST, EMOTIONBTN_SECOND, EMOTIONBTN_THIRD, EMOTIONBTN_FOURTH,
		EMOTIONBTN_FIFTH, EMOTIONBTN_SIXTH, EMOTIONBTN_SEVENTH, EMOTIONBTN_EIGHTH, EMOTICONBTN_END };

protected:
	CUI_Emoticon_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_EMOTICON_BTN eType);
	CUI_Emoticon_Button(const CUI_Emoticon_Button& rhs);
	virtual ~CUI_Emoticon_Button() = default;

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
	UI_EMOTICON_BTN m_eType = { EMOTICONBTN_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Emoticon_Button* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_EMOTICON_BTN eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
