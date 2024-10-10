#ifndef __LAYER_INGAME_UI_H__
#define __LAYER_INGAME_UI_H__

#include "stdafx.h" // 기본적인 헤더 파일 포함
#include "Environment.h" // 환경 설정 관련 헤더 파일 포함

// 인게임 UI 레이어 클래스 - 게임 중 사용자 인터페이스를 관리
class LayerIngameUI : public Node {
private:
	long long score; // 현재 게임 점수를 저장하는 변수
	char scoreString[128]; // 점수를 문자열로 저장하기 위한 배열
public:
	Label* IbScore; // 점수를 표시하는 레이블
	Button* btnPause; // 일시정지 버튼
	Scale9Sprite* pausePanel; // 일시정지 패널
	Button* btnResume; // 재개 버튼
	Button* btnRestart; // 재시작 버튼
	Button* btnHome; // 홈으로 돌아가는 버튼
	DrawNode* dnCurtain; // 화면을 덮는 커튼 역할 (일시정지 시 사용)

	static LayerIngameUI* create(); // LayerIngameUI 객체를 생성하는 함수
	virtual bool init() override; // 초기화 함수

	void setScore(long long score); // 점수를 설정하는 함수
	long long getScore; // 현재 점수를 반환하는 함수

	void showPausePanel(); // 일시정지 패널을 표시하는 함수
	void hidePausePanel(); // 일시정지 패널을 숨기는 함수

};
#endif