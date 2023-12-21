#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_ClassicSkill final : public CUI
{
public:
	enum UI_CLASSICSKILL { SKILL_FIRST, SKILL_SECOND, SKILL_THIRD, UICLASSICSKILL_END };

protected:
	CUI_SkillSection_ClassicSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CLASSICSKILL eType);
	CUI_SkillSection_ClassicSkill(const CUI_SkillSection_ClassicSkill& rhs);
	virtual ~CUI_SkillSection_ClassicSkill() = default;

public:
	void Set_SkillType(CHARACTER_TYPE eType);

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
	UI_CLASSICSKILL m_eType = { UICLASSICSKILL_END };
	CHARACTER_TYPE m_ePlayerType = { SWORD_MAN };

	_int m_iTextureIndex = { -1 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Key_Input(_float fTimeDelta);

public:
	static CUI_SkillSection_ClassicSkill* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_CLASSICSKILL eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
