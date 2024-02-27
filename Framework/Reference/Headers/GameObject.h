#pragma once

#include "Engine_Defines.h"
#include "Base.h"

/* Ŭ���̾���Ʈ���� ������ �پ��� ���ӿ�����Ʈ���� �θ𰡵ȴ�. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
	
protected:
	/* ������ ������ �� */
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strObjectTag, _int iObjectType);

	/* �纻�� ������ �� */
	CGameObject(const CGameObject& rhs); /* ���� ������. */
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Tick(_float fTimeDelta) {};
	virtual void Tick(_float fTimeDelta);
	virtual void LateTick(_float fTimeDelta);
	virtual HRESULT Render();
	virtual HRESULT Render_ShadowDepth();
	virtual HRESULT Render_Reflect();

	virtual HRESULT Render_Instance(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) { return S_OK; }
	virtual HRESULT Render_Instance_AnimModel(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, 
		const vector<_float4x4>& WorldMatrices,
		const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) { return S_OK; }

	virtual HRESULT Render_Instance_AnimModel_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc) { return S_OK; }
	virtual HRESULT Render_Instance_Shadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices) { return S_OK; }

	virtual HRESULT Render_Minimap() { return S_OK; }

	// Cascade Shadow Render Temp
	virtual HRESULT Render_Cascade_Depth(const Matrix mCascadeShadowGenMat[3]) { return S_OK; }
	virtual HRESULT Render_Instance_AnimModel_CascadeShadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer,
		const vector<_float4x4>& WorldMatrices, const vector<TWEEN_DESC>& TweenDesc, const vector<ANIMODEL_INSTANCE_DESC>& AnimModelDesc, const Matrix lightMatrix[3]) {
		return S_OK;
	}
	virtual HRESULT Render_Instance_CascadeShadow(class CShader* pInstancingShader, class CVIBuffer_Instancing* pInstancingBuffer, const vector<_float4x4>& WorldMatrices, const Matrix lightMatrix[3]) { return S_OK; }


	virtual HRESULT Render_Picking() { return S_OK; }

public:
	virtual void Enter_Scene() { };
	virtual void Return_Pool() { };

public:
	virtual void Collision_Enter(const COLLISION_INFO& tInfo) {};
	virtual void Collision_Continue(const COLLISION_INFO& tInfo) {};
	virtual void Collision_Exit(const COLLISION_INFO& tInfo) {};


public:
	virtual void Ground_Collision_Enter(PHYSX_GROUND_COLLISION_INFO tInfo) {};
	virtual void Ground_Collision_Continue(PHYSX_GROUND_COLLISION_INFO tInfo) {};
	virtual void Ground_Collision_Exit(PHYSX_GROUND_COLLISION_INFO tInfo) {};

public:
	template<typename T>
	T* Get_Component(const wstring& strComponentTag)
	{
		if (0 >= m_Components.size())
			return nullptr;

		auto iter = m_Components.find(strComponentTag);

		if (iter == m_Components.end())
			return nullptr;

		return dynamic_cast<T*>(iter->second);
	}

	class CTransform*		Get_Component_Transform();		/* 'Com_Transform'���� �߰��Ǿ�� ��� ���� */
	class CModel*			Get_Component_Model();			/* 'Com_Model'���� �߰��Ǿ�� ��� ����  */
	class CStateMachine*	Get_Component_StateMachine();	/* 'Com_StateMachine' ���� �߰��Ǿ�� ��� ����  */
	class CShader*			Get_Component_Shader();			/* 'Com_Shader'���� �߰��Ǿ�� ��� ����  */
	class CRigidBody*		Get_Component_Rigidbody();		/* 'Com_Rigidbody'���� �߰��Ǿ�� ��� ����  */ 
	class CRenderer*		Get_Component_Renderer();		/* 'Com_Renderer'���� �߰��Ǿ�� ��� ����  */ 

public:
	virtual HRESULT SetUp_State(_fmatrix StateMatrix) { return S_OK; }


public:
	void Set_PrototypeTag(const wstring& strPrototypeTag) { m_strPrototypeTag = strPrototypeTag; }
	const wstring& Get_PrototypeTag() { return m_strPrototypeTag; }

	void Set_ObjectTag(const wstring& strTag) { m_strObjectTag = strTag; }
	const wstring& Get_ObjectTag() { return m_strObjectTag; }
	
	void Set_ObjectID(_uint iID) { m_iObjectID = iID; }
	
	void Set_Enable(_bool enable) { m_bEnable = enable; }
	const _bool& Get_Enable() const { return m_bEnable; }

	_uint Get_ObjectID() { return m_iObjectID; }
	_uint Get_ObjectType() { return m_iObjectType; }

	void Set_Dead(_bool bDead) { m_bDead = bDead; }
	_bool Is_Dead() { return m_bDead; }

	void Reserve_Dead(_bool bReserve) { m_bReserveDead = bReserve; }
	_bool Is_ReserveDead() { return m_bReserveDead; }

	void Set_NaviObject(_bool bNaviGation) { m_bNaviObject = bNaviGation; }
	_bool Is_NaviObject() { return m_bNaviObject; }

	const _bool& IsQuestItem() const { return m_bIsQuestObject; }
	void Set_QuestItem(_bool quest) { m_bIsQuestObject = quest; }

	const _float& Get_CamDistance() const { return m_fCamDistance; }

	_float Get_ObjectTimeScale() { return m_fObjectTimeScale; }
	void Set_ObjectTimeScale(_float fTimeScale) { m_fObjectTimeScale = fTimeScale; }


public:
	vector<class CCollider*>& Get_Collider(_uint eDetectionType) { return m_Colliders[eDetectionType]; }
	HRESULT Add_Collider(_uint iLevelIndex, _uint eColliderType, _uint eDetectionType, void* pArg);
	virtual HRESULT Set_ActiveColliders(_uint eDetectionType, _bool bActive);


	virtual HRESULT Set_Collider_AttackMode(_uint eAttackMode, _float fAirBornPower, _float fPushPower, _float fDamage, _bool bHitLag = true);
	virtual HRESULT Set_Collider_Elemental(ELEMENTAL_TYPE eElementalType);

protected:
	void LateUpdate_Collider(_float fTimedelta);
	void Render_Collider();
	

protected:
	virtual HRESULT Ready_Components() PURE;
	HRESULT Add_Component(const wstring& pComponentTag, class CComponent* pComponent);
	HRESULT Add_Component(_uint iLevelIndex, const wstring& pPrototypeTag, const wstring& pComponentTag, CComponent** ppOut, void* pArg = nullptr);
	HRESULT Compute_CamZ(_fvector vWorldPos);




protected:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

protected:
	map<wstring, class CComponent*> m_Components;
	map<_uint, vector<class CCollider*>> m_Colliders;

protected:
	_float				m_fCamDistance = 0.f;
	wstring				m_strPrototypeTag = L"";
	wstring				m_strObjectTag = L"";

	static				_uint g_ObjecId;
	_int				m_iObjectID = -1;
	_int				m_iObjectType = -1;

	_bool				m_bReserveDead = false;
	_bool				m_bDead = false;

	_bool				m_bEnable = false;
	_bool				m_bIsQuestObject = false;

protected:
	_bool m_bNaviObject = false;


protected:
	Matrix m_ServerMatrix = Matrix::Identity;

protected:
	_float3 m_vBloomPower = { 0.f, 0.f, 0.f };

protected:
	_float m_fObjectTimeScale = 1.f;


protected:
	CComponent* Find_Component(const wstring & strComponentTag);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END