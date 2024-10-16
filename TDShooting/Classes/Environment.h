#ifndef  __ENVIRONMENT_H__ 
#define  __ENVIRONMENT_H__

#define DEBUG_MODE true // 디버그 모드 설정, true일 때 디버그 정보 출력

#define PLAYER_MOVEMENT_SPEED 400 // 플레이어 이동 속도 상수
#define ENEMY_BULLET_SPEED 500 // 적 이동 속도 

// 011(2)
#define PLAYER_MASK 0x00000003

// 110(2)
#define ENEMY_MASK 0x00000006

// 100(2)
#define PLAYER_BULLET_MASK 0x00000004

// 001(2)
#define ENEMY_BULLET_MASK 0x00000001

#define TAG_PLAYER 1
#define TAG_ENEMY 2
#define TAG_PLAYER_BULLET 3
#define TAG_ENEMY_BULLET 4

#endif // 헤더 가드 종료
