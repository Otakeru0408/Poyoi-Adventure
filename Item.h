#pragma once
#include "Actor.h"

class Item :public Actor {
public:
	Item(const TCHAR* path, int whichMap,bool isAppeared);
	void Draw()override;
	void SetAppeared(bool b) { isAppeared = b; }
	bool GetAppeared()const { return isAppeared; }
	void SetClosed(bool b) { isClosed = b; }
	bool GetClosed()const { return isClosed; }
	void SetAdded(bool b) { isAdded = b; }
	bool GetAdded()const { return isAdded; }
private:
	bool isAppeared;
	bool isClosed;
	bool isAdded;
};

/*
Item�N���X�ł̓}�b�v�ɂ�����炸�w�肵���}�b�v�ɕ`�悵�A�X��MinorRoll�̃t���O�Ɉˑ�������邩����Ȃ��������߂�B
�`��֐��ł�isAppeared�ɂ���ĕ`�悷�邩���Ȃ�������
�Ⴆ�΂���L�����ƐڐG���邱�ƂŌ����Ƃ���}�b�v�ɕ`�悳��A���̌��ɐG���ƃ|���C�N�̃A�C�e�����ɒǉ������
�A�C�e�����̓|���C�N�̃N���X��draw���邩�B
*/