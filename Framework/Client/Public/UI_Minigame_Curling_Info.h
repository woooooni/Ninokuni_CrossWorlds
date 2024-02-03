#pragma once

#include "UI_Minigame_Curling_Base.h"

BEGIN(Client)
class CUI_Minigame_Curling_Info final : public CUI_Minigame_Curling_Base
{
	enum class TEX_TYPE { FRAME, FACE, TYPEEND };

private:
	CUI_Minigame_Curling_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType);
	CUI_Minigame_Curling_Info(const CUI_Minigame_Curling_Info& rhs);
	virtual ~CUI_Minigame_Curling_Info() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	virtual HRESULT	Ready_Components() override;
	virtual HRESULT	Ready_Default() override;

	HRESULT			Bind_ShaderResources();

private:
	CTexture*		m_pTextureComs[(_uint)TEX_TYPE::TYPEEND] = { nullptr };

private:
	_bool			m_bPlayerType = false;
	CUI::UI_INFO	m_tUiDescs[(_uint)TEX_TYPE::TYPEEND];

public:
	static CUI_Minigame_Curling_Info* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
