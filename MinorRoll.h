#pragma once
#include "Actor.h"
#include <vector>
#include <string>

class MinorRoll :public Actor {
public:
	MinorRoll(class Tile* tile, const TCHAR* path,std::string name,int xNum,int yNum, int posX, int posY);
	MinorRoll(class Tile* tile, const TCHAR* path, std::string name, int posX, int posY);
	~MinorRoll();
	void Update(float deltaTime)override;
	void Draw()override;
	void isNear();
	void SetSerif(int num, int pos = 0);		//�Z���t�͂��ׂĔz��ɕۑ����Ă����A�e�Őݒ肷��
	void SetSerif(std::string _serif, int pos = 0);
	void SetAppeared(bool b) { isAppeared = b;}
	bool GetAppeared()const { return isAppeared; }
	void SetClosed(bool b) { isClosed = b; }
	bool GetClosed()const { return isClosed; }
	void SetOverlaped(bool b) { isOverlaped = b; }
	bool GetOverlaped()const { return isOverlaped; }
	void SetAnimated(bool b) { isAnimated = b; }
	bool GetAnimated()const { return isAnimated; }
private:
	class Tile* mTile; 
	int image2[10];
	std::string name;		//tile�N���X�Ŏ��ʂ��邽�߂̖��O

	float mCurrFrame;
	float mAnimFPS;
	std::vector<int> mImages;
	std::vector<std::string> serifs = { "��ρI���̃|���~ �����ꂽ�I","�������@�ł��`�I","�|���~�Ɖ������@��ɍs�����I","�����ā`�I","","���ւցA����̃t�B�A���Z"
	,"���肪�Ɓ[�I","�ȂɁ[�[�I�H"};
	std::string serif;
	bool isOverlaped;
	int serifPos;
	bool isClosed;	//�L��������x�ڐG������ω����N�������߂̃t���O
	bool isAppeared;	//�L������\�����邩�ǂ���
	bool isAnimated;	//���C�x���g�V�[���ɂ��邩�ǂ���
};