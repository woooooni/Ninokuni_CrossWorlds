#pragma once

#include "Base.h"
#include "GameObject.h"

#include "Animation.h"
#include "Transform.h"
#include "Model.h"

BEGIN(Engine)

class CBehaviorTree;
class ENGINE_DLL CBTNode abstract : public CBase
{
public:
	//typedef struct tagBTDesc
	//{
	//	CGameObject* pOwner = nullptr;
	//	//CGameObject* pTarget = nullptr;
	//	CModel*		 pOwnerModel = nullptr;
	//	//CModel*		 pTargetModel = nullptr;
	//	CTransform*  pOwnerTransform = nullptr;
	//	//CTransform*  pTargetTransform = nullptr;
	//}BTNODE_DESC;

public:
	enum class NODE_STATE { NODE_RUNNING, NODE_FAIL, NODE_SUCCESS, NODE_END };

protected:
	CBTNode();
	virtual ~CBTNode() = default;

public:
	virtual HRESULT Initialize_Prototype(CBehaviorTree* pBT);
	virtual void	Start() {};
	virtual CBTNode::NODE_STATE	Tick(const _float& fTimeDelta);

public:
	_bool	Get_IsStart() { return m_bIsStart; }
	void	Set_IsStart(_bool bIsStart) { m_bIsStart = bIsStart; }
	void	Init_Success() { 
		// m_bIsStart = false;
		m_bIsSucces = false; 
	}

	void	Init_Start();

	void Add_ChildNode(CBTNode* pNode);

protected:
	_bool	m_bIsStart = false;
	_bool	m_bIsSucces = false;

	list<CBTNode*> m_ltChildNode;
	CBehaviorTree* m_pBT = nullptr;

	//BTNODE_DESC m_tBTMonsterDesc = {};

public:
	virtual void Free() override;
};

END

