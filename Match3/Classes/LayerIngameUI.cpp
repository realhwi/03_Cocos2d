#include "stdafx.h" // 기본적인 헤더 파일 포함
#include "LayerIngameUI.h" // 인게임 UI 클래스 헤더 파일 포함

// LayerIngameUI 객체 생성 함수
LayerIngameUI* LayerIngameUI::create()
{
	auto ret = new LayerIngameUI();
	if (ret && ret->init()) ret->autorelease(); // 초기화 성공 시 자동 해제 예약
	else CC_SAFE_DELETE(ret); // 초기화 실패 시 메모리 해제

	return ret;
}

// LayerIngameUI 초기화 함수
bool LayerIngameUI::init()
{
	if (!Node::init()) return false; // 부모 클래스 초기화 실패 시 false 반환

	// 점수 표시 레이블 추가
	addChild(IbScore = Label::createWithTTF("", FONT_NAME, 48.0f));
	IbScore->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
	IbScore->setPosition(Vec2(30, 1280 - 70));

	// 일시정지 버튼 추가
	addChild(btnPause = Button::create("res/btn_pause_normal.png", "res/btn_pause_pressed.png", "res/btn_pause_disabled"));
	btnPause->setPosition(Vec2(720 - 70, 1280 - 70));

	// 일시정지 패널 설정
	const Size PANEL_SIZE(600, 400);
	const float SPACING = 170;
	const float BUTTON_BOTTOM_SPACING = 100;

	// 커튼 효과 추가 (일시정지 시 사용)
	addChild(dnCurtain = DrawNode::create());
	dnCurtain->drawSolidRect(Vec2::ZERO, Vec2(720, 1280), Color4F(0, 0, 0, 0.8));

	// 일시정지 패널 추가
	addChild(pausePanel = Scale9Sprite::create("res/panel.png"));
	pausePanel->setPosition(Vec2(720 / 2, 1280 / 2));
	pausePanel->setScale9Enabled(true);
	pausePanel->setContentSize(Size(600, 400));

	// 패널에 버튼들 추가 (재시작, 홈, 재개 버튼)
	pausePanel->addChild(btnRestart = Button::create("res/btn_restart_normal.png", "res/btn_restart_pressed.png", "res/btn_restart_disabled.png"));
	pausePanel->addChild(btnHome = Button::create("res/btn_home_normal.png", "res/btn_home_pressed.png", "res/btn_home_disabled.png"));
	pausePanel->addChild(btnResume = Button::create("res/btn_play_normal.png", "res/btn_play_pressed.png", "res/btn_play_disabled"));

	// 버튼 위치 설정
	btnResume->setPosition(Vec2(PANEL_SIZE.width / 2, BUTTON_BOTTOM_SPACING));
	btnHome->setPosition(Vec2(PANEL_SIZE.width / 2 - SPACING, BUTTON_BOTTOM_SPACING));
	btnRestart->setPosition(Vec2(PANEL_SIZE.width / 2 + SPACING, BUTTON_BOTTOM_SPACING));

	// 일시정지 상태 표시 레이블 추가
	Label* IbPaused = Label::createWithTTF("PAUSED!!!", FONT_NAME, 64.0f);
	pausePanel->addChild(IbPaused);
	IbPaused->setColor(Color3B(0, 0, 0));
	IbPaused->setPosition(Vec2(PANEL_SIZE.width / 2, 300));

	setScore(0); // 초기 점수 설정
	hidePausePanel(); // 일시정지 패널 숨기기

	return true;
}

// 점수 설정 함수
void LayerIngameUI::setScore(long long score)
{
	sprintf(scoreString, "Score: %Id", score); // 점수를 문자열로 변환
	IbScore->setString(scoreString); // 레이블에 점수 설정
}

// 일시정지 패널 표시 함수
void LayerIngameUI::showPausePanel()
{
	pausePanel->setVisible(true); // 일시정지 패널 보이기
	dnCurtain->setVisible(true); // 커튼 보이기

	dnCurtain->setOpacity(0); // 커튼 투명도 설정
	dnCurtain->runAction(FadeIn::create(0.125)); // 커튼 페이드 인 애니메이션 실행

	auto pos = pausePanel->getPosition();
	pausePanel->setPosition(pos - Vec2(0, 1000)); // 패널을 아래로 이동
	pausePanel->runAction(EaseExponentialInOut::create(MoveTo::create(0.125f, pos))); // 패널을 원래 위치로 이동
}

// 일시정지 패널 숨기기 함수
void LayerIngameUI::hidePausePanel()
{
	auto pos = pausePanel->getPosition();
	pausePanel->runAction(Sequence::create(
		EaseExponentialOut::create(MoveTo::create(0.25f, pos - Vec2(0, 1000))), // 패널을 아래로 이동시키는 애니메이션 실행
		CallFunc::create([=]() {
			pausePanel->setPosition(pos); // 패널 위치 복원
			pausePanel->setVisible(false); // 패널 숨기기
			dnCurtain->setVisible(false); // 커튼 숨기기
			}),
		nullptr));
}