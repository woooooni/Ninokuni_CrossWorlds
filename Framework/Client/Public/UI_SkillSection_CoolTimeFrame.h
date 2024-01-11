#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_SkillSection_CoolTimeFrame final : public CUI
{
public:
	enum UI_COOLFRAME_TYPE { FRAME_FIRST, FRAME_SECOND, FRAME_THIRD, FRAME_END };

protected:
	CUI_SkillSection_CoolTimeFrame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
		UI_COOLFRAME_TYPE eType);
	CUI_SkillSection_CoolTimeFrame(const CUI_SkillSection_CoolTimeFrame& rhs);
	virtual ~CUI_SkillSection_CoolTimeFrame() = default;

public:
	virtual void Set_CharacterType(CHARACTER_TYPE eType) override;
	void Use_Skill();
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	CTexture* m_pMaskTextureCom = { nullptr };
	UI_COOLFRAME_TYPE m_eFrameType = { FRAME_END };
	class CSkill* m_pSkill = { nullptr };

	_int m_iPass = { 1 };
	_bool m_bUsable = { false };
	_float m_fMaxGauge = { 0.f };
	_float m_fCurGauge = { 0.f };

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 500.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);


private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT Ready_Skill();
	HRESULT	Bind_ShaderResources();
	void SetUp_FrameColor();
	void Movement_BasedOnHiding(_float fTimeDelta);

public:
	static CUI_SkillSection_CoolTimeFrame* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext,
		UI_COOLFRAME_TYPE eType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
