#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Stellia_Timer final : public CUI
{
protected:
	CUI_Stellia_Timer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Stellia_Timer(const CUI_Stellia_Timer& rhs);
	virtual ~CUI_Stellia_Timer() = default;

public:
	void Set_Timer(_float fMaxSecond); // 타이머 최대 '초(Sec)'를 설정한다. -> 바로 활성화됨

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CTexture* m_pBackTextureCom = { nullptr };
	class CStellia* m_pStellia = { nullptr };
	_uint m_iPass = { 14 };

	_float m_fMaxSecond = { 0 };
	_float m_fCurSecond = { 0 };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT Set_Owner();
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Stellia_Timer* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
