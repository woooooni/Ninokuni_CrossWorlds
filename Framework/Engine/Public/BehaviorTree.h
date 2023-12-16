#pragma once

#include "Component.h"
#include "BTNode.h"

#include "Transform.h"
#include "Model.h"
#include "GameObject.h"

BEGIN(Engine)

struct ENGINE_DLL djb2Hasher
{
	size_t operator() (const wstring& str) const
	{
		_ulong	hash = 5381;
		_uint	size = str.length();

		for (_uint i = 0; i < size; i++)
			hash = ((hash << 5) + hash) + (str[i]); /* hash * 33 + c */

		return hash;
	}
};	// HashFunctionObject (djb2)

struct tagBlackBoard	// Interface
{
	virtual void* GetValue() PURE;
};

template<typename T>
struct tagBlackBoardData : public tagBlackBoard
{
public:
	tagBlackBoardData() = default;
	tagBlackBoardData(T _value) :value(_value) {}
	virtual void* GetValue() override { return &value; }
	void			SetValue(T _value) { value = _value; }

private:
	T value;
};

template<typename T>
struct tagBlackBoardData<T*> : public tagBlackBoard
{
public:
	tagBlackBoardData() = default;
	tagBlackBoardData(T* _value) :value(_value) {}
	virtual void* GetValue() override { return value; } // 이거 캐스팅 안되면 value 바로 가져오도록
	void			SetValue(T* _value) { value = _value; }

private:
	T* value;
};

#define GET_VALUE(type, var) static_cast<type*>(var->second->GetValue())
#define SET_VALUE(type, var, value) static_cast<tagBlackBoardData<type>*>(var->second)->SetValue(value)

using BLACKBOARD = unordered_map<wstring, tagBlackBoard*, djb2Hasher>;

////////////////////////////////////////////////////////////////

class ENGINE_DLL CBehaviorTree abstract : public CComponent
{
protected:
	CBehaviorTree(ID3D11Device * pDevice, ID3D11DeviceContext * pContext);
	CBehaviorTree(const CBehaviorTree& rhs);
	virtual ~CBehaviorTree() = default;

public:
	virtual HRESULT Initialize_Prototype(CGameObject * pObject);
	virtual HRESULT Initialize(void* pArg);
	virtual void	Tick(const _float & fTimeDelta);
	virtual void	LateTick(const _float & fTimeDelta);

public:
	BLACKBOARD& GetBlackBoard() { return m_BlackBoard; }

	CModel* Get_BTModel() { return m_pBTModel; }

public:
	virtual void     Init_NodeStart() {};

	virtual CBTNode* Get_CurNode() { return m_pCurNode; }
	virtual void	 Set_CurNode(CBTNode* pNode) { m_pCurNode = pNode; }

protected:
	CBTNode* m_pRootNode = nullptr;
	CBTNode* m_pCurNode = nullptr;

	CModel* m_pBTModel = nullptr;

	BLACKBOARD			m_BlackBoard;

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END