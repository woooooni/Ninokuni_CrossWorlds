#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_Basic final : public CUI
{
public:
	enum UI_MINIGAMEBASIC {	TOWERDEFENCE_MENU, TOWERDEFENCE_START, TOWERDEFENCE_GOLD,
		GRANDPRIX_FRAME, GRANDPRIX_CLASSFRAME, GRANDPRIX_SPECIALFRAME,
		GRANDPRIX_READY, GRANDPRIX_THREE, GRANDPRIX_TWO, GRANDPRIX_ONE, GRANDPRIX_START, GRANDPRIX_END, 
		GRANDPRIX_BIPLANE, GRANDPRIX_GAUGEBACK, GRANDPRIX_GAUGEGLOW, GRANDPRIX_SPACE, GRANDPRIX_ERROR, MINIGAME_END };

protected:
	CUI_Minigame_Basic(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_MINIGAMEBASIC eType);
	CUI_Minigame_Basic(const CUI_Minigame_Basic& rhs);
	virtual ~CUI_Minigame_Basic() = default;

public:
	virtual void Set_Active(_bool bActive) override;

	_bool Is_End() {
		if (!(m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END))
			return false;
		return m_bEnd; }

	void Set_Start(_bool bStart) {
		if (!(m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END))
			return;
		m_bStart = bStart;
	}

	_bool Is_Started() { 
		if (!(m_eType == GRANDPRIX_READY || m_eType == GRANDPRIX_THREE || m_eType == GRANDPRIX_TWO ||
			m_eType == GRANDPRIX_ONE || m_eType == GRANDPRIX_START || m_eType == GRANDPRIX_END))
			return false; 
		return m_bStart; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();
	virtual HRESULT Render_Minimap() override;

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;
	
private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	HRESULT Ready_TextInformation();

private:
	void Tick_Count(_float fTimeDelta);
	void LateTick_Count(_float fTimeDelta);

private:
	UI_MINIGAMEBASIC m_eType = { MINIGAME_END };
	_bool m_bUseIndex = { false };
	_uint m_iTextureIndex = { 0 };

	// Grandprix Count용 변수
	_bool m_bStart = { false }; // 사이즈 변형이 시작되었는지
	_bool m_bEnd = { false }; // 사이즈 변형이 끝났는지 -> 끝나면 매니저에서 Set_Active(false)를 설정하고 다음 인덱스로 넘기도록 한다.
	_float2 m_vMaxSize = _float2(0.f, 0.f); // 최대 사이즈
	_float2 m_vOriginSize = _float2(0.f, 0.f); // 최소 사이즈

	// Grandprix Gauge용 변수
	_bool m_bResize = { false };

	_float m_fTimeAcc = { 0.f };

public:
	static CUI_Minigame_Basic* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_MINIGAMEBASIC eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
