#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Loading_Character final : public CUI
{
protected:
	CUI_Loading_Character(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading_Character(const CUI_Loading_Character& rhs);
	virtual ~CUI_Loading_Character() = default;

public:
	void Set_TextureIndex(_int iIndex) { m_iTextureIndex = iIndex; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_uint m_iTextureIndex = { 0 };

	_bool m_bArrived = { false };
	_bool m_bAlpha = { false };

	_float m_fSpeed = { 800.f };
	_float m_fTimeAcc = { 0.f };
	_float2 m_vStartPos = _float2(0.f, 0.f);
	_float2 m_vArrivedPos = _float2(0.f, 0.f);

	_bool m_bHover = { false };
	_bool m_bTurn = { false };
	_float2 m_vHoverPos = _float2(0.f, 0.f); // 도착하고나서 맴도는 Position
	_float2 m_vTurnPos = _float2(0.f, 0.f); // 다음으로 돌아가는 Position

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Move_ToHoverPosition(_float fTimeDelta);

public:
	static CUI_Loading_Character* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
