#pragma once
#include "Tool.h"

BEGIN(Engine)
class CGameObject;
class CLight;
END

BEGIN(Client)

struct LightHelper
{
	Vec3 vRotationDeg;
	Vec3 vSpotAngles = Vec3(1.f, 1.f, 1.f);
};

class CTool_Map final : public CTool
{
public:
	enum LEVEL_LIST { LOBBY, EVERMORE, HALL, WINTER, WITCH, LEVEL_LIST_END };
	enum BRUSH_STATE { NONE_BRUSH, HEGIHT_BRUSH, TEXTURE_BRUSH, BRUSH_END };

private:
	CTool_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Map() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Tick(_float fTimeDelta) override;

private:
	void AddMapObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void DeleteObject(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void BatchObject(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void BatchTerrain(LEVELID iLevelID, LAYER_TYPE iLayerType);
	void DeleteTerrain(LEVELID iLevelID, LAYER_TYPE iLayerType);

	void DeleteLight(_uint iLightID);


	void Picking();
private:
	void MapObjectSpace();
	void MapLightSpace();
	void MapTerrainSpace();
private:
	void ChangeState();

private:
	HRESULT Save_Map_Data(const wstring& strMapFileName);
	HRESULT Load_Map_Data(const wstring& strMapFileName);

	HRESULT Save_Light_Data(const wstring& strLightFilePath);
	HRESULT Load_Light_Data(const wstring& strLightFilePath);



private:
	// 오브젝트 상태제어 
	_bool m_bAddObject = false;
	_bool m_bPlantMode = false;
	_bool m_bAddTerrain = false;

	_bool m_bAddLight = false;
	LightHelper m_LightHelper = {};

	// 내 컨트롤 제어상태
	_int m_iControlState = 0;
	_int m_iLightControlState = 0;
	_int m_iTerrainState = 0;

	wstring m_strLevelName = L"Evermore";
	wstring m_strPrevLevelName = L"";
	_uint m_iCurrentLevel = 0;
	const char* m_ImguiSelectableNameList[LEVEL_LIST_END] =
	{ "Lobby", "Evermore", "Hall", "Winter", "Witch" };

	// 선택한 오브젝트
	CGameObject* m_pSelectObj = nullptr;
	CGameObject* m_pSelectTerrain = nullptr;
	CLight* m_pSelectLight = nullptr;

	// IMGUI STATE
	_bool m_bWindowMoveFlag = false;
	_bool m_bWindowResizeFlag = false;

	// Terrain
	Vec2 m_vTerrainCount;
	
	_int m_iBrushType = 0;

	_float m_fBrushStrenght = 20.f;
	_float m_fBrushSize = 30.f;


public:
	static CTool_Map* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END
