#include "stdafx.h" // 표준 헤더 파일 포함
#include "SceneHome.h" // 홈 화면 장면 헤더 파일 포함
#include "Environment.h" // 환경 설정 헤더 파일 포함
#include "SceneIngame.h" // 인게임 장면 헤더 파일 포함

// SceneHome 객체를 생성하는 정적 함수
SceneHome* SceneHome::create()
{
    // SceneHome 객체를 동적으로 생성하고 초기화합니다.
    auto ret = new SceneHome();
    if (ret && ret->init()) ret->autorelease(); // 초기화 성공 시 자동 해제 예약
    else CC_SAFE_DELETE(ret); // 초기화 실패 시 메모리 해제
    return ret;
}

// SceneHome 초기화 함수
bool SceneHome::init()
{
    if (!Scene::init()) return false; // 부모 클래스 초기화 실패 시 false 반환

    // 타이틀 이미지 스프라이트 생성 및 설정
    Sprite* img = Sprite::create("res/title_girl.png");
    addChild(img); // 현재 장면에 스프라이트 추가
    img->setOpacity(0); // 투명도 설정 (0: 완전 투명)
    img->runAction(FadeIn::create(0.25)); // 0.25초 동안 페이드 인 애니메이션 실행
    img->setPosition(Vec2(720 / 2, 800)); // 스프라이트 위치 설정

    // 타이틀 텍스트 스프라이트 생성 및 설정
    Sprite* title = Sprite::create("res/title.png");
    addChild(title); // 현재 장면에 타이틀 추가
    title->setPosition(Vec2(720 / 2, -100)); // 시작 위치 설정
    title->runAction(EaseSineOut::create(MoveTo::create(0.25f, Vec2(720 / 2, 600)))); // 0.25초 동안 위치 이동 애니메이션 실행

    // 게임 시작 버튼 생성 및 설정
    addChild(btnStart = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));
    // 게임 종료 버튼 생성 및 설정
    addChild(btnQuit = Button::create("res/btn_normal.png", "res/btn_pressed.png", "res/btn_disabled.png"));

    // 버튼 위치 설정
    btnStart->setPosition(Vec2(720 / 2, 400));
    btnQuit->setPosition(Vec2(720 / 2, 250));

    // 버튼 폰트 이름 설정
    btnStart->setTitleFontName(FONT_NAME);
    btnQuit->setTitleFontName(FONT_NAME);

    // 버튼 폰트 크기 설정
    btnStart->setTitleFontSize(40.0f);
    btnQuit->setTitleFontSize(40.0f);

    // 버튼 텍스트 색상 설정
    btnStart->setTitleColor(Color3B(0, 50, 50));
    btnQuit->setTitleColor(Color3B(0, 50, 50));

    // 버튼 텍스트 설정
    btnStart->setTitleText("Start Game");
    btnQuit->setTitleText("Quit Game");

    Global::getInstance()->stopBackgroundMusic();

    // 게임 시작 버튼 클릭 이벤트 리스너 추가
    btnStart->addClickEventListener([=](Ref* r) {
        auto scene = SceneIngame::create(); // 인게임 장면 생성
        auto transit = TransitionSlideInR::create(0.125f, scene); // 오른쪽에서 슬라이드 인 전환 효과 설정
        Director::getInstance()->replaceScene(transit); // 현재 장면을 인게임 장면으로 교체
        Global::getInstance()->playPop(); // 버튼을 누를때 사운드 재생 
        });

    // 게임 종료 버튼 클릭 이벤트 리스너 추가
    btnQuit->addClickEventListener([=](Ref* r) {
        Global::getInstance()->playPop(); // 버튼을 누를 때 사운드 재생 
        Director::getInstance()->end(); // 애플리케이션 종료
        });

    return true;
}
