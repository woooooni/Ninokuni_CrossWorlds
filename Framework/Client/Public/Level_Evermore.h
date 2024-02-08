#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Evermore final : public CLevel
{
private:
	CLevel_Evermore(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Evermore() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Debug() override;

public:
	virtual HRESULT Enter_Level() override;
	virtual HRESULT Exit_Level() override;

public:
	static _bool g_bFirstEnter;

private:
	HRESULT Ready_Layer_Camera(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_BackGround(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Terrain(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Character(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Monster(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Npc(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_UI(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Effect(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Prop(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName);
	HRESULT Ready_Light(const wstring& strLightFilePath);
	HRESULT Ready_Trigger(const wstring& strTriggerName);

public:
	static class CLevel_Evermore* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END