#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_BtnChangeCamera final : public CUI
{
protected:
	CUI_BtnChangeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_BtnChangeCamera(const CUI_BtnChangeCamera& rhs);
	virtual ~CUI_BtnChangeCamera() = default;

public:
	void Hide_UI(_bool bHide) { m_bHide = bHide; m_bHideFinish = false; }

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
	_bool m_bUpdate = { false }; // 인덱스 Change용 변수
	_uint m_iTextureIndex = { 0 };

	_bool m_bHide = { false };
	_bool m_bHideFinish = { false };
	_float m_fHideSpeed = { 200.f };
	_float2 m_vOriginPosition = _float2(0.f, 0.f);
	_float2 m_vHidePosition = _float2(0.f, 0.f);

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Key_Input(_float fTimeDelta);
	void Movement_BasedOnHiding(_float fTimeDelta);
	void Update_CameraMode();

public:
	static CUI_BtnChangeCamera* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
