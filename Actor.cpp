#include "Actor.h"
#include "Game.h"

Actor::Actor()
	:x(0)
	, y(0)
	, w(0)
	, h(0)
	, image(0)
	, isFirst(false)
	, HP(100)
	, maxHP(100)
	, attack(10)
	,attackType(DamageType::PHYSICS)
	, damage(0)
	, damageType(DamageType::PHYSICS)
	, isGuard(false)
	, isFollowed(false)
{

}

void Actor::SavePos(int flag) {
	if (flag == 0) {
		_x = x;
		_y = y;
	}
	else if (flag == 1) {
		x = _x;
		y = _y;
	}
}

void Actor::SavePos(int flag, int px, int py) {
	if (flag == 0) {
		_x = px;
		_y = py;
	}
	else if (flag == 1) {
		x = _x;
		y = _y;
	}
}

int Actor::Following(int _x, int _y) {
	std::pair<int, int> p(_x, _y);
	_pos.push(p);
	if (_pos.size() == 10) {
		p = _pos.front();
		if (culcDist(_x, _y, p.first, p.second)>=30*30) {		//ìÒì_ä‘ÇÃãóó£ÇåvéZÇµÅA30à»è„Ç»ÇÁë„ì¸Ç∑ÇÈ
			x = p.first;
			y = p.second;
		}
		_pos.pop();
		return culcDist(_x, _y, p.first, p.second);
	}
}

void Actor::clearPos() {
	std::queue<std::pair<int, int>> _q;
	std::swap(_pos, _q);
}
