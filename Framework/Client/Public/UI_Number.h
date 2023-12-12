#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Number final : public CUI
{
public:
	typedef struct tagUI_NumInfo : public CUI::UI_INFO
	{
		_int iNumber = { -1 };

	} UI_NUMBERINFO;

protected:
	CUI_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Number(const CUI_Number& rhs);
	virtual ~CUI_Number() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	void Set_Number(_uint iNumber);
	void Resize_Scale();

private:
	_uint m_iNumber = { 0 };
	string m_sNumber = "";

	_bool m_bResize = { false };
	_float m_fMaxScale = { 0.f };
	_float m_fOriginScale = { 0.f };

private:
	HRESULT	Ready_Components();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Number* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
