#ifndef __BULLET_H__
#define __BULLET_H_

#include "stdafx.h"

class Bullet : public Node {
private:
	Sprite* spr;
	PhysicsBody* body;
public:
	static Bullet* create(int bitmask, int tag);
	virtual bool init(int bitmask, int tag);

	PhysicsBody* getBody();
};

#endif 
