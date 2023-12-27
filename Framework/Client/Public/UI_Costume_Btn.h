#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Costume_Btn final : public CUI
{
public:
	enum UI_COSTUMEBTN
	{ COSTUME_CLOTH, COSTUME_HAIR, COSTUME_HAIRACC, COSTUME_EXTERIORDECO,
		COSTUME_FACEDECO, COSTUME_WEAPON, COSTUMEBTN_END };
	// 의상, 헤어, 머리 장식, 외형 장식, 얼굴, 무기
	enum UI_COSTUMEBTN_TYPE { COSTUMEBTN_UNCLICKED, COSTUMEBTN_CLICKED, SKILLBTNTYPE_END };

protected:
	CUI_Costume_Btn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, UI_COSTUMEBTN_TYPE eButtonType, UI_COSTUMEBTN eUIType);
	CUI_Costume_Btn(const CUI_Costume_Btn& rhs);
	virtual ~CUI_Costume_Btn() = default;

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
	UI_COSTUMEBTN m_eUIType = { UI_COSTUMEBTN::COSTUMEBTN_END };
	UI_COSTUMEBTN_TYPE m_eButtonType = { UI_COSTUMEBTN_TYPE::SKILLBTNTYPE_END };
	_bool m_bClicked = { false }; // 선택되었다

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Costume_Btn* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, UI_COSTUMEBTN_TYPE eButtonType, UI_COSTUMEBTN eUIType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
