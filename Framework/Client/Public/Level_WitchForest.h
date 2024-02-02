#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_WitchForest final : public CLevel
{
private:
	CLevel_WitchForest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_WitchForest() = default;

public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Tick(_float fTimeDelta) override;
	virtual HRESULT LateTick(_float fTimeDelta) override;
	virtual HRESULT Render_Debug() override;

public:
	virtual HRESULT Enter_Level() override;
	virtual HRESULT Exit_Level() override;

private:
	HRESULT Ready_Layer_Camera(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_BackGround(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Terrain(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Character(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Monster(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_UI(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Effect(const LAYER_TYPE eLayerType);
	HRESULT Ready_Layer_Dynamic(const LAYER_TYPE eLayerType, const wstring& strMapFileName);
	HRESULT Ready_Layer_Npc(const LAYER_TYPE eLayerType);
	HRESULT Ready_Light(const wstring& strLightFilePath);
	HRESULT Ready_Layer_Prop(const LAYER_TYPE eLayerType);

public:
	static class CLevel_WitchForest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END