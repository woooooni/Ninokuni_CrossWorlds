#pragma once
#include "GameNpc.h"

BEGIN(Client)

class CRuby final : public CGameNpc
{
public:
	enum RUBY_STATE { RUBY_IDLE, RUBY_WALK, RUBY_TALK, RUBY_SEAT, RUBY_STATE_END };
	enum ESCORT_SECTION { SECTION1, SECTION2, SECTION3, SECTION_END };
private:
	CRuby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CRuby(const CRuby& rhs);
	virtual ~CRuby() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;


public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo) override;

public:
	virtual CGameObject* Get_RidingObject() { return m_pRidingObject; }
	const _bool& Get_QuestSection(_uint iIndex) const
	{
		if (iIndex >= ESCORT_SECTION::SECTION_END)
			return false;
		
		return m_bQuestSection[iIndex];
	}

	
	void Set_QuestSection(_uint iIndex, _bool curSection)
	{
		if (iIndex >= ESCORT_SECTION::SECTION_END)
			return;

		m_bQuestSection[iIndex] = curSection;
	}

private:
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();

private:
	class CUI_World_NPCTag* m_pTag = { nullptr };
	class CGameObject* m_pRidingObject = nullptr;
	class CGameObject* m_pQuestItem = nullptr;

	

private:
	_bool m_bQuestSection[ESCORT_SECTION::SECTION_END] = { false, false };

public:
	static CRuby* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

