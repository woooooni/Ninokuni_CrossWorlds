#pragma once

#include "Monster.h"

BEGIN(Client)

class CBoss abstract : public CMonster
{
public:
	enum class BOSS_BOOLTYPE {
		BOSSBOOL_ATKAROUND, BOSSBOOL_SKILLAROUND, BOSSBOOL_BERSERK,
		BOSSBOOL_END
	};

public:
	CBoss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, const MONSTER_STAT& tStat);
	CBoss(const CBoss& rhs);
	virtual ~CBoss() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);

	virtual HRESULT Render();
	HRESULT Render_ShadowDepth();

	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc) override;

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc) override;

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) override;
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) override;

public:
	virtual void Set_Infinite(_float fInfiniteTime, _bool bInfinite)
	{
		m_bInfinite = bInfinite;
		m_fInfiniteTime = fInfiniteTime;
		m_fAccInfinite = 0.f;
	}
	_bool Is_Infinite() { return m_bInfinite; }

public:
	CHierarchyNode* Get_Socket(const wstring& strSocketName);
	const MONSTER_STAT& Get_Stat() { return m_tStat; }


public:
	virtual void On_Damaged(const COLLISION_INFO& tInfo);
	virtual void Set_SkillTree() {};

	/* Bool 정보들 */
	virtual _bool  Get_Bools(BOSS_BOOLTYPE eType) { return m_bBools[(_uint)eType]; }
	virtual void   Set_Bools(BOSS_BOOLTYPE eType, _bool bIsBool) { m_bBools[(_uint)eType] = bIsBool; }

	/* Origin 정보 */
	virtual _vector Get_OriginPos() { return m_vOriginPos; }
	virtual _vector Get_OriginLook() { return m_vOriginLook; }

private:
	virtual HRESULT Ready_Components();
	virtual HRESULT Ready_States();
	virtual HRESULT Ready_Colliders();

private:
	void LookAt_DamagedObject(CGameObject* pAttacker);
	void Play_DamagedSound();

protected:
	_bool   m_bBools[(_uint)BOSS_BOOLTYPE::BOSSBOOL_END] = { false, }; // 보스가 사용하는 bool모음.

	// 최초 위치와 최초 look
	_vector	m_vOriginPos = {};
	_vector	m_vOriginLook = {};

public:
	virtual void Free() override;

};

END

