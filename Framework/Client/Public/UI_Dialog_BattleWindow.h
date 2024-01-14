#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Dialog_BattleWindow final : public CUI
{
protected:
	CUI_Dialog_BattleWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Dialog_BattleWindow(const CUI_Dialog_BattleWindow& rhs);
	virtual ~CUI_Dialog_BattleWindow() = default;

public:
	void Set_Contents(const wstring& strContents);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	wstring m_strContents = TEXT("");
	_float2 m_vTextPosition = _float2(0.f, 0.f);
	_int m_iTextCount = { 0 };
	_int m_iMaxCount = { 0 };
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Replace_DialogPosition();
	void Tick_Text(_float fTimeDelta);

public:
	static CUI_Dialog_BattleWindow* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
