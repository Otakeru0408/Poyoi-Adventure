#include "Tile.h"
#include "TileMgr.h"
#include "MinorRoll.h"
#include <fstream>
#include <sstream>


Tile::Tile(TileMgr* _tMgr,std::string filepath)	//�}�b�v�@��40 ����30
:tMgr(_tMgr)
, isPlayingEvent(eventState::NONE)
,flow(0)
,_x(0)
,_y(0)
,isGoaled(true)		//�ړ�����false�Ƃ���Ǝn�߂�true�ɂȂ�B
{
	w = 16;
	h = 16;
	std::ifstream ifs(filepath);				//csv�t�@�C������X�e�[�W��ǂݍ���ŉ摜��\������
	std::string line;
	int j = 0;		//j�͏c�����܂�y	���ꂢ��Ȃ��H
	while (std::getline(ifs, line)) {
		std::vector<std::string> strvec = Split(line, ',');
		std::vector<int> _stage;
		for (int i = 0; i < strvec.size(); i++) {		//i�͉������܂�x
			_stage.emplace_back(std::stoi(strvec[i].c_str()));
		}
		stage.emplace_back(_stage);
		j++;
	}
	int num = tMgr->GetMaptipNum();
	for (int i = 0; i < num; i++) {
		images.emplace_back(tMgr->GetImage(i));
	}
}

void Tile::AddEvent(std::string filepath) {
	std::ifstream ifs(filepath);
	std::string line;
	eventFlow.clear();
	eventFlow.shrink_to_fit();
	while (std::getline(ifs, line)) {
		std::vector<std::string> strvec = Split(line, ',');
		events _event;
		_event.name = strvec[0];
		_event.x = std::stoi(strvec[1].c_str());
		_event.y = std::stoi(strvec[2].c_str());
		_event.time= std::stoi(strvec[3].c_str());
		_event.serif = strvec[4];
		eventFlow.emplace_back(_event);		//�X�N���v�g��s����ۑ�
	}
	eventMgr.push(eventFlow);				//�C�x���g������v�b�V������B
	isPlayingEvent = eventState::START;
}

void Tile::Draw() {
	for (int i = 0; i < stage.size(); i++) {
		for (int j = 0; j < stage[0].size(); j++) {
			DrawGraph(j * w, i * h, images[stage[i][j]], FALSE);
		}
	}
	if (!mRolls.empty()&&!mEffects.empty()) {
		switch (GetmRolls().begin()->second->attackType) {
		case Actor::PHYSICS:mEffects["physics"]->Draw();
			break;
		case Actor::MAGICS:mEffects["magics"]->Draw();
			break;
		default:break;
		}
	}

	for (auto mRoll : mRolls) {
		mRoll.second->Draw();
	}
}

void Tile::UpdateAll(float deltaTime) {
	for (auto mRoll : mRolls) {
		mRoll.second->Update(deltaTime);
	}
	for (auto mEffect : mEffects) {
		mEffect.second->Update(deltaTime);
	}
}

std::vector<std::string> Tile::Split(std::string& input, char delimiter) {
	std::istringstream stream(input);
	std::string field;
	std::vector<std::string> result;
	while (std::getline(stream, field, delimiter)) {
		result.push_back(field);
	}
	return result;
}

bool Tile::CheckTile(int _x, int _y) {
	bool isHit = false;
	for (int i = 0; i < hitImages.size(); i++) {
		if (stage[_y][_x] == hitImages[i]) {
			isHit = true;
		}
	}
	return isHit;
	/*if (stage[_y][_x] == 2 || stage[_y][_x] == 3 || stage[_y][_x] == 5) {
		return true;
	}
	else {
		return false;
	}*/
}

void Tile::AddActor(std::string name,MinorRoll* mRoll) {
	mRolls[name] = mRoll;
}

void Tile::RemoveActor(std::string name) {
	auto iter = mRolls.find(name);
	if (iter != mRolls.end()) {
		mRolls.erase(iter);
	}
}

void Tile::AddEffect(std::string name, class MinorRoll* mRoll) {
	mEffects[name] = mRoll;
}

void Tile::RemoveEffect(std::string name) {
	auto iter = mEffects.find(name);
	if (iter != mEffects.end()) {
		mEffects.erase(iter);
	}
}

bool Tile::findmRoll(std::string name) {
	auto iter = mRolls.find(name);
	if (iter != mRolls.end()) {
		return true;
	}
	else {
		return false;
	}
}

void Tile::SetTile(int x, int y,int state) {
	stage[y][x] = state;
}

//12 29 �` 15 29

void Tile::UpdateEvent(float deltaTime) {
	if (!eventMgr.empty()) {
		switch (isPlayingEvent) {
		case NONE:
			break;
		case START:
			eventFlow = eventMgr.front();
			isPlayingEvent = PLAYING;
			GetmRolls(eventFlow[flow].name)->SetAnimated(true);	//����ŋߊ��Ȃ��Ă��Z���t���o��悤�ɂȂ���
			tMgr->SetAnimated(true);
			break;
		case PLAYING:
			/*1.�X�N���v�g�P�s�Ɉ��s�����B
				�E���݂̍��W�ƃX�N���v�g��x,y�𑫂����l��ۑ�����B
				�E�Z���t�̐ݒ���s���B
			  2.�X�N���v�g�P�s�Ŗ���s�����B
				�E�S�[���̍��W�Ɍ������ď������ړ�������B
				�E�ړ���A�S�[���ɓ��B���Ă��邩�m�F�B
				�E�S�[�����Ă���flow++���Ă܂��s����������P�ɖ߂�B
				�E�����s���Ȃ��Ȃ�����R�ցB
			  3.�C�x���g�S�̂��I����Ĉ��s�����B
				�EisP��end�ɂ���B
			  */
			if (isGoaled) {
				_x = GetmRolls(eventFlow[flow].name)->x + eventFlow[flow].x;
				_y = GetmRolls(eventFlow[flow].name)->y + eventFlow[flow].y; 
				time = 0;
				if (eventFlow[flow].serif != "") {
					GetmRolls(eventFlow[flow].name)->SetSerif(eventFlow[flow].serif.c_str());
				}
				isGoaled = false;
			}
			else {
				if (!Equals(GetmRolls(eventFlow[flow].name)->x, _x)) {
					GetmRolls(eventFlow[flow].name)->x += eventFlow[flow].x * deltaTime / eventFlow[flow].time;
				}
				if (!Equals(GetmRolls(eventFlow[flow].name)->y, _y)) {
					GetmRolls(eventFlow[flow].name)->y += eventFlow[flow].y* deltaTime / eventFlow[flow].time;
				}
				if (!Equals(time, eventFlow[flow].time)) {
					time += deltaTime;
				}
				if (Equals(GetmRolls(eventFlow[flow].name)->x, _x) && Equals(GetmRolls(eventFlow[flow].name)->y, _y)&& Equals(time, eventFlow[flow].time)) {
					if (flow < eventFlow.size() - 1) {
						flow++;
						isGoaled = true;
					}
					else if (flow == eventFlow.size() - 1) {
						isPlayingEvent = eventState::END;
						isGoaled = true;
					}
				}
			}
			break;
		case END:
			GetmRolls(eventFlow[flow].name)->SetAnimated(false);
			tMgr->SetAnimated(false);
			eventMgr.pop();
			isPlayingEvent = NONE;

			break;
		default:break;
		}
	}
}