#pragma once

#include "Base.h"

#include "Client_Defines.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"




BEGIN(Client)
class CImGui_Manager : public CBase
{
	DECLARE_SINGLETON(CImGui_Manager)
	
private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT Reserve_Manager(HWND hWnd,  ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void	Tick(_float fTimeDelta);
	void	Render_ImGui();
	

	

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;
	HWND m_hWnd = nullptr;


private:
	_bool m_bShowModel_Tool = false;
	_bool m_bShowCamera_Tool = false;
	_bool m_bShowEffect_Tool = false;
	_bool m_bShowParticle_Tool = false;
	_bool m_bShowMap_Tool = true;
	_bool m_bShowTerrain_Tool = true;
	_bool m_bShowNavigation_Tool = false;


private:
	class CTool_Model* m_pModel_Tool = nullptr;
	class CTool_Effect* m_pEffect_Tool = nullptr;
	class CTool_Particle* m_pParticle_Tool = nullptr;
	class CTool_Map* m_pMap_Tool = nullptr;
	class CTool_Navigation* m_pNavigation_Tool = nullptr;
	class CTool_Camera* m_pCamera_Tool = nullptr;
	class CTool_Terrain* m_pTerrain_Tool = nullptr;


public:
	virtual void Free() override;
};
END