#pragma once
#include "Actor.h"
#include <vector>
#include <string>
#include <map>

struct events {
	std::string name;
	int x;
	int y;
	int time;
	std::string serif;
};

class Tile{		
public:
	enum eventState {
		START,
		PLAYING,
		END,
		NONE,
	};
	Tile(class TileMgr* _tNgr,std::string filepath);
	void Draw();
	void UpdateAll(float deltaTime);
	std::vector<std::string> Split(std::string& input, char delimiter);
	bool CheckTile(int _x, int _y);
	void AddActor(std::string name, class MinorRoll* mRoll);
	void RemoveActor(std::string name);
	void AddEffect(std::string name, class MinorRoll* mRoll);
	void RemoveEffect(std::string name);
	class TileMgr* GetTMgr() { return tMgr; }
	class MinorRoll* GetmRolls(std::string name) { return mRolls[name]; }
	class MinorRoll* GetmEffects(std::string name) { return mEffects[name]; }
	std::map<std::string, class MinorRoll*> GetmRolls() { return mRolls; }
	std::map<std::string, class MinorRoll*> GetmEffects() { return mEffects; }
	void deleteAll() { mRolls.clear(); }
	bool findmRoll(std::string name);
	void SetTile(int x, int y,int state);

	void AddEvent(std::string filepath);
	void UpdateEvent(float deltaTime);

	eventState GetPlayingEvent()const { return isPlayingEvent; }
	void SetPlayingEvent(eventState s) { isPlayingEvent = s; }
	bool Equals(float p1, float p2) {//���float�̐������������������ǂ������ׂ�֐�
		return static_cast<int>(p1) == static_cast<int>(p2);
	}
private:
	std::vector<int> images;
	std::vector<int> hitImages = { 2,3,5,6 };
	//int stage[30][40];
	std::vector<std::vector<int>> stage;
	int w;
	int h;
	class TileMgr* tMgr;
	std::map<std::string, class MinorRoll*> mRolls;		//���ꂼ��̃}�b�v�ɏ�������A�N�^�[
	std::map<std::string, class MinorRoll*> mEffects;	//�L�����ɂ���G�t�F�N�g�pmRolls
	eventState isPlayingEvent;//���s����ŏ��̂Ƃ�start,���s�̍Œ���playing,�I�������end,�L���[�ɖ���������none
	std::vector<events> eventFlow;
	int flow;
	std::queue<std::vector<events>> eventMgr;
	int _x, _y;		//����̓C�x���g�V�[���̏����̎��Ɉꎞ�I�Ɏg������
	float time;		//���Ԍo�߂����m����p
	bool isGoaled;
};