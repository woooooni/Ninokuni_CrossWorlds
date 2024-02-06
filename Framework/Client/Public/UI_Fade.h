#pragma once

#include "UI.h"

BEGIN(Engine)
class CTexture;
END

BEGIN(Client)
class CUI_Fade final : public CUI
{
protected:
	CUI_Fade(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Fade(const CUI_Fade& rhs);
	virtual ~CUI_Fade() = default;

public:
	void	Set_DefaultSetting();
	void	Set_Fade(const _bool& pIsFadeOut, const _float& pFadeTime, const _bool& bIsWhite = false);
	const	_bool&		Get_Fade() { return m_bIsFadeOut; }
	void	Set_White(const _bool& pIsWhite)
	{
		m_bIsWhite = pIsWhite;
	}
	void	Set_Finish();

	const	_bool& Get_Finish() { return m_bIsComplete; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_bool m_bIsFadeOut = { false }; // false = FadeOut, true = FadeIn
	_bool m_bIsWhite = { true }; // White Type Veil을 사용하는가
	_bool m_bIsComplete = { true }; // FadeIn or FadeOut이 완료되었는가

	_float m_fAlpha = { 0.f }; // Veil에 지정할 알파값
	_float m_fFadeTime = { 0.f }; // FadeIn or FadeOut하는 총 시간

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Fade* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
