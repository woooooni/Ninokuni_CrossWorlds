#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)
class CSkill;

class CSkill_Manager : public CBase
{	
	DECLARE_SINGLETON(CSkill_Manager)

private:
	CSkill_Manager();
	virtual ~CSkill_Manager() = default;

public:
	HRESULT Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void Tick(_float fTimeDelta);
	void LateTick(_float fTimeDelta);

public:
	_bool Use_Skill(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType);
	_bool Is_Useable(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType);
	class CSkill* Get_Skill(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType);

private:
	ID3D11Device* m_pDevice = nullptr;
	ID3D11DeviceContext* m_pContext = nullptr;


private:
	map<SKILL_TYPE, class CSkill*> m_Skills[CHARACTER_TYPE::CHARACTER_END];

private:
	_bool m_bReserved = false;

public:
	virtual void Free() override;
};

END
