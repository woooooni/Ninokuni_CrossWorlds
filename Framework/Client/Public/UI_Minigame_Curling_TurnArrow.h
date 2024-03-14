#pragma once

#include "UI_Minigame_Curling_Base.h"

BEGIN(Client)
class CUI_Minigame_Curling_TurnArrow final : public CUI_Minigame_Curling_Base
{
private:
	CUI_Minigame_Curling_TurnArrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	CUI_Minigame_Curling_TurnArrow(const CUI_Minigame_Curling_TurnArrow& rhs);
	virtual ~CUI_Minigame_Curling_TurnArrow() = default;

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

public:
	static CUI_Minigame_Curling_TurnArrow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
