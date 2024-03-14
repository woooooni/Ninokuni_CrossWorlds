#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Minigame_Timer final : public CUI
{
protected:
	CUI_Minigame_Timer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Minigame_Timer(const CUI_Minigame_Timer& rhs);
	virtual ~CUI_Minigame_Timer() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Minigame_Timer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
