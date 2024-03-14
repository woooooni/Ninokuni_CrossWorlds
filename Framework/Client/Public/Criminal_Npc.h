#pragma once
#include "GameNpc.h"

BEGIN(Client)

class CCriminal_Npc final : public CGameNpc
{
public:
	enum NPC_CRIMINAL_STATE {
		NPC_CRIMINAL_ESCAPE = 4, NPC_CRIMINAL_ESCAPERUN, NPC_CRIMINAL_LOCKPICK, NPC_CRIMINAL_FUN, NPC_CRIMINAL_END };

private:
	CCriminal_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CCriminal_Npc(const CCriminal_Npc& rhs);
	virtual ~CCriminal_Npc() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;


public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo) override;

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();

private:
	class CUI_World_NPCTag* m_pTag = { nullptr };

public:
	static CCriminal_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

