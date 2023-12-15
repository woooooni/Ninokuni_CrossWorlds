#pragma once

#include "UI.h"

// 퀘스트 Window(우측 간이Window)에서 사용하는 Accpt버튼
// 완료버튼도 같이 사용할 예정

BEGIN(Client)
class CUI_BtnAccept final : public CUI
{
protected:
	CUI_BtnAccept(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BtnAccept(const CUI_BtnAccept& rhs);
	virtual ~CUI_BtnAccept() = default;

public:
	virtual void Set_Active(_bool bActive) override;

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
	_bool m_bFinish = { false };
	_bool m_bResizeStart = { false };

	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);
	_float m_fTimeAcc = { 0.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);

public:
	static CUI_BtnAccept* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
