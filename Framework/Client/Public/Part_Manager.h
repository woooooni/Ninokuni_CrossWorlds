#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"

BEGIN(Client)
class CPart_Manager : public CBase
{	
	DECLARE_SINGLETON(CPart_Manager)

private:
	CPart_Manager();
	virtual ~CPart_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	class CModel* Get_PartModel(CHARACTER_TYPE eCharacterType, PART_TYPE ePartType);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	map<PART_TYPE, vector<CModel*>> m_PartModels[CHARACTER_TYPE::CHARACTER_END];

private:
	HRESULT Ready_PartModels();

public:
	virtual void Free() override;
};

END
