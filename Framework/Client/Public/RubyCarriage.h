#pragma once

#include "DynamicObject.h"

BEGIN(Client)

class CRubyCarriage final : public CDynamicObject
{
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
	virtual HRESULT Ready_State();

public:
	const _bool& TakeTheCarriage() const { return m_bTakeTheCarriage; }
	void Set_TakeTheCarriage(_bool take) { m_bTakeTheCarriage = take; }

private:
	_bool m_bTakeTheCarriage = false;

public:
	static CRubyCarriage* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int eType);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

