#ifndef __ENVIRONMENT_H__
#define __ENVIRONMENT_H__

#include "stdafx.h"

// 게임 상태를 나타내는 열거형 - 게임이 현재 어떤 상태인지 관리 (플레이 중, 일시정지, 승리, 패배, 블록 이동 중 등)
enum class GameState {
	PLAYING, PAUSED, WIN, LOSE, BLOCK_MOVING // 사용자 입력 방지 추가 
};

// 게임 보드의 가로, 세로 블록 개수 설정
#define BLOCK_HORIZONTAL 7
#define BLOCK_VERTICAL 9 

// 블록 위치를 계산할 때 사용할 기본 오프셋 (화면 중앙 기준)
#define BLOCK_OFFSET Vec2(720 / 2, 1280 / 2)  

// 각 블록의 너비와 높이 설정
#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 80

// 블록 종류 개수 설정 (랜덤으로 생성할 블록의 종류)
#define BLOCK_VAR 4 // 블록 개수 지정 

// 폰트 이름 정의 (게임 내 텍스트에 사용할 폰트)
#define FONT_NAME "fonts/SDSamliphopangcheTTFBasic.ttf"

// 두 변수의 값을 교환하는 매크로 함수
#define SWAP(TYPE,A,B){TYPE t = A; A = B; B = t;}

// Global 클래스 - 게임 점수를 관리하는 싱글톤 클래스
class Global {
private:
	Global() {} // private 생성자 - 외부에서 객체 생성을 막기 위해 사용
	long long score = 0; // 게임 점수를 저장하는 변수
	int backgroundMusicId = -1;

public:
	static Global* getInstance(); // Global 클래스의 인스턴스를 반환하는 정적 함수 (싱글톤 패턴 구현)

	void setScore(long long value); // 점수를 설정하는 함수
	long long getScore(); // 현재 점수를 반환하는 함수
	void addScore(long long value); // 현재 점수에 값을 더하는 함수

	void playPop();
	void playBackgroundMusic();
	void stopBackgroundMusic();
};

#endif 