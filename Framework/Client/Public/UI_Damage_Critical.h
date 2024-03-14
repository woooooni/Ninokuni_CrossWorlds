#pragma once

#include "UI.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_Damage_Critical final : public CUI
{
public:
	enum UI_CRITICALFONT { CRITICAL_BLUE, CRITICAL_YELLOW, CRITICAL_RED, CRITICAL_REDBLUR, CRITICALFONT_END };

public:
	typedef struct tagCriticalFont
	{
		class CTransform* pTransform = { nullptr };
		_float2 vOffset = _float2(0.f, 0.f);
		UI_CRITICALFONT eType = { CRITICAL_RED };

	}CRITICAL_DESC;

public:
	CUI_Damage_Critical(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Damage_Critical(const CUI_Damage_Critical& rhs);
	virtual ~CUI_Damage_Critical() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_CRITICALFONT m_eFontType = { CRITICAL_REDBLUR };
	CRITICAL_DESC m_FontDesc;

	class CTransform* m_pTargetTransform = { nullptr };
	_float2 m_vTargetPosition = _float2(0.f, 0.f);
	_float2 m_vOffset = _float2(0.f, 0.f);

	_bool m_bFadeOut = { false }; // FadeOut을 시작할 것인가?
	_float m_fFadeTimeAcc = { 0.f };
	_float m_fArrivedPosY = { 0.f };

	_bool m_bResize = { false };
	_float2 m_fMaxScale = _float2(0.f, 0.f);
	_float2 m_fOriginScale = _float2(0.f, 0.f);

	_uint m_iPass = { 1 };

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Update_Position();

public:
	static CUI_Damage_Critical* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
