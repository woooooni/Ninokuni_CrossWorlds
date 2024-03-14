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
	_bool Get_Resizable() { return m_bResizable; }
	void Set_Resizable(_bool bResize) { m_bResizable = bResize; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CCharacter* m_pCharacter = { nullptr };

	_float m_fTimeAcc = { 0.f };
	_bool m_bFinish = { false };
	_bool m_bResizable = { false };

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);

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
