#ifndef __SCENE_INGAME_H__ 
#define __SCENE_INGAME_H__

#include "stdafx.h" 

class SceneIngame : public Scene {
private:
	Node* player = nullptr; // 플레이어 노드, 초기 위치와 크기를 설정함
	bool up = false, down = false, left = false, right = false; // W, A, S, D 키 입력에 따른 이동 방향을 나타내는 변수
	bool fire = false; // 'J' 키 입력에 따른 발사 여부를 나타내는 변수
public:
	static SceneIngame* create(); // SceneIngame 객체를 생성하는 정적 함수
	virtual bool init() override; // 씬 초기화 함수
	virtual void onEnter() override; // 씬이 활성화될 때 호출되는 함수

	void onKeyPressed(EventKeyboard::KeyCode c, Event* e); // 키가 눌렸을 때 호출되는 함수
	void onKeyRelesed(EventKeyboard::KeyCode c, Event* e); // 키가 해제되었을 때 호출되는 함수

	void logic(float dt); // 매 프레임마다 실행되는 로직 함수, 움직임 및 발사 처리
};

#endif 
