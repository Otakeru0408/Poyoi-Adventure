#pragma once
#include <DxLib.h>
#include <stdint.h>
#include<queue>
#include <utility>

class Actor {
public:
	enum DamageType {
		PHYSICS,
		MAGICS,
	};

	Actor();
	virtual void Update(float deltaTime){}
	virtual void Draw(){}
	virtual void ProcessInput(uint32_t* key){}

	void SavePos(int flag);
	void SavePos(int flag,int px,int py);
	int Following(int _x, int _y);
	void clearPos();
	int culcDist(int x1, int y1, int x2, int y2) {
		return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	float x;
	float y;
	int w;
	int h;
	int image;
	bool isFirst;	//tilemgr�ł̐ڐG����Ɏg�p����B��x�ڐG���Ă��邩�ǂ���
	int HP;
	int maxHP;
	int attack;		//�U����
	DamageType attackType;
	int damage;		//�󂯂��_���[�W��
	DamageType damageType;	//�󂯂��_���[�W�̃^�C�v	0:���� 1:���@
	bool isGuard;
	bool isFollowed;	//�L���������Ă��邩�ǂ���
protected:
	int _x;					//�ʃ}�b�v�ɍs�����Ƃ��Ɍ��̍��W��ۑ����Ă����p
	int _y;
	std::queue<std::pair<int, int>> _pos;
};

/*
�ڐG����ɂ��Ă̋@�\������������

��̗�FActor1��Actor2��x,y,w,h�����ꂼ��󂯎���Ĕ͈͂��ڐG���Ă��邩���m�F����B
�������ڐG���Ă�����P��Ԃ��B���������łȂ���΂O��Ԃ��B

���F���Actor���ڐG�����ۉ����𓮍삳���邪�A���̌����͐ڐG�����܂܂Ȃ̂Ŕ����������Ă��܂��B
�t���O��p���ĉ����ł��邪�ώG�ɂȂ�B

�����ł��̋@�\���J�v�Z�������Ċy�Ɏg����悤�ɂ������B

�g�����Ƃ��ẮA
if(intersect(x1,y1,w1,h1,x2,y2,w2,h2)){������͓�̗̈悪���߂ĐڐG�����ꍇ�̂�true��Ԃ�
	//�Ȃ񂩏���������
}

bool Intersect(class Actor* a,x2,y2,w2,h2){
	if(isHit2(*a.x,*a.y,*a.w,*a.h,x2,y2,w2,h2)&&!*a.isFirst){
		isFirst=true;
		return true;
	}else if(!isHit2(x1,y1,w1,h1,x2,y2,w2,h2)&&isFirst){
		isFirst=false;
	}
	return false;
}
*/