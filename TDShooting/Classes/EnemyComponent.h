#ifndef __ENEMY_COMPONENT_H__
#define __ENEMY_COMPONENT_H__

#include "stdafx.h"

// 적의 공격 타입을 정의하는 열거형
enum class EnemyAttackType {
    NORMAL // 일반 공격 타입
};

// 적의 공격을 관리하는 컴포넌트 클래스
class EnemyAttackRoutine : public Component {
private:
    EnemyAttackType type; // 공격 타입을 저장
    float elapsed = 0; // 시간 경과를 저장하는 변수
public:
    static EnemyAttackRoutine* create(EnemyAttackType t); // 컴포넌트 생성 함수
    virtual bool init(EnemyAttackType t); // 초기화 함수

    virtual void update(float dt); // 매 프레임마다 호출되는 업데이트 함수

    void logic_normal(float dt); // NORMAL 공격 타입의 로직을 처리하는 함수
};

// 적의 이동을 관리하는 컴포넌트 클래스 (아직 구현되지 않음)
class EnemyMovementRoutine : public Component {

};

#endif 
