#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_QuickSlot_Item final : public CUI
{
public:
	enum UI_QUICKSLOT_ITEM { QUICKITEM_FIRST, QUICKITEM_SECOND, QUICKITEM_THIRD, QUICKITEM_END };

protected:
	CUI_QuickSlot_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType);
	CUI_QuickSlot_Item(const CUI_QuickSlot_Item& rhs);
	virtual ~CUI_QuickSlot_Item() = default;

public:
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }

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
	UI_QUICKSLOT_ITEM m_eType = { QUICKITEM_END };
	ITEM_CODE m_eCode = { ITEM_CODE::CODE_END };
	class CTexture* m_pItemTextureCom = { nullptr };

	_uint m_iTextureIndex = { 0 };

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vHidePosition = _float2(0.f, 0.f);

	_bool m_bUsable = { false }; // 아이템이 차있으면 true, 아니면 false

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);
	void Movement_BasedOnHiding(_float fTimeDelta);
	void Use_Item();

public:
	static CUI_QuickSlot_Item* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_QUICKSLOT_ITEM eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
