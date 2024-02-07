#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_MapName_Text final : public CUI
{
public:
	enum UI_MAPNAME_TEXT { EVERMORE_SOUTH, EVERMORE_EARTH, EVERMORE_WEST, KINGDOM,
		ICELAND_DEFAULT, ICELAND_MEETINGTREE, ICELAND_SAMURAVILLAGE, ICELAND_SHELTER,
		ICELAND_ICEVILLAGE, ICELAND_STARTREE, ICELAND_GHOSTFOREST, ICELAND_SHADOW,
		/* ¸¶³àÀÇ ½£ Ãß°¡ ÇÊ¿äÇÔ */
		WITCH_ENTRANCE, WITCH_SHELTER, WITCH_MOON, WITCH_BANDI,
		// ½£ÀÇ ÀÔ±¸, Á¤·ÉÀÇ ½°ÅÍ, ÃÊ½Â´Þ Æò¿ø ºÏºÎ, ¹Ýµð Á¤¿ø
		MAPNAMETEXT_END };

protected:
	CUI_MapName_Text(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MapName_Text(const CUI_MapName_Text& rhs);
	virtual ~CUI_MapName_Text() = default;

public:
	void Set_Type(UI_MAPNAME_TEXT eType) { m_eType = eType; }
	void Set_Alpha(_float fAlpha) { m_fAlpha = fAlpha; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_MAPNAME_TEXT m_eType = { UI_MAPNAME_TEXT::EVERMORE_SOUTH };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_MapName_Text* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
