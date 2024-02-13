#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_World_Interaction final : public CUI
{
public:
	enum UI_INTERACTION_TYPE {
		INTERACTION_KICK,
		INTERACTION_PICKUP, INTERACTION_PICKDOWN, 
		INTERACTION_QUEST, INTERACTION_END };

protected:
	CUI_World_Interaction(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_World_Interaction(const CUI_World_Interaction& rhs);
	virtual ~CUI_World_Interaction() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_Owner(class CGameObject* pOwner);
	void Set_InteractionType(UI_INTERACTION_TYPE eType) { m_eType = eType; }

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CGameObject* m_pOwner = { nullptr };
	class CCharacter* m_pPlayer = { nullptr };
	UI_INTERACTION_TYPE m_eType = { INTERACTION_END };

	_bool m_bRotate = { false };
	_float m_fTimeAcc = { 0.f };
	_float2 m_vOffset = _float2(0.f, 0.f);
	_float2 m_vOriginOffset = _float2(0.f, 0.f);

	_bool m_bResize = { false };
	_float2 m_vOriginSize = _float2(0.f, 0.f);
	_float2 m_vMinSize = _float2(0.f, 0.f);

	_float m_fSpeed = { 30.f };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

private:
	void Update_ButtonIcon();

public:
	static CUI_World_Interaction* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
