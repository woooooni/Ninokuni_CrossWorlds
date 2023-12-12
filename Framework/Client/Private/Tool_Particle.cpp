#include "stdafx.h"
#include "Tool_Particle.h"
#include "imgui.h"

CTool_Particle::CTool_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool(pDevice, pContext)
{
}

HRESULT CTool_Particle::Initialize()
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

void CTool_Particle::Tick(_float fTimeDelta)
{
	ImGui::Begin("Particle_Tool");

	// 트랜스폼 // 
	   // 포지션
	   // 로테이션
	   // 스케일

	// 파티클 시스템 //
	   // Duration          / 지속시간
	   // Looping           / 반복여부
	   // Start Delay       / 방출 시작 지연 시간
	   // Start Lifetime    / 생명 주기 (언제 사라지는가)
	   // Start Speed       / 입자 방출 속도 (상수/ 랜덤)
	   // Start Size        / 시작 크기
	   // Start Rotation    / 시작 각도
	   // Start Color       / 시작 색깔
	   // Play On Awake     / 씬 시작 시 재생 여부
	   // Emitter Velocity  / 방출속도
	   // Max Particles     / 최대 입자수

	   // Gravity Modifier  / 중력 효과
	   // Auto Random Seed  / 재생할 때마다 랜덤으로 표시
	   // Stop Action       / 모든 파티클이 완료되어 시스템이 중지되었을 때
	   // Culling Mode      / 화면을 벗어나면 일시정지할 것인가
	   // Ring Buffer Mode  / max particle 수에 도달할 때까지 파티클을 계속 활성화/ 맥스에 도달 시 오래된 파티클 재활용


	ImGui::End();
}

CTool_Particle* CTool_Particle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Particle* pInstance = new CTool_Particle(pDevice, pContext);
	if (FAILED(pInstance->Initialize()))
	{
		Safe_Release(pInstance);
		MSG_BOX("Failed_Create : CTool_Particle");
		return nullptr;
	}
	return pInstance;
}

void CTool_Particle::Free()
{
	__super::Free();
}
