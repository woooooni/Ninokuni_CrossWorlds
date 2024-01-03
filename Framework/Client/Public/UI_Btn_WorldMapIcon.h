#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Btn_WorldMapIcon final : public CUI
{
public:
	enum UI_WORLDMAP_ICONTYPE{ ICON_EVERMORE, ICON_ICELAND, ICON_WITCHFOREST, WORLDMAPICON_END};

protected:
	CUI_Btn_WorldMapIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		const wstring& strObjectTag, UI_WORLDMAP_ICONTYPE eType);
	CUI_Btn_WorldMapIcon(const CUI_Btn_WorldMapIcon& rhs);
	virtual ~CUI_Btn_WorldMapIcon() = default;

public:
	virtual void Set_Active(_bool bActive) override;

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
	UI_WORLDMAP_ICONTYPE m_eIconType = { WORLDMAPICON_END };
	class CUI_Basic* m_pIcon = { nullptr };
	class CUI_WindowWorldMap* m_pWorldMap = { nullptr };
	_int m_iTextureIndex = { -1 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_Btn_WorldMapIcon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		const wstring& strObjectTag, UI_WORLDMAP_ICONTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
