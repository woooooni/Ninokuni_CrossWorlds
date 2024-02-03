#pragma once
#include "GameNpc.h"

BEGIN(Client)

class CDreamMazeWitch_Npc final : public CGameNpc
{
public:
	enum NPC_WITCH_STATE {
		WITCHSTATE_INVASION_IDLE,
		WITCHSTATE_INVASION_APPEAR, WITCHSTATE_INVASION_DISAPPEARTURN, WITCHSTATE_INVASION_DISAPPEAR,
		WITCHSTATE_BATTLE_FOLLOWING, 
		WITCHSTATE_END
	};

private:
	CDreamMazeWitch_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	CDreamMazeWitch_Npc(const CDreamMazeWitch_Npc& rhs);
	virtual ~CDreamMazeWitch_Npc() = default;

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

public:
	void Witch_BattleSet(CGameObject* pStellia);
	void Set_IsBattle(_bool bIsBattle) { m_bIsBattle = bIsBattle; }
	void Set_IsFollowing(_bool bIsFollowing) { m_bIsFollowing = bIsFollowing; }
	_bool Get_IsFollowing() { return m_bIsFollowing; }

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States() override;
	virtual HRESULT Ready_Colliders() override;

private:
	HRESULT Ready_Sockets();

private:
	void Following_Stellia(_float fTimeDelta);

private:
	class CUI_World_NPCTag* m_pTag = { nullptr };
	class CUI_World_NPCSpeechBalloon* m_pBalloon = { nullptr };
	
	/* Battle */
	_bool m_bIsBattle = false;
	_bool m_bIsFollowing = false;
	/* Damping */
	CGameObject* m_pStellia = nullptr;
	CGameObject* m_pPlayer = nullptr;
	_float m_fFollowingSpeed = 0.f;

	Vec4		 m_vCurPos = Vec4::Zero;
	const _float m_fDampingCoefficient = 0.027f; /* (144 프레임 기준 댐핑 계수 - 0과 1사이 범위를 갖으며, 값이 클수록 빨리 따라감)*/
	const _float m_fDampingLimitDistance = 100.f;


public:
	static CDreamMazeWitch_Npc* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

