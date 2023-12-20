#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Camera.h"

BEGIN(Engine)
class CModel;
END

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
	class CModel* Get_PartModel(CHARACTER_TYPE eCharacterType, PART_TYPE ePartType, const _uint iIndex);
	vector<class CModel*>* Get_PartModels(CHARACTER_TYPE eCharacterType, PART_TYPE ePartType);


public:
	/* 툴에서 플레이어 애니메이션 편집시 해당 플레이어 타입 파츠들도 Apply 된다. */
	HRESULT Apply_PlayerAnimation(const CHARACTER_TYPE& eCharacterType);

	/* 툴에서 플레이어 애니메이션 저장시 해당 플레이어 타입 파츠들도 Save 된다. */
	HRESULT Save_Parts(const CHARACTER_TYPE& eCharacterType);


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
