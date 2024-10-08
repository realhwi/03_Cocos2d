#ifndef __SCENE_INGAME_H__
#define __SCENE_INGAME_H__

#include "stdafx.h"
#include "Environment.h"

class SceneIngame: public Scene{
private:
	GameState state; // 게임 상태 선언 

	// blockData는 0 값일 경우 비어있는 블록, 0이 아닐 경우, 양수 값일 경우 블록
	// 이차원 배열을 만들어 블록의 위치를 관리 
	int blockData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	// blockSprite는 nullptr일 경우 비어있음 
	// 블록이 있다면 Sprite로 블록을 화면에 표시함 
	Sprite* blockSprite[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	// 유니트 스텍을 위한 자료구조 
	Vec2 judgeStack[128];
	// 스택에 있는 자료의 수 
	int judgeStackCount = 8;
	// 0이라면 스택에 자료가 없음, 그게 아니라면 자료가 있음 
	int judgeData[BLOCK_VERTICAL][BLOCK_HORIZONTAL];

	// 새로운 블록을 생성하는 함수 
	void createBlock(int x,int y, int type);

	// x,y위치에 있는 블록이 어떤 상태인지 (읽기) 
	int getBlockData(int x, int y);

	// x,y위치에 블록 상태를 type을 받아서 지정 (쓰기) 
	void setBlockData(int x, int y,int type);

	// x,y 위치에 블록이 있으면 블록의 그림을 가져옴 
	Sprite* getBlockSprite(int x, int y);

	// x,y 위치에 블록의 그림을 설정 
	void setBlockSprite(int x, int y, Sprite* s);

	//x,y의 블록을 제거하는 함수 
	void destroyBlock(int x,int y);

	// 게임 좌표를 블록 좌표로 변환하는 함수 
	// gameCoord : 게임 내 실제 좌표 (픽셀단위)
	// 반환 값 : 해당 좌표에 대응하는 블록 위치 좌표 
	Vec2 convertGameCoordToBlockCoord(Vec2 gameCoord);
	// 블록 좌표를 게임 좌표로 변환하는 함수 
	// blockCoord : 블록의 위치 좌표 (블록단위)
	// 반환 값 : 해당 게임 내 실제 좌표에서 어디에 해당하는지 반환 
	Vec2 convertBlockCoordToGameCoord(Vec2 blockCoord);

	// 아래에서부터 찾아 올라가면서 비어있는 블록을 찾고
	// -1이 리턴이 되면 비어있는 블록이 없다는 뜻이다. 
	int findEmptyBlockYIndex(int x, int y); 

	// y위치부터 찾아올라가면서 비어있지 않은 블록을 찾고
	// -1이 리턴이 되면 비어있지 않은 블록이 없다는 뜻이다. 
	int findFiledBlockIndex(int x,int y); 

	// 블록을 떨어뜨리는 함수 
	void dropBlock(int x);

	void stackPush(Vec2 value); //stack에 자료를 집어넣는 함수
	Vec2 stackPop(); // 스택에서 자료를 빼오는 함수
	void stackEmpty(); // 스택을 비워주는 역할
	bool stackFind(Vec2 value); // 스택에 자료가 있는지 판단 
	void judgeMatch(int x, int y); // 3매치가 이루어져있는지 판단 

	
public:
	static SceneIngame* create();
	virtual bool init() override;
	virtual void onEnter() override; 
	// onEnter 노드가 어떤 상위 노드에 붙어 실행 될 때 자동 실행되는 콜백 함수 

	void initUI(); // ui를 만드는 함수 
	void initGame(); // 게임을 최초 초기화 하는 함수 
	void destroyUI(); // ui를 삭제하는 함수 
	void destroyGame(); // 게임의 스프라이트 같은 것들을 삭제하는 함수 

	// 모든 블록의 Sprite를 정찰하는 함수
	// 블록 Sprite들이 화면에서 올바른 위치에 정렬되도록 조정 
	void alignBlockSprite();

	bool onTouchBegan(Touch* t, Event* e); // 터치 시작
	void onTouchMoved(Touch* t, Event* e); // 터치 이동 
	void onTouchEnded(Touch* t, Event* e); // 터치 끝날때 

	void startGame(); //시작
	void pauseGame(); //일시정지
	void winGame(); //이겼을 때
	void loseGame(); //졌을 때
};


#endif 
