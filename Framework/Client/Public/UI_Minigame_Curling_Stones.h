#pragma once

#include "UI_Minigame_Curling_Base.h"

BEGIN(Client)
class CUI_Minigame_Curling_Stones final : public CUI_Minigame_Curling_Base
{
private:
	CUI_Minigame_Curling_Stones(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType);
	CUI_Minigame_Curling_Stones(const CUI_Minigame_Curling_Stones& rhs);
	virtual ~CUI_Minigame_Curling_Stones() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual HRESULT Send_Message_Int(const _int& iValue);

private:
	virtual HRESULT	Ready_Components() override;
	virtual HRESULT	Ready_Default() override;
	HRESULT			Bind_ShaderResources();

private:
	_bool			m_bPlayerType = false;
	_uint			m_iCurStones = 0;

public:
	static CUI_Minigame_Curling_Stones* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
