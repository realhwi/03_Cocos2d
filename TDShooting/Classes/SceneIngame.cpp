#include "stdafx.h" 
#include "SceneIngame.h" 

SceneIngame* SceneIngame::create()
{
	auto ret = new SceneIngame(); // 새 객체 생성
	if (ret && ret->init()) ret->autorelease(); // 초기화에 성공하면 자동 해제를 설정
	else CC_SAFE_DELETE(ret); // 실패 시 객체 삭제
	return ret; // 생성된 객체 반환
}

bool SceneIngame::init()
{
	if (!Scene::initWithPhysics()) return false; // 물리 엔진을 가진 씬으로 초기화, 실패 시 종료

	auto keybd = EventListenerKeyboard::create(); // 키보드 이벤트 리스너 생성
	// 키 입력을 처리하는 콜백 연결
	keybd->onKeyPressed = std::bind(&SceneIngame::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keybd->onKeyReleased = std::bind(&SceneIngame::onKeyRelesed, this, std::placeholders::_1, std::placeholders::_2);

	getEventDispatcher()->addEventListenerWithSceneGraphPriority(keybd, this); // 이벤트 리스너를 씬에 추가

	schedule(CC_SCHEDULE_SELECTOR(SceneIngame::logic)); // 일정 시간마다 logic 함수 호출

	return true; // 초기화 성공
}

void SceneIngame::onEnter()
{
	Scene::onEnter(); // 부모 클래스의 onEnter 호출

	auto world = getPhysicsWorld(); // 물리 월드(PhysicsWorld) 객체 가져오기
	if (DEBUG_MODE) world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); // 디버그 모드일 때 물리 디버그 설정
	world->setGravity(Vec2::ZERO); // 중력 설정 (여기서는 중력 없음)

	player = Node::create(); // 플레이어 노드 생성
	player->setPosition(Vec2(1280 / 2, 720 / 2)); // 플레이어 위치를 화면 중앙으로 설정

	auto body = PhysicsBody::createBox(Size(75, 75)); // 플레이어의 크기를 설정하고 물리 바디 생성
	body->setRotationEnable(false); // 회전 불가 설정
	body->setCollisionBitmask(0); // 충돌 마스크 설정 (충돌 대상 없음)
	player->addComponent(body); // 플레이어에 물리 바디 추가
	addChild(player); // 플레이어 노드를 씬에 추가
}

void SceneIngame::onKeyPressed(EventKeyboard::KeyCode c, Event* e)
{
	const bool value = true;
	// 키보드 입력에 따라 방향과 발사 여부 설정
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W: up = value; break;
	case EventKeyboard::KeyCode::KEY_S: down = value; break;
	case EventKeyboard::KeyCode::KEY_A: left = value; break;
	case EventKeyboard::KeyCode::KEY_D: right = value; break;
	case EventKeyboard::KeyCode::KEY_J: fire = value; break;
	}
}

void SceneIngame::onKeyRelesed(EventKeyboard::KeyCode c, Event* e)
{
	const bool value = false;
	// 키보드 입력이 해제되면 방향과 발사 상태 해제
	switch (c) {
	case EventKeyboard::KeyCode::KEY_W: up = value; break;
	case EventKeyboard::KeyCode::KEY_S: down = value; break;
	case EventKeyboard::KeyCode::KEY_A: left = value; break;
	case EventKeyboard::KeyCode::KEY_D: right = value; break;
	case EventKeyboard::KeyCode::KEY_J: fire = value; break;
	}
}

void SceneIngame::logic(float dt)
{
	if (!player) return; // 플레이어가 없으면 종료

	Vec2 pos = player->getPosition(); // 현재 위치 가져오기
	// 방향키에 따라 위치 변경
	if (up) pos += Vec2(0, dt * PLAYER_MOVEMENT_SPEED);
	if (down) pos -= Vec2(0, dt * PLAYER_MOVEMENT_SPEED);
	if (left) pos -= Vec2(dt * PLAYER_MOVEMENT_SPEED, 0);
	if (right) pos += Vec2(dt * PLAYER_MOVEMENT_SPEED, 0);

	player->setPosition(pos); // 새로운 위치 설정

	// 발사 상태일 때 총알 생성
	if (fire) {
		auto bullet = Node::create(); // 총알 노드 생성
		auto body = PhysicsBody::createCircle(5); // 원형의 물리 바디 생성
		body->setCollisionBitmask(0); // 충돌 마스크 설정 (충돌 대상 없음)
		bullet->setPosition(player->getPosition()); // 총알 위치를 플레이어 위치로 설정
		bullet->addComponent(body); // 총알에 물리 바디 추가
		body->setVelocity(Vec2(0, 1000)); // 총알의 속도 설정 (위 방향)
		bullet->runAction(Sequence::create(DelayTime::create(1.0f), RemoveSelf::create(), nullptr)); // 1초 후 총알을 제거
		addChild(bullet); // 총알을 씬에 추가
	}
}
