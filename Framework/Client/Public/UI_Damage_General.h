#pragma once

#include "UI.h"

BEGIN(Engine)
class Transform;
END

BEGIN(Client)
class CUI_Damage_General final : public CUI
{
public:
	enum UI_DMG_FONTTYPE { DMG_GENERALATTACK, DMG_MISS, DMGFONT_END };

public:
	typedef struct tagGeneralFont
	{
		CTransform* pTargetTransform = nullptr;
		_uint iDamage = 0;
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

	_uint m_iTextNum = { 0 }; // 숫자 한자리
	_int m_iDamage = { 0 }; // Setting된 Damage값을 저장
	_float2 m_vTargetPosition = _float2(0.f, 0.f);

	_bool m_bFadeOut = { false }; // FadeOut을 시작할 것인가?
	_float m_fFadeTimeAcc = { 0.f };

	_bool m_bSetPosition = { false };
	_float m_fOffsetX = { 0.f };
	_float2 m_fRandomOffset;

	_uint m_iPass = { 1 };

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

	virtual HRESULT	Render_Miss();
	virtual HRESULT Render_GeneralDamage();

public:
	static CUI_Damage_General* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_DMG_FONTTYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
