#include "stdafx.h"
#include "GameInstance.h"
#include "Buff_Manager.h"
#include "Utils.h"
#include "Skill.h"
#include "Game_Manager.h"
#include "Character_Manager.h"
#include "Player.h"
#include "Character.h"
#include "Character_Projectile.h"


IMPLEMENT_SINGLETON(CBuff_Manager)

CBuff_Manager::CBuff_Manager()
{

}

HRESULT CBuff_Manager::Reserve_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	if (true == m_bReserved)
		return S_OK;

	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);


	return S_OK;
}

void CBuff_Manager::Tick(_float fTimeDelta)
{
	for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
	{		
		auto iter = m_Buffs[i].begin();
		while(iter != m_Buffs[i].end())
		{
			iter->second.fAccBuffTime += fTimeDelta;
			iter->second.fAccEffectDeleteTime += fTimeDelta;
			
			if (iter->second.fAccBuffTime >= iter->second.fEndBuffTime)
			{
				iter->second.fAccBuffTime = 0.f;
				iter->second.iAccBuffCount++;

				Use_Buff(iter->first);
				if (iter->second.iAccBuffCount >= iter->second.iBuffCount)
				{
					iter = m_Buffs[i].erase(iter);
					continue;
				}
			}
			iter++;
		}
	}
}

void CBuff_Manager::LateTick(_float fTimeDelta)
{
	
}



void CBuff_Manager::Apply_Buff(CHARACTER_TYPE eCharacterType, BUFF_TYPE eBuffType, const BUFF_DESC& BuffDesc)
{
	if (eCharacterType >= CHARACTER_TYPE::CHARACTER_END)
		return;

	if (eBuffType >= BUFF_TYPE::BUFF_TYPE_END)
		return;

	auto iter = m_Buffs[eCharacterType].find(eBuffType);

	if (iter == m_Buffs[eCharacterType].end())
		m_Buffs[eCharacterType].emplace(eBuffType, BuffDesc);
	else
		iter->second = BuffDesc;
}

void CBuff_Manager::Use_Buff(BUFF_TYPE eBuffType)
{
	if (HP_POTION == eBuffType)
	{
		for (_uint i = 0; i < CHARACTER_TYPE::CHARACTER_END; ++i)
		{
			CCharacter* pCharacter = CCharacter_Manager::GetInstance()->Get_Character(CHARACTER_TYPE(i));
			if (nullptr != pCharacter)
				pCharacter->Increase_HP(50);
		}
		
	}

	else if (DESTROYER_FRENGE_CHARGE == eBuffType)
	{
		for (_uint i = 0; i < 3; ++i)
		{
			CCharacter* pCharacter = CGame_Manager::GetInstance()->Get_Player()->Get_Character();
			if (nullptr == pCharacter)
			{
				MSG_BOX("Find_Character_Failed. : CBuff_Manager::Use_Buff");
				return;
			}



			CCharacter_Projectile::CHARACTER_PROJECTILE_DESC ProjectileDesc = {};
			ProjectileDesc.pOwner = pCharacter;


			CGameObject* pGameObject = nullptr;
			if (FAILED(GI->Add_GameObject(GI->Get_CurrentLevel(), LAYER_TYPE::LAYER_CHARACTER, L"Prototype_GameObject_Destroyer_FrengeCharge_Thunder", &ProjectileDesc, &pGameObject)))
			{
				MSG_BOX("Add GameObject Failed. DESTROYER_FRENGE_CHARGE : CBuff_Manager::Use_Buff");
				return;
			}

			CTransform* pTransform = pGameObject->Get_Component<CTransform>(L"Com_Transform");
			if (nullptr == pTransform)
			{
				MSG_BOX("Find Transform Failed. DESTROYER_FRENGE_CHARGE : CBuff_Manager::Use_Buff");
				return;
			}

			Vec4 vPosition = pCharacter->Get_CharacterTransformCom()->Get_Position();

			vPosition.z += GI->RandomFloat(-5.f, 5.f);
			vPosition.x += GI->RandomFloat(-5.f, 5.f);

			pTransform->Set_State(CTransform::STATE_POSITION, vPosition);
		}
			
	}

}




void CBuff_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	
}




