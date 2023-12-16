#include "stdafx.h"
#include "StelliaBT.h"

#include "GameInstance.h"

#include "Stellia.h"

#include "BTNode_Condition.h"
#include "BTNode_Leaf.h"
#include "BTNode_Select.h"
#include "BTNode_RandomSelect.h"
#include "BTNode_Sequence.h"
#include "BTNode_Loop.h"

#include "StelliaNode_Dead.h"

#include "StelliaNode_Chase.h"

#include "StelliaNode_Scratch.h"
#include "StelliaNode_DoubleScratch.h"
#include "StelliaNode_TailRoll.h"
#include "StelliaNode_Stamp.h"
#include "StelliaNode_Laser.h"
#include "StelliaNode_Charge.h"
#include "StelliaNode_ThirdLaser.h"
#include "StelliaNode_Boom.h"


CStelliaBT::CStelliaBT(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBehaviorTree(pDevice, pContext)
{
}

CStelliaBT::CStelliaBT(const CStelliaBT& rhs)
	: CBehaviorTree(rhs)
{
}

HRESULT CStelliaBT::Initialize_Prototype(CGameObject* pObject)
{
	// m_BlackBoard.emplace(TEXT("Stellia"), new tagBlackBoardData<CGameObject*>((CGameObject*)this));

	CBTNode::tagBTDesc btNodeDesc;
	btNodeDesc.pOwner = pObject;
	btNodeDesc.pOwnerModel = pObject->Get_Component<CModel>(L"Com_Model");
	btNodeDesc.pOwnerTransform = pObject->Get_Component<CTransform>(L"Com_Transform");

	//btNodeDesc.pTarget = pObject;
	//btNodeDesc.pTargetModel = pObject->Get_Component<CModel>(L"Com_Model");
	//btNodeDesc.pTargetTransform = pObject->Get_Component<CTransform>(L"Com_Transform");


	CModel* pModel = pObject->Get_Component<CModel>(L"Com_Model");

	m_pStellia = dynamic_cast<CStellia*>(pObject);
	m_pRootNode = CBTNode_Select::Create(this);

	/* 상위 Sequence 관련 */
	CBTNode_Sequence* pSeq_Dead = CBTNode_Sequence::Create(this);
	CBTNode_Sequence* pSeq_Combat = CBTNode_Sequence::Create(this);
	
	/* Dead 관련 */
	/* function<_bool()>을 받는 CBTNode_Condition::Create 함수에서는 멤버 함수를 사용하고 있기 때문에 추가적인 처리가 필요 */
	CStelliaNode_Dead* pDeadNode = CStelliaNode_Dead::Create(&btNodeDesc, this);
	
	/* Chase 관련 */
	//CBTNode_Loop* pLoop_IsChase = CBTNode_Loop::Create(bind(&CStelliaBT::IsChase, this));
	//CStelliaNode_Chase* pChaseNode = CStelliaNode_Chase::Create(pModel);

	/* Battle 관련 */
	// CBTNode_RandomSelect* pRanSel_Battle = CBTNode_RandomSelect::Create();
	//CBTNode_Select* pSel_Battle = CBTNode_Select::Create();
	CStelliaNode_Scratch* pScratchNode = CStelliaNode_Scratch::Create(&btNodeDesc, this);
	CStelliaNode_DoubleScratch* pDoubleScratchNode = CStelliaNode_DoubleScratch::Create(&btNodeDesc, this);
	CStelliaNode_TailRoll* pTailRollNode = CStelliaNode_TailRoll::Create(&btNodeDesc, this);
	CStelliaNode_Stamp* pStampNode = CStelliaNode_Stamp::Create(&btNodeDesc, this);
	CStelliaNode_Laser* pLaserNode = CStelliaNode_Laser::Create(&btNodeDesc, this);
	CStelliaNode_Charge* pChargeNode = CStelliaNode_Charge::Create(&btNodeDesc, this);
	CStelliaNode_ThirdLaser* pThirdLaserNode = CStelliaNode_ThirdLaser::Create(&btNodeDesc, this);
	CStelliaNode_Boom* pBoomNode = CStelliaNode_Boom::Create(&btNodeDesc, this);

	/* condition 관련 */
	CBTNode_Condition* pCon_IsDead = CBTNode_Condition::Create(bind(&CStelliaBT::IsZeroHp, this), pDeadNode, pScratchNode);

	m_pRootNode->Add_ChildNode(pSeq_Dead); // root.push(dead Sequence)
	pSeq_Dead->Add_ChildNode(pCon_IsDead); // dead Sequence.push(dead Condition)
	pSeq_Dead->Add_ChildNode(pDeadNode); // dead Sequence.push(dead Lerp)

	m_pRootNode->Add_ChildNode(pSeq_Combat); // root.push(combat Sequence)
	//pSeq_Combat->Add_ChildNode(pLoop_IsChase); // combat sequence.push(chase loop)
	//pLoop_IsChase->Add_ChildNode(pChaseNode); // chase loop.push(chase lerp)

	pSeq_Combat->Add_ChildNode(pScratchNode); // battle randomSelect.push(scratch Lerp)
	pSeq_Combat->Add_ChildNode(pDoubleScratchNode); // battle randomSelect.push(doubleScratch Lerp)
	pSeq_Combat->Add_ChildNode(pTailRollNode); // battle randomSelect.push(tailRoll Lerp)
	pSeq_Combat->Add_ChildNode(pStampNode); // battle randomSelect.push(stamp Lerp)
	pSeq_Combat->Add_ChildNode(pLaserNode); // battle randomSelect.push(laser Lerp)
	pSeq_Combat->Add_ChildNode(pChargeNode); // battle randomSelect.push(charge Lerp)
	pSeq_Combat->Add_ChildNode(pThirdLaserNode); // battle randomSelect.push(thirdLaser Lerp)
	pSeq_Combat->Add_ChildNode(pBoomNode); // battle randomSelect.push(boom Lerp)


	return S_OK;
}

HRESULT CStelliaBT::Initialize(void* pArg)
{
	return S_OK;
}

void CStelliaBT::Tick(const _float& fTimeDelta)
{
	m_pRootNode->Tick(fTimeDelta);

}

void CStelliaBT::LateTick(const _float& fTimeDelta)
{
	IsZeroHp();
	IsChase();
	IsBerserk();
}

_bool CStelliaBT::IsZeroHp()
{
	if (m_pStellia->Get_Stat().fHp <= 0)
		return true;

	return false;
}

_bool CStelliaBT::IsChase()
{
	// 추후 거리계산 필요
	if (GetAsyncKeyState('H'))
		m_BIsChase = !m_BIsChase;

	return m_BIsChase;
}

_bool CStelliaBT::IsBerserk()
{
	return false;
}

CStelliaBT* CStelliaBT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pObject)
{
	CStelliaBT* pInstance = new CStelliaBT(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pObject)))
	{
		MSG_BOX("Fail Create : CStelliaBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStelliaBT::Clone(void* pArg)
{
	CStelliaBT* pInstance = new CStelliaBT(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Fail Clone : CStelliaBT");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStelliaBT::Free()
{
	__super::Free();

	Safe_Release(m_pRootNode);
}
