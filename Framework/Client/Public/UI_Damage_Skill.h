#pragma once

#include "UI.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_Damage_Skill final : public CUI
{
public:
	enum UI_DAMAGEFONT { BLUE, GOLD_WITHRED, GREEN, PURPLE, RED, WHITE, WHITEGOLD, GOLD, DAMAGEFOND_END };

public:
	typedef struct tagDamageFont
	{
		CTransform* pTargetTransform = nullptr;
		_uint iDamage = 0;
		_float2 vTargetPosition = _float2(g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);

	}DAMAGE_DESC;

public:
	CUI_Damage_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_DAMAGEFONT eType);
	CUI_Damage_Skill(const CUI_Damage_Skill& rhs);
	virtual ~CUI_Damage_Skill() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Resize_Scale();

private:
	UI_DAMAGEFONT m_eFontType = { DAMAGEFOND_END };
	DAMAGE_DESC m_FontDesc;

	class CTransform* m_pTargetTransform = { nullptr };

	_uint m_iTextNum = { 0 }; // 숫자 한자리
	_uint m_iDamage = { 0 }; // Setting된 Damage값을 저장
	_float2 m_vTargetPosition = _float2(0.f, 0.f);

	_bool m_bFadeOut = { false }; // FadeOut을 시작할 것인가?
	_float m_fFadeTimeAcc = { 0.f };
	_float m_fArrivedPosY = { 0.f };

	_float m_fOffsetX = { 0.f };
	_float2 m_fRandomOffset;

	_bool m_bResize = { false };
	_float m_fMaxScale = { 0.f };
	_float m_fOriginScale = { 0.f };

	_uint m_iPass = { 1 };

	_bool m_bSetPosition = { false };

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Damage_Skill* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_DAMAGEFONT eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
