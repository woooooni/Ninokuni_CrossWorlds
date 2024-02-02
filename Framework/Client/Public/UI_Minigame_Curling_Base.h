#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_Curling_Base abstract : public CUI
{
protected:
	CUI_Minigame_Curling_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	CUI_Minigame_Curling_Base(const CUI_Minigame_Curling_Base& rhs);
	virtual ~CUI_Minigame_Curling_Base() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual HRESULT Send_Message()									{ return S_OK; }
	virtual HRESULT Send_Message_Int(const _int& wstrValue)			{ return S_OK; }
	virtual HRESULT Send_Message_Float(const _float& fValue)		{ return S_OK; }
	virtual HRESULT Send_Message_Wstring(const wstring& wstrValue)	{ return S_OK; }
	virtual HRESULT Send_Message_Vec3(const Vec3& vValue)			{ return S_OK; }
	virtual HRESULT Send_Message_Bool(const _bool& bValue)			{ return S_OK; }

protected:
	virtual HRESULT	Ready_Components() override;
	virtual HRESULT	Ready_Default();
	HRESULT			Bind_ShaderResources();

protected:
	_uint	m_iPass = { 1 };
	Vec4	m_vColor = DirectX::Colors::Red;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};
END
