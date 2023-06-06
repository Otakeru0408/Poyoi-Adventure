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
	void SetSerif(int num, int pos = 0);		//セリフはすべて配列に保存しておき、各個で設定する
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
	std::string name;		//tileクラスで識別するための名前

	float mCurrFrame;
	float mAnimFPS;
	std::vector<int> mImages;
	std::vector<std::string> serifs = { "大変！妹のポヨミ さらわれた！","化け物　でた～！","ポヨミと化け物　南に行った！","助けて～！","","ぐへへ、おれのフィアンセ"
	,"ありがとー！","なにーー！？"};
	std::string serif;
	bool isOverlaped;
	int serifPos;
	bool isClosed;	//キャラが一度接触した後変化を起こすためのフラグ
	bool isAppeared;	//キャラを表示するかどうか
	bool isAnimated;	//今イベントシーンにいるかどうか
};