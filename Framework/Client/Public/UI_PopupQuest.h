#pragma once

#include "UI.h"

// Btn QuickQuest를 누르면 나오는 작은 퀘스트 팝업창(실제게임상 게임화면 왼쪽)

BEGIN(Client)
class CUI_PopupQuest final : public CUI
{
protected:
	CUI_PopupQuest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_PopupQuest(const CUI_PopupQuest& rhs);
	virtual ~CUI_PopupQuest() = default;

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
	_bool m_bHide = { false };

	_bool m_bUpdate = { false };
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_PopupQuest* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
