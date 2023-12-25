#pragma once

#include "UI.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CUI_Emoticon_SpeechBalloon final : public CUI
{
protected:
	CUI_Emoticon_SpeechBalloon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Emoticon_SpeechBalloon(const CUI_Emoticon_SpeechBalloon& rhs);
	virtual ~CUI_Emoticon_SpeechBalloon() = default;

public:
	virtual void Set_Active(_bool bActive) override;
	void Set_Alpha(_bool bAlpha) { m_bAlpha = bAlpha; }
	void Set_EmoticonType(_uint iIndex);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CTransform* m_pCharacterTrans = { nullptr };

	//_float m_fTimeAcc = { 0.f };
	_bool m_bAlpha = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();

public:
	static CUI_Emoticon_SpeechBalloon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
