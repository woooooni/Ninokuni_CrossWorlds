#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_LevelUp final : public CUI
{
public:
	enum UILEVELUP_TYPE { UILEVELUP_FRAME, UILEVELUP_BG, UILEVELUP_END };

protected:
	CUI_LevelUp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UILEVELUP_TYPE eType);
	CUI_LevelUp(const CUI_LevelUp& rhs);
	virtual ~CUI_LevelUp() = default;

public:
	virtual void Set_Active(_bool bActive);
	void SetUp_Level(_uint iLevel);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	UILEVELUP_TYPE m_eType = { UILEVELUP_END };

	_bool m_bSetAlpha = { false };
	_bool m_bUpdate = { false };
	_bool m_bHideText = { false };

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float m_fMaxWidth = { 0.f };

	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_LevelUp* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UILEVELUP_TYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
