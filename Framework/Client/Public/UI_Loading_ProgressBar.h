#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Loading_ProgressBar final : public CUI
{
public:
	enum UI_PROGRESSBAR { UIPROG_BACK, UIPROG_BAR, UIPROG_END };

protected:
	CUI_Loading_ProgressBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, UI_PROGRESSBAR eType);
	CUI_Loading_ProgressBar(const CUI_Loading_ProgressBar& rhs);
	virtual ~CUI_Loading_ProgressBar() = default;

public:
	_bool Is_LoadingDone();

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_PROGRESSBAR m_eType = { UIPROG_END };

	_float m_fWidth = { 0.f };
	_uint m_iPass = { 0 };

	_bool m_bFinish = { false };
	_float m_fLoadingProg = { 0.1f }; // 현재 로딩이 되고있는 퍼센테이지를 나타낸다.

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Loading_ProgressBar* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strObjectTag, UI_PROGRESSBAR eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
