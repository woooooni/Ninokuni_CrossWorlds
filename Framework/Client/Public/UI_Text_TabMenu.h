#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Text_TabMenu final : public CUI
{
public:
	enum UI_MENUTITLE { TITLE_COSTUME, TITLE_INVEN, TITLE_SHOP, TITLE_SKILL, TITLE_VEHICLE, TITLE_WORLDMAP, UITITLE_END };
	// 코스튬, 가방, 상점, 클래스 스킬, 탈 것, 월드맵

protected:
	CUI_Text_TabMenu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Text_TabMenu(const CUI_Text_TabMenu& rhs);
	virtual ~CUI_Text_TabMenu() = default;

public:
	void Set_TextType(UI_MENUTITLE eType);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_MENUTITLE m_eType = { UITITLE_END };
	_bool m_bShowChild = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Text_TabMenu* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
