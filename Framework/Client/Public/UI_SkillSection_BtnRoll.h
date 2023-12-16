#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_BtnRoll final : public CUI
{
protected:
	CUI_SkillSection_BtnRoll(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_SkillSection_BtnRoll(const CUI_SkillSection_BtnRoll& rhs);
	virtual ~CUI_SkillSection_BtnRoll() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void On_MouseEnter(_float fTimeDelta) override;
	virtual void On_Mouse(_float fTimeDelta) override;
	virtual void On_MouseExit(_float fTimeDelta) override;

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_SkillSection_BtnRoll* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
