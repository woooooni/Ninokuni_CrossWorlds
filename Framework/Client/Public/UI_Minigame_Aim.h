#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_Aim final : public CUI
{
protected:
	CUI_Minigame_Aim(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_Aim(const CUI_Minigame_Aim& rhs);
	virtual ~CUI_Minigame_Aim() = default;

public:
	void Set_Owner(class CVehicle_Flying* pOwner);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CVehicle_Flying* m_pOwner = { nullptr };

	_bool m_bRotate = { false };
	_float m_fTimeAcc = { 0.f };
	_float2 m_vOffset = _float2(0.f, 0.f);
	_float2 m_vOriginOffset = _float2(0.f, 0.f);

	_bool m_bResize = { false };
	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);

	_bool m_bIsTarget = { false };
	_float m_fDistance = { 0.f };
	_uint m_iTextureIndex = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Update_Distance();

public:
	static CUI_Minigame_Aim* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
