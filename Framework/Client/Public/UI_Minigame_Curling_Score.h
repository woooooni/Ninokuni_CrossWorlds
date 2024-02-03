#pragma once

#include "UI_Minigame_Curling_Base.h"

BEGIN(Client)
class CUI_Minigame_Curling_Score final : public CUI_Minigame_Curling_Base
{
	typedef struct tagBindResourcesDesc
	{
		_uint iTextureIndex = 0;
		CUI::UI_INFO tUIDesc = {};
	}BIND_DESC;

private:
	CUI_Minigame_Curling_Score(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType);
	CUI_Minigame_Curling_Score(const CUI_Minigame_Curling_Score& rhs);
	virtual ~CUI_Minigame_Curling_Score() = default;

public:
	virtual HRESULT	Initialize_Prototype();
	virtual HRESULT	Initialize(void* pArg);
	virtual void	Tick(_float fTimeDelta);
	virtual void	LateTick(_float fTimeDelta);
	virtual HRESULT	Render();

public:
	virtual void	Send_Message_Int(const _int& iValue);

private:
	virtual HRESULT	Ready_Components() override;
	virtual HRESULT	Ready_Default() override;
	HRESULT			Bind_ShaderResources();

	const vector<BIND_DESC> Get_BindDesc();

private:
	_bool			m_bPlayerType = false;
	_uint			m_iCurScore = 0;

	const _uint		m_iInterdigitSpacing = 60;	// 자릿수간 간격
	CUI::UI_INFO	m_tUIDesc_OriginLeft = {};	// 왼쪽 첫 자릿수 기준 데이터

public:
	static CUI_Minigame_Curling_Score* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& wstrTag, const _bool& bPlayerType);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
