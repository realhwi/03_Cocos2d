#ifndef __SCENE_HOME_H__
#define __SCENE_HOME_H__

#include "stdafx.h" 

// SceneHome 클래스 - 홈 화면 장면을 나타냅니다.
class SceneHome : public Scene {
public:
    Button* btnStart; // 게임 시작 버튼
    Button* btnQuit; // 게임 종료 버튼

    static SceneHome* create(); // SceneHome 객체를 생성하는 정적 함수
    virtual bool init() override; // 장면 초기화 함수 (Scene의 init 함수 재정의)
};

#endif