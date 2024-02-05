#pragma once

#include "DynamicObject.h"

BEGIN(Client)

class CRubyCarriage final : public CDynamicObject
{
public:
	struct NPC_STAT
	{
		_TCHAR strKorName[MAX_PATH] = {};
		_TCHAR strSubName[MAX_PATH] = {};

		ELEMENTAL_TYPE eElementType = ELEMENTAL_END;

		_float fHp = 100.f;

		_float fMaxHp = 100.f;

		_int   iAtk = 0;
		_int   iDef = 0;
	};

public:
	enum STATE_TYPE { STATE_IDLE, STATE_MOVE, STATE_END };

protected:
	explicit CRubyCarriage(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	explicit CRubyCarriage(const CRubyCarriage& rhs);
	virtual ~CRubyCarriage() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Tick(_float fTimeDelta) override;
	virtual void LateTick(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	virtual HRESULT Render_ShadowDepth() override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual HRESULT Ready_Components() override;
	virtual HRESULT Ready_Colliders();
	virtual HRESULT Ready_State();
	virtual HRESULT Ready_States();

public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo);

public:
	const _bool& TakeTheCarriage() const { return m_bTakeTheCarriage; }
	void Set_TakeTheCarriage(_bool take) { m_bTakeTheCarriage = take; }

public:
	const NPC_STAT& Get_Stat() { return m_tStat; }
	const wstring& Get_KorName() { return m_strKorName; }
	const wstring& Get_SubName() { return m_strSubName; }

private:
	NPC_STAT m_tStat = {};
	wstring m_strKorName = TEXT("");
	wstring m_strSubName = TEXT("");

	ELEMENTAL_TYPE m_eDamagedElemental = ELEMENTAL_TYPE::WOOD;

private:
	_bool m_bTakeTheCarriage = false;

public:
	static CRubyCarriage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

