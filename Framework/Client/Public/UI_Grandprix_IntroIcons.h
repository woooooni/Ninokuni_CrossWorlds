#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CUI_Grandprix_IntroIcons final : public CUI
{
public:
	enum GRANDPRIX_INTRO { SWORDMAN, ENGINEER, SPLITTER, VS_ICON, INTRO_END };

protected:
	CUI_Grandprix_IntroIcons(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GRANDPRIX_INTRO eType);
	CUI_Grandprix_IntroIcons(const CUI_Grandprix_IntroIcons& rhs);
	virtual ~CUI_Grandprix_IntroIcons() = default;

public:
	virtual void Set_Active(_bool bActive) override;

	_bool Is_Finished() { return m_bFinished; }
	void Set_Finish(_bool bFinish) { m_bFinished = bFinish; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	GRANDPRIX_INTRO m_eType = { INTRO_END };
	_uint m_iPass = { 1 };
	_float2 m_vStart = _float2(0.f, 0.f);
	_float2 m_vGoal = _float2(0.f, 0.f);
	_float m_fSpeed = { 100.f };
	_bool m_bFinished = { false };

private:
	virtual HRESULT Ready_Components() override;
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

	void Update_Position(_float fTimeDelta);
	void Update_Scale(_float fTimeDelta);

public:
	static CUI_Grandprix_IntroIcons* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GRANDPRIX_INTRO eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END