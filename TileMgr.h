#pragma once
#include <vector>
#include<stdint.h>
#include <algorithm>
#include <string>
#include <functional>
#include "Actor.h"
#include <map>
#include "Poyoi.h"
#include "Fps.h"

//typedef int(*battleWay)(class Poyoi*, class MinorRoll*);

class TileMgr {
public:
	TileMgr(int windowW,int windowH);
	~TileMgr();
	void Draw();
	int GetImage(int i) { return images[i]; }
	int GetMaptipNum()const { return maptipNum; }

	//void AddActor(class Actor* actor);
	//void RemoveActor(class Actor* actor);
	void AddActor(std::string name, class MinorRoll* mRoll);
	void RemoveActor(std::string name);

	void Update(float deltaTime);
	void FieldUpdate(float deltaTime);
	void HouseUpdate(float deltaTime);
	void BattleUpdate(float deltaTime);
	void FieldInput(uint32_t* key);
	void BattleInput(uint32_t* key);
	void DrawUI();
	int GetW()const { return w; }
	int GetH()const { return h; }
	int GetPx();
	int GetPy();
	int GetWindowW()const { return windowW; }
	int GetWindowH()const { return windowH; }
	int GetScene()const { return scene; }
	void SetinBattleF(int _whichMap, std::string name);
	void SetinBattleH(int _whichMap, std::string name);
	void SetAnimated(bool b) { isAnimated = b; }
	bool GetAnimated()const { return isAnimated; }

	bool Intersect(class Actor* a, int x2, int y2, int w2, int h2);
	void ProcessInput(uint32_t* key);

	bool checkTile(int x, int y);

	bool findActor(std::string name);

	//�Ԃ�l���P�ɂ���Ƃ���͓G���������������������Ƃ��Ӗ�����B
	int battle(class Poyoi* p, class MinorRoll* m);
	int guard(class Poyoi* p, class MinorRoll* m);
	int run(class Poyoi* p, class MinorRoll* m);
	int store(class Poyoi* p, class MinorRoll* m);
	int absorb(class Poyoi* p, class MinorRoll* m);

	//������͍��W�����S�̏ꍇ
	bool isHit(float bx, float by, float bw, float bh, float ex, float ey, float ew, float eh);
	//������͍��W������̏ꍇ
	bool isHit2(float bx, float by, float bw, float bh, float ex, float ey, float ew, float eh);

private:
	class Poyoi* mPoyoi;
	//std::vector<class Actor*> mPoyos;
	std::map<std::string, class MinorRoll*>mPoyos;		//�ǂ̃}�b�v�ł��p�����ēo�ꂷ��L�����i��l���Ȃǁj�͂����ɓ����B�}�b�v����Ȃ�Tile�N���X�̔z���
	std::vector<class Tile*> mTiles;
	std::vector<class Tile*> mHouseMaps;
	int maptipNum;
	int images[10];
	int w;
	int h;
	int windowW;
	int windowH;
	bool mUpdatingActor;

	int mapNumW;
	int mapNumH;
	int whichMap; 
	int _whichMap;
	int whichMapMax;
	bool isChecked;	//�}�b�v�ړ����ɋ��E�s��������o�O�΍�
	int whereStage;	//�t�B�[���h���������A�͂��܂��_���W�������A�݂����Ȃ̂����ʂ���
	int speed;		//�|���C�N�̓����X�s�[�h�i�Ȃ�ł����Œ�`�H�j
	int scene;		//�Q�[���S�̂̃V�[���Ǘ��ϐ��@0:�^�C�g�� 1:���ʂ̈ړ��}�b�v 2:�퓬�V�[�� 3������:�Q�[���I�[�o�[
	bool isAnimated;

	//�^�C�g����ʗp
	class Tile* titleTile;

	//�퓬�V�[���p�ϐ�
	class Tile* bScene;
	int turn;		//0:�|���C�I���@1:�|���C����&���� 2:�G����
	std::vector<std::string> command = { "�키","�h�䂷��","�y�����ŋ����𐿂�","���߂�","�z������"};
	int whichCommand;
	//battleWay bWay[3] = {battle,guard,run};
	float count;
	int result;		//�퓬���������ʁi1�^�[�����j����͂���
	float stopTime;
	int followdist = 0;
	bool isStop;
	int maxStore;

	bool finish = 0;

	Fps mFps;
};

/*
�����[�[�[�[�[�I�I�I�I�I
�֐��|�C���^����肭�g���Ă�낤�Ǝv������N���X�̃����o�֐��ł͂Ȃ񂩂��܂������Ȃ������[�[�[�I�I�I�I�I�I�I�I
����ς�switch���g��Ȃ��Ⴄ�܂�������̂��悧�I�I�I�I�I�@by03/23�̂悰
*/

/*
�u�|���C�N�̋~�o���v
1 ����̐l�ɘb�𕷂��Ė��������ꂽ���Ƃ�m��B
2 ���͉������Ɠ�ɂ������ƕ����̂ŉ��̕��ɍs���Ă݂�Ɣ����󂢂Ă���̂ōs���Ă݂�B
3 �������Ɩ���������B
4 �����������Ă���̂Ō���T���B
5 �Ƃɂ����ƌ������炦��B(�c�H)
6 ���̘S���̌�����������ƓG���P���|�����Ă���B
7 �G�Ɛ키�B
8 �G��|������ƂɋA��B
9 ����u���s�������A�����́B�v�Ƃ����ďP���|�����Ă���B
10 ����Ɛ키�B
11 �����Ă���𑒂苎��B��l�Œ��ǂ���炷�B
�@�@�@�@�@�@�@�@�@�@�@�߂ł����߂ł����B

�K�v�ȏ���
**�P�ł������Ƃ����Ă����͎�ɓ���Ȃ��悤�ɂ���B
**�Q�œ�ɍs�����ƕ����܂ł͉��֍s�����͕��Ă��邱�Ƃɂ���B
**�T�Ō�������ɂ��炤�Ƃ��̃Z���t�͕ς���ׂ�
**�U�ŉ����������ƂɈ��Ԋu�����Đ퓬�ɓ���ׂ�
**�W�œG�����������疅�̃Z���t��������Č������Ă���悤�ɂ���B
�E�X�Ŗ�����ĉƂɓ���Ƃ��ꂪ�P���|�����Ă���悤�ɂ���B
�E11�ł���ɏ������������O�Ɏ̂đ䎌���͂��ď�����悤�ɂ���B
�E���̂��Ɠ�l�ŉ�b����V�[��������ƂȂ��悢�B

�E������U������Ƃ��ɂ�����ƑO�ɂ҂���Ƃł�Ɩʔ����B
�E�G�̍U����10��ɂP��Ƃ������喂�@��ł��Ă��đ�_���[�W���󂯂�Ƃ��ɂ������B����ł��̑O����\�����Ėh��ł���悤�ɂ���B
�E�|���C�ɂ��N���e�B�J�������񂩂Ɉ��o��悤�ɂ������B

���ꂩ����ׂ��ۑ�
�E�^�C�g����ʁ��G���f�B���O��ʂ�ǉ�����B
�E�퓬�V�X�e�������ς���B�i���X�N�����^�[�����ӎ������݌v�j
�E�e���ɉ�����ǉ�����B
�E�i�K�v�Ȃ����������ǁj�Ȃ񂩂̃{�^���������ƃ_�b�V������悤�ɂ���H

�Z�퓬�̃V�X�e���ɂ���
�|���C�͌��݂́u�키�v�A�u�h�䂷��v�A�u������v�̑��ɁA�u�͂𒙂߂�v�A�u�z������v��ǉ�����B
����̓��X�N���^�[���Ɋ�Â����I�����ł���B
�u���߂�v�@�ł͓G�̍U��������ɂ�����1�^�[�����͂𒙂߂�B�����Ď��̃^�[���Ő키��I������Ɣ{�̗͂���B
�P�^�[�����߂������ł͂��܂�Ӗ��͂Ȃ���2�^�[�����߂�ƂS�{�A3�^�[�����߂��8�{�ɂȂ�B�܂�2�̊K��ő����Ă����̂��B
����ɂ�����̃��X�N�Ƃ��Ē��߂�ł͗͂̔����Œ��߂��^�[���������Ȃ��Ȃ��Ă��܂��B
�u�z������v�@����͓G�̍U�����z�����Ď�����HP���񕜂���Z�ł���B�������A�z���ł���͕̂����U�������B
�G�͖��@�U�����d�|���Ă���̂ł��̎��͋z���Ɏ��s���_���[�W�����̂܂܎󂯂�B

�Ώ����ׂ����
�EActor��x,y��float�ɂ����̂łǂ����ŕs����N���Ă邩���B
�ETIle��event�n��END�ɂ����Ƃ��ɂ܂�event���c���ĂĂ��Ƃ肠�����퓬�ɓ���悤�ɂȂ�������Ă�B
�����܂��C�x���g���c���Ă���퓬�I����C�x���g�������悤�ɂȂ��Ă�H
�E�Ȃ�poyoteki�̃Z���t���r������""���ꏏ�ɕ\�����ꂿ����Ă�B
�Epoyomob1�Ɛ퓬���āA���������poyoteki�ɐ킢�𒧂ނ�poyomob1���\�������o�O�B
*/