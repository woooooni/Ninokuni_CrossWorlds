#pragma once

#include "UI.h"

BEGIN(Engine)
class Transform;
END

BEGIN(Client)
class CUI_Damage_General final : public CUI
{
public:
	enum UI_DMG_FONTTYPE { DMG_MISS, DMG_DODGE, DMGFONT_END };

public:
	typedef struct tagGeneralFont
	{
		_float2 vPosition = _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);

	}GENERAL_DESC;

public:
	CUI_Damage_General(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DMG_FONTTYPE eType);
	CUI_Damage_General(const CUI_Damage_General& rhs);
	virtual ~CUI_Damage_General() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UI_DMG_FONTTYPE m_eFontType = { DMGFONT_END };
	GENERAL_DESC m_FontDesc;

	class CTransform* m_pTargetTransform = { nullptr };
	_float2 m_vTargetPosition = _float2(0.f, 0.f);

	_bool m_bFadeOut = { false }; // FadeOut을 시작할 것인가?
	_float m_fFadeTimeAcc = { 0.f };

	_uint m_iPass = { 1 };

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Damage_General* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_DMG_FONTTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
