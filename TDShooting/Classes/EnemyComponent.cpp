#include "stdafx.h"
#include "EnemyComponent.h"
#include "SceneIngame.h"
#include "Bullet.h"
#include "Unit.h"

// EnemyAttackRoutine 컴포넌트 생성 함수
EnemyAttackRoutine* EnemyAttackRoutine::create(EnemyAttackType t)
{
    auto ret = new EnemyAttackRoutine(); // 새로운 객체 생성
    if (ret && ret->init(t)) ret->autorelease(); // 초기화 성공 시 자동 해제 설정
    else CC_SAFE_DELETE(ret); // 실패 시 메모리 해제
    return ret; // 생성된 객체 반환
}

// 컴포넌트 초기화 함수
bool EnemyAttackRoutine::init(EnemyAttackType t)
{
    if (!Component::init()) return false; // 상위 클래스 초기화 실패 시 종료

    this->type = t; // 전달받은 공격 타입을 멤버 변수에 저장

    return true; // 초기화 성공
}

// 매 프레임마다 호출되는 함수로 공격 타입에 따라 적의 행동을 업데이트
void EnemyAttackRoutine::update(float dt)
{
    switch (this->type) {
    case EnemyAttackType::NORMAL: logic_normal(dt); break; // NORMAL 타입일 경우 logic_normal 호출
    }
}

// NORMAL 공격 타입 로직을 처리하는 함수
void EnemyAttackRoutine::logic_normal(float dt)
{
    elapsed += dt; // 경과 시간 누적

    // 일정 시간이 지난 후 공격 실행
    if (elapsed > 1.0f) {
        elapsed = 0.0f; // 경과 시간 초기화
        SceneIngame* scene = (SceneIngame*)Director::getInstance()->getRunningScene(); // 현재 실행 중인 SceneIngame 씬을 가져옴
        auto player = scene->getPlayer(); // 플레이어 객체 가져오기
        if (player == nullptr) return; // 플레이어가 없는 경우 종료

        auto bullet = Bullet::create(ENEMY_BULLET_MASK, TAG_ENEMY_BULLET); // 적의 총알 생성
        bullet->setPosition(getOwner()->getPosition()); // 총알의 위치를 적의 위치로 설정
        scene->addChild(bullet); // 씬에 총알 추가

        Vec2 pos = player->getPosition() - getOwner()->getPosition(); // 플레이어와 적의 위치 차이를 구함
        pos = pos.getNormalized() * ENEMY_BULLET_SPEED; // 방향을 정규화하고 속도를 곱해 총알의 속도 설정
        bullet->getBody()->setVelocity(pos); // 총알에 속도 적용

        bullet->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr)); // 1초 후 총알 제거
    }
}
