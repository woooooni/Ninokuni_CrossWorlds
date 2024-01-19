#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_InGame_Setting_RadioGroup final : public CUI
{
public:
	enum UI_RADIOGROUP
	{ GRAPHIC_NATURAL, GRAPHIC_SHADOW, GRAPHIC_OUTLINE, GRAPHIC_BLOOM, GRAPHIC_BLUR, GRAPHIC_SSAO, GRAPHIC_PBR, RADIOGROUP_END };

protected:
	CUI_InGame_Setting_RadioGroup(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_RADIOGROUP eType);
	CUI_InGame_Setting_RadioGroup(const CUI_InGame_Setting_RadioGroup& rhs);
	virtual ~CUI_InGame_Setting_RadioGroup() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_RADIOGROUP m_eGroupType = { RADIOGROUP_END };

	class CUI_InGame_Setting_RadioBtn* m_pOnBtn = { nullptr };
	class CUI_InGame_Setting_RadioBtn* m_pOffBtn = { nullptr };

private:
	virtual HRESULT	Ready_Components() override;
	void Ready_RadioButtons();

public:
	static CUI_InGame_Setting_RadioGroup* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_RADIOGROUP eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
