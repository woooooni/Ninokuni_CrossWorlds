#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Loading_CharacterLogo final : public CUI
{
protected:
	CUI_Loading_CharacterLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Loading_CharacterLogo(const CUI_Loading_CharacterLogo& rhs);
	virtual ~CUI_Loading_CharacterLogo() = default;

public:
	void	Set_TextureIndex(_int iIndex) { m_iTextureIndex = iIndex; }
	void	Set_Text();

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Ready_Components() override;
	HRESULT			Ready_State();
	HRESULT			Bind_ShaderResources();

private:
	_bool	m_bReverse = { true };
	_int	m_iTextureIndex = { -1 };
	_float	m_fTimeAcc = { 0.f };

	_float2 m_vPosition = _float2(120.f, 230.f);
	_float4 m_vColor = _float4(0.f, 0.f, 0.f, 1.f);
	_float2 m_vScale = _float2(0.65f, 0.65f);
	_tchar	m_szInfoText[MAX_PATH];
	_uint	m_iMaxCount = { 0 };
	_int	m_iTextCount = { 0 };

private:
	void	Tick_Text(_float fTimeDelta);

public:
	static CUI_Loading_CharacterLogo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END