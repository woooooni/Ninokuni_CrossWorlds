#include "stdafx.h"
#include "GameInstance.h"
#include "Skill_Manager.h"
#include "Utils.h"
#include "Skill.h"

IMPLEMENT_SINGLETON(CSkill_Manager)

CSkill_Manager::CSkill_Manager()
{

}

HRESULT CSkill_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);



	m_bReserved = true;


	return S_OK;
}

void CSkill_Manager::Tick(_float fTimeDelta)
{
	
	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto iter : m_Skills[i])
		{
			iter.second->Tick(fTimeDelta);
		}
	}
}

void CSkill_Manager::LateTick(_float fTimeDelta)
{
	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto iter : m_Skills[i])
		{
			iter.second->LateTick(fTimeDelta);
		}
	}
}

_bool CSkill_Manager::Use_Skill(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType)
{
	if (CHARACTER_TYPE::CHARACTER_END >= eCharacterType || SKILL_TYPE::SKILL_END >= eSkillType)
		return false;

	return true;
}

_bool CSkill_Manager::Is_Useable(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType)
{
	if (CHARACTER_TYPE::CHARACTER_END >= eCharacterType || SKILL_TYPE::SKILL_END >= eSkillType)
		return false;

	auto iter = m_Skills[eCharacterType].find(eSkillType);
	if (iter == m_Skills[eCharacterType].end())
		return false;

	return iter->second->Is_Useable();
}

CSkill* CSkill_Manager::Get_Skill(CHARACTER_TYPE eCharacterType, SKILL_TYPE eSkillType)
{
	if (CHARACTER_TYPE::CHARACTER_END >= eCharacterType || SKILL_TYPE::SKILL_END >= eSkillType)
		return nullptr;

	auto iter = m_Skills[eCharacterType].find(eSkillType);
	if (iter == m_Skills[eCharacterType].end())
		return nullptr;

	
	return iter->second;
}


void CSkill_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{
		for (auto iter : m_Skills[i])
		{
			Safe_Release(iter.second);
		}
		m_Skills[i].clear();
	}
	
}



