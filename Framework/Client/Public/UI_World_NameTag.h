#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_World_NameTag final : public CUI
{
public:
	enum UI_NAMETAG { NAMETAG_LOBBY, NAMETAG_GAMEPLAY, NAMETAG_END };

protected:
	CUI_World_NameTag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_NAMETAG eType);
	CUI_World_NameTag(const CUI_World_NameTag& rhs);
	virtual ~CUI_World_NameTag() = default;

public:
	void Set_Owner(class CGameObject* pOwner);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CCharacter* m_pOwner = { nullptr };
	CTexture* m_pFXTextureCom = { nullptr };
	UI_NAMETAG m_eType = { NAMETAG_END };
	_float2 m_vTextPos = _float2(0.f, 0.f);

	_bool m_bRotate = { false };
	_float m_fTimeAcc = { 0.f };
	_float m_fOffsetY = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_World_NameTag* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_NAMETAG eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
