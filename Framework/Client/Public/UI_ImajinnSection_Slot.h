#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_ImajinnSection_Slot final : public CUI
{
public:
	enum UI_IMAJINNSLOT { IMAJINNSLOT_FIRST, IMAJINNSLOT_SECOND, IMAJINNSLOT_THIRD, IMAJINNSLOT_END };

protected:
	CUI_ImajinnSection_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_IMAJINNSLOT eSlotType);
	CUI_ImajinnSection_Slot(const CUI_ImajinnSection_Slot& rhs);
	virtual ~CUI_ImajinnSection_Slot() = default;

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
	UI_IMAJINNSLOT m_eSlotType = { IMAJINNSLOT_END };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_ImajinnSection_Slot* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_IMAJINNSLOT eSlotType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
