#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Boss_NameTag final : public CUI
{
protected:
	CUI_Boss_NameTag(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Boss_NameTag(const CUI_Boss_NameTag& rhs);
	virtual ~CUI_Boss_NameTag() = default;

public:
	virtual void Set_Active(_bool bActive) override;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	_float m_fTimeAcc = { 0.f };
	_int m_iTextureIndex = { 0 };
	_bool m_bDone = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Boss_NameTag* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
