#pragma once

#include "UI.h"

BEGIN(Client)
class CUI_Emoticon_BalloonEmoticon final : public CUI
{
protected:
	CUI_Emoticon_BalloonEmoticon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Emoticon_BalloonEmoticon(const CUI_Emoticon_BalloonEmoticon& rhs);
	virtual ~CUI_Emoticon_BalloonEmoticon() = default;

public:
	void Set_TextureIndex(_uint iIndex);

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

private:
	class CTexture* m_pTextureSecond = { nullptr };
	class CTexture* m_pTextureThird = { nullptr };
	class CTexture* m_pTextureFourth = { nullptr };
	class CTexture* m_pTextureFifth = { nullptr };
	class CTexture* m_pTextureSixth = { nullptr };
	class CTexture* m_pTextureSeventh = { nullptr };
	class CTexture* m_pTextureEighth = { nullptr };

	_uint m_iTextureIndex = { 0 };
	_uint m_iMaxIndex = { 0 };

	_float m_fFrame = { 0.f };
	_bool m_bFinished = { false };
	_float m_fTimeAcc = { 0.f };

	_bool m_bAlpha = { false };

private:
	virtual HRESULT	Ready_Components() override;

private:
	HRESULT	Ready_State();
	HRESULT	Bind_ShaderResources();
	void Calculate_AnimUV(_uint iIndex);

public:
	static CUI_Emoticon_BalloonEmoticon* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
