#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Loading_MainLogo final : public CUI
{
public:
	enum UI_MAINLOGOTYPE { MAINLOGO_TEXT, MAINLOGO_NUM, MAINLOGO_END };

protected:
	CUI_Loading_MainLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MAINLOGOTYPE eType);
	CUI_Loading_MainLogo(const CUI_Loading_MainLogo& rhs);
	virtual ~CUI_Loading_MainLogo() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

private:
	UI_MAINLOGOTYPE m_eType = { MAINLOGO_END };

	_float m_fTimeAcc = { 0.f };
	_int m_iTextureIndex = { -1 };

	vector<_bool> m_bFinish;

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMaxSize = _float2(0.f, 0.f);
	_float2 m_vSizeAcc = _float2(0.f, 0.f);

	_float m_fRestartTimeAcc = { 0.f };
	_bool m_bRestart = { true };

	_bool m_bResize = { false };


public:
	static CUI_Loading_MainLogo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MAINLOGOTYPE eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END