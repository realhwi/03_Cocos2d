#include "stdafx.h"
#include "SceneIngame.h"
#include "SceneHome.h"

//x,y 위치에 블록을 생성하는 함수 
void SceneIngame::createBlock(int x, int y, int type)
{
	// TextureCache에서 이미지를 가져옴 (로드된 이미지)
	auto cache = Director::getInstance()->getTextureCache();
	// 이미지를 사용해 새로운 Sprite 생성 
	auto spr = Sprite::createWithTexture(
		cache->getTextureForKey("res/match3_tiles_px.png"), 
		Rect(0+(type*40), 0, 40, 40));
	spr->setScale(2); // block 크기 두배로 
	addChild(spr); // 현재 장면에 추가 (화면에 보이게)
	// 블록 데이터와 블록 그림을 x,y 위치에 설정
	setBlockData(x,y,type);	
	setBlockSprite(x,y,spr);
}

// x,y 위치에 블록 상태 확인 (데이터 반환) 
int SceneIngame::getBlockData(int x, int y)
{
	return blockData[y][x]; 
}

// x,y위치에 블록 상태를 설정 (데이터 지정) 
void SceneIngame::setBlockData(int x, int y, int type)
{
	blockData[y][x] = type;
}

// x,y위치의 블록 Sprite(그림)를 가져오는 함수
Sprite* SceneIngame::getBlockSprite(int x, int y)
{
	return blockSprite[y][x];
}

// x,y 위치의 블록 Sprite(그림)을 설정하는 함수 
void SceneIngame::setBlockSprite(int x, int y, Sprite* s)
{
	blockSprite[y][x] = s;
}

// x,y 위치의 블록을 제거하는 함수 
void SceneIngame::destroyBlock(int x, int y)
{
	if(blockData[y][x]){ // 블록 데이터가 존재한다면 (0이 아니라면) 
		state = GameState::BLOCK_MOVING; // 상태 추가 : 
		// 블록이 깜빡이면서 사라지게 
		blockSprite[y][x]->runAction(Sequence::create(
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			FadeOut::create(0.125f),
			FadeIn::create(0.125f),
			Spawn::create(ScaleTo::create(0.125f,0.0),FadeOut::create(0.125f),nullptr),
			RemoveSelf::create(), //블록을 제거 
			nullptr
			));
		blockSprite[y][x] = nullptr; // 블록 포인터를 비움
		blockData[y][x] = 0; // 데이터 값을 0으로 설정해서 빈 공간으로 표시 
		// 일정 시간 후 dropBlock 호출로 블록을 떨어뜨림
		this->runAction(Sequence::create(
			DelayTime::create(0.625f),
			CallFunc::create([=](){dropBlock(x);}),
			nullptr
			));
	}
}

//게임 화면 좌표 -> 블록 배열 좌표
Vec2 SceneIngame::convertGameCoordToBlockCoord(Vec2 gameCoord)
{
	Vec2 blockOrigin = BLOCK_OFFSET
		- Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
		// 각 블록 크기의 절반을 더해 중심을 맞춤
		// blockOrigin = 블록 배열의 시작 위치 
	Vec2 delta = gameCoord - blockOrigin; 
	// delta계산 : 게임 좌표에서 시작 위치를 빼서 블록 배열 상대 위치 구하는 공식 
	Vec2 pos = Vec2((int)(delta.x / BLOCK_WIDTH + 0.5), (int)(delta.y / BLOCK_HEIGHT + 0.5));
	 // pos : 상대 위치를 블록 크기로 나눠 몇 번째 블록에 있는지 알아냄 
	// -> 게임 좌표를 블록 배열 좌표로 변환 

	return pos;
}

// 블록 배열 좌표 -> 게임 화면 좌표 
Vec2 SceneIngame::convertBlockCoordToGameCoord(Vec2 blockCoord)
{
	//blockOrigin : 블록 배열의 시작 위치를 계산하는 변수
	Vec2 blockOrigin = BLOCK_OFFSET
		- Vec2((BLOCK_HORIZONTAL * BLOCK_WIDTH) / 2, (BLOCK_VERTICAL * BLOCK_HEIGHT) / 2)
		+ Vec2(BLOCK_WIDTH, BLOCK_HEIGHT) / 2;
		// 각 블록 크기의 절반을 더해 중심 설정 

	return blockOrigin + Vec2(BLOCK_WIDTH * blockCoord.x, BLOCK_HEIGHT * blockCoord.y);
	// 몇 번째 블록인지 게임 화면의 좌표로 변환
}

int SceneIngame::findEmptyBlockYIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if(getBlockData(x,i)==0) return i;
	}
	return -1;
}

int SceneIngame::findFiledBlockIndex(int x, int y)
{
	for (int i = y; i < BLOCK_VERTICAL; i++) {
		if(getBlockData(x,i)!=0) return i;
	}
	return -1;
}

void SceneIngame::dropBlock(int x)
{
	bool isDrop = false; // 블록이 떨어졌는지 체크 
	for (int i = 0; i < BLOCK_VERTICAL; i++) {
		int empty_y = findEmptyBlockYIndex(x, i);
		if (empty_y == -1) continue;
		int filed_y = findFiledBlockIndex(x, empty_y + 1);
		if (filed_y == -1) { // 새로운 블록 생성 
			createBlock(x,empty_y,rand()%BLOCK_VAR+1);
			// 블록이 화면 밖에서 시작해 자연스럽게 떨어지도록 위치를 설정 
			blockSprite[empty_y][x]->setPosition(convertBlockCoordToGameCoord(Vec2(x,BLOCK_VERTICAL + 1)));
			blockSprite[empty_y][x]->runAction(MoveTo::create(0.125f,convertBlockCoordToGameCoord(Vec2(x,empty_y))));
			continue;
		}

		// 블록 데이터 교환
		{
			int a = getBlockData(x, empty_y);
			int b = getBlockData(x, filed_y);
			SWAP(int, a, b);
			setBlockData(x, empty_y, a); // 교환된 값을 원본에 반영
			setBlockData(x, filed_y, b);
		}

		// 블록 스프라이트 교환
		{
			Sprite* a = getBlockSprite(x, empty_y);
			Sprite* b = getBlockSprite(x, filed_y);
			SWAP(Sprite*, a, b);
			setBlockSprite(x, empty_y, a); // 교환된 값을 원본에 반영
			setBlockSprite(x, filed_y, b);

			a->stopAllActions();//기존 액션 정지하여 중복 방지 
			// 더 자연스럽게 내려오게 runAction의 MoveTo사용 
			a->runAction(MoveTo::create(0.125f,convertBlockCoordToGameCoord(Vec2(x,empty_y))));
		}
		isDrop = true; 
	}

	if (isDrop) {
		for (int i = 0; i < BLOCK_VERTICAL; i++) {
			judgeMatch(x,i);// 3개 매치 찾기 
		}
	}
	else {
		state = GameState::PLAYING;
	}
	// alignBlockSprite();
}

void SceneIngame::stackPush(Vec2 value)
{
	if(judgeData[(int)value.y][(int)value.x]!=0) return;
	judgeStack[judgeStackCount++] = value;
	judgeData[(int)value.y][(int)value.x] = 1;

}

Vec2 SceneIngame::stackPop()
{
	auto ret = judgeStack[--judgeStackCount];
	judgeData[(int)ret.y][(int)ret.x] = 0;
	return ret;
}

void SceneIngame::stackEmpty()
{
	judgeStackCount = 0;
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			judgeData[k][i] = 0;
		}
	}
}

bool SceneIngame::stackFind(Vec2 value)
{
	return judgeData[(int)value.y][(int)value.x] == 1;
}

void SceneIngame::judgeMatch(int x, int y)
{
	int blockData = getBlockData(x,y);

	if(blockData == 0) return;

	stackPush(Vec2(x,y));
	int push_count = 0;

	for (int i = 0; i < 4; i++) {
		int next_x = x;
		int next_y = y;
		int inc_x;
		int inc_y;

		switch (i) {
		case 0: inc_x = 1; inc_y = 0; push_count = 0; break;
		case 1: inc_x = -1; inc_y = 0; break;
		case 2: inc_x = 0; inc_y = 1; push_count = 0; break;
		case 3: inc_x = 0; inc_y = -1; break;
		}
		while (true) {
			next_x += inc_x;
			next_y += inc_y;
			if(next_x < 0 || next_x >= BLOCK_HORIZONTAL) break;
			if(next_y < 0 || next_y >= BLOCK_VERTICAL) break; 
			if (getBlockData(next_x, next_y) == blockData) {
				stackPush(Vec2(next_x,next_y));
				push_count++;
			}
			else break; 
		}
		if(i%2==0) continue;
		if (push_count < 2) {
			for (int k = 0; k < push_count; k++) {
				stackPop();
			}
		}
	}
	// 스택에 쌓인 블록이 1개 이상일 때 매치 처리
	if (judgeStackCount > 1) {
		Global::getInstance()->addScore(judgeStackCount * 10); // 매치된 블록 수에 따라 점수 추가
		ui->setScore(Global::getInstance()->getScore()); // UI에 점수 업데이트
		while (judgeStackCount > 0) {
			Vec2 p = stackPop(); // 스택에서 블록 좌표 꺼내기
			destroyBlock(p.x, p.y); // 블록 제거
		}
	}
	else {
		state = GameState::PLAYING; // 매치가 없으면 게임 상태를 PLAYING으로 설정
	}
	stackEmpty(); // 스택 비우기
}

// 장면을 생성하는 함수 
SceneIngame* SceneIngame::create()
{
	auto ret = new SceneIngame();
	if(ret && ret->init()) ret->autorelease();
	// 성공하면 메모리 해제 예약
	else CC_SAFE_DELETE(ret); 
	// 실패하면 메모리에서 제거 
	return ret;
}

// 장면을 초기화하는 함수 
bool SceneIngame::init()
{
	if(!Scene::init()) return false; 

	srand(time(0)); //랜덤 함수 생성 

	// 이미지를 미리 로드해서 캐시에 저장해둠 (나중에 빠르게 불러올 수 있게)  
	Director::getInstance()->getTextureCache()->addImage("res/match3_tiles_px.png");

	auto touch = EventListenerTouchOneByOne::create();
	touch->onTouchBegan = std::bind(&SceneIngame::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchMoved = std::bind(&SceneIngame::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchEnded = std::bind(&SceneIngame::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
	touch->onTouchCancelled = touch->onTouchEnded;

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(touch,this);

	return true;
}

// 장면이 화면에 나타날때 호출되는 함수 
void SceneIngame::onEnter()
{
	Scene::onEnter();
	
	this->initUI(); // UI 초기화
	this->initGame(); // 게임 초기화 
	this->startGame(); // 게임 시작 
}

void SceneIngame::initUI()
{
	// 인게임 UI 초기화하고 UI 레이어를 현재 장면에 추가
	addChild(ui = LayerIngameUI::create());
	ui->setLocalZOrder(1); // UI 레이어를 다른 요소보다 위에 표시

	// 일시정지 버튼 클릭했을 때 처리
	ui->btnPause->addClickEventListener([=](Ref* r) {
		if (state == GameState::PLAYING) { // 게임이 진행 중일 때만 일시정지 가능
			ui->showPausePanel(); // 일시정지 패널 보여주기
			state = GameState::PAUSED; // 게임 상태를 일시정지로 변경
		}
		});

	// 재개 버튼 클릭했을 때 처리
	ui->btnResume->addClickEventListener([=](Ref* r) {
		if (state == GameState::PAUSED) { // 게임이 일시정지 상태일 때만 재개 가능
			ui->hidePausePanel(); // 일시정지 패널 숨기기
			state = GameState::PLAYING; // 게임 상태를 다시 플레이로 변경
		}
		});

	// 재시작 버튼 클릭했을 때 처리
	ui->btnRestart->addClickEventListener([=](Ref* r) {
		if (state == GameState::PAUSED) { // 게임이 일시정지 상태일 때만 재시작 가능
			ui->hidePausePanel(); // 일시정지 패널 숨기기
			ui->setScore(0); // 점수 초기화
			this->destroyGame(); // 기존 게임 상태 초기화
			this->initGame(); // 게임 재초기화
			this->startGame(); // 게임 시작
			state = GameState::PLAYING; // 게임 상태를 플레이로 변경
		}
		});

	// 홈 버튼 클릭했을 때 처리
	ui->btnHome->addClickEventListener([=](Ref* r) {
		if (state == GameState::PAUSED) { // 게임이 일시정지 상태일 때만 홈으로 이동 가능
			auto scene = SceneHome::create(); // 홈 화면 장면 생성
			auto transit = TransitionSlideInL::create(0.125f, scene); // 왼쪽에서 슬라이드 인 전환 효과 설정
			Director::getInstance()->replaceScene(transit); // 현재 장면을 홈 화면으로 교체
		}
		});
}

void SceneIngame::destroyUI()
{
}


// 게임을 종료할 때 블록과 점수를 초기화하는 함수
void SceneIngame::destroyGame()
{
	Global::getInstance()->setScore(0); // 게임 종료 시 점수 초기화
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) { // 가로 방향으로 반복
		for (int k = 0; k < BLOCK_VERTICAL; k++) { // 세로 방향으로 반복
			setBlockData(i, k, 0); // 블록 데이터를 0으로 초기화
			getBlockSprite(i, k)->removeFromParent(); // 화면에서 블록 제거
			setBlockSprite(i, k, nullptr); // 블록 스프라이트 포인터 초기화
		}
	}
	this->alignBlockSprite(); // 블록들을 화면에 맞추기
}


// 블록 데이터를 화면에 정갈하게 맞춰주는 함수 
void SceneIngame::alignBlockSprite()
{
	// 가로 방향으로 블록을 하나씩 반복 
	for (int i = 0; i < BLOCK_HORIZONTAL; i++) {
		// 세로 방향으로 블록을 하나씩 반복 
		for (int k = 0; k < BLOCK_VERTICAL; k++) {
			// i,k 위치에 있는 블록의 Sprite를 가져옴 
			auto s = getBlockSprite(i, k);

			// 블록이 있는경우, 해당 블록을 올바른 위치에 맞춰줌
			// BlockCoordToGameCoor를 통해 블록 좌표를 화면 좌표로 변환하여 위치 
			if(s!=nullptr) s->setPosition(convertBlockCoordToGameCoord(Vec2(i,k)));
		}
	}
}

bool SceneIngame::onTouchBegan(Touch* t, Event* e)
{
	if (state == GameState::PAUSED) {
		// 일시정지 상태에서는 터치 입력 무시
		return true;
	}

	Vec2 p = convertGameCoordToBlockCoord(t->getLocation());

	if (state == GameState::PLAYING) {
		// 원하는 부분만 클릭하기 (블록 있는 부분) 
		if (p.x >= BLOCK_HORIZONTAL || p.x < 0) return true;
		if (p.y >= BLOCK_VERTICAL || p.y < 0) return true;

		CCLOG("Converted Block Coord: (%f, %f)", p.x, p.y); // 마우스 클릭 좌표 출력
		destroyBlock(p.x, p.y); // 클릭한 부분 블록 삭제되게 
	}

	return true;
}

void SceneIngame::onTouchMoved(Touch* t, Event* e)
{
}

void SceneIngame::onTouchEnded(Touch* t, Event* e)
{
}

void SceneIngame::startGame()
{
}

void SceneIngame::pauseGame()
{
}

void SceneIngame::winGame()
{
}

void SceneIngame::loseGame()
{
}
