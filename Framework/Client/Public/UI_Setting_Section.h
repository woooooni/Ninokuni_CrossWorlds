#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Setting_Section final : public CUI
{
public:
	enum UI_SETTING_SECTION { SECTION_GAME, SECTION_GRAPHIC, SECTION_AUDIO, SECTION_END };

protected:
	CUI_Setting_Section(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_SETTING_SECTION eType);
	CUI_Setting_Section(const CUI_Setting_Section& rhs);
	virtual ~CUI_Setting_Section() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_SETTING_SECTION m_eType = { SECTION_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Setting_Section* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, UI_SETTING_SECTION eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
