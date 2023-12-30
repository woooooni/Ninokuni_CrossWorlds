#pragma once

#include "Client_Defines.h"
#include "Base.h"


/*
* [TW] 모든 캐릭터와 파츠 모델들을 가지고 있습니다.
*/
BEGIN(Client)
class CCharacter;

class CCharacter_Manager : public CBase
{	
	DECLARE_SINGLETON(CCharacter_Manager)

private:
	CCharacter_Manager();
	virtual ~CCharacter_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

public:
	class CCharacter* Get_Character(CHARACTER_TYPE eType) { 
		if (eType >= CHARACTER_TYPE::CHARACTER_END)
			return nullptr;

		return m_pCharacters[eType];
	}

public:
	class CModel* Get_PartModel(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType, const _uint iIndex);
	vector<class CModel*>* Get_PartModels(const CHARACTER_TYPE& eCharacterType, const PART_TYPE& ePartType);


private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;

private:
	// 캐릭터 사본을 보관한다.
	class CCharacter* m_pCharacters[CHARACTER_TYPE::CHARACTER_END];
	map<PART_TYPE, vector<class CModel*>> m_PartModels[CHARACTER_TYPE::CHARACTER_END];

private:
	_bool m_bReserved = false;
	
private:
	/* Loading_Proto_Parts_Model() -> Ready_PartModels() -> Ready_Characters()*/
	HRESULT Loading_Proto_Parts_Model(const wstring& strFolderPath);
	HRESULT Ready_PartModels();
	HRESULT Ready_Characters();

public:
	virtual void Free() override;
};

END
