#include "MinorRoll.h"
#include "Tile.h"
#include "TileMgr.h"

MinorRoll::MinorRoll(Tile* tile, const TCHAR* path, std::string name, int xNum, int yNum, int posX, int posY)
	:Actor()
	, mTile(tile)
	, image2{ 0,0,0,0,0,0,0,0,0,0 }
	,name(name)
	, mCurrFrame(0.0f)
	, mAnimFPS(10)
	,serif(serifs[0])
	, isOverlaped(false)
	,serifPos(0)
	,isClosed(false)
	,isAppeared(true)
	,isAnimated(false)
{
	if (name != "") {
		mTile->AddActor(name, this);
	}
	LoadDivGraph(path, xNum*yNum,xNum, yNum, 32, 32, &image2[0]);
	GetGraphSize(image2[0], &w, &h);
	image = image2[0];
	int i = 0;
	while (image2[i] != 0) {
		mImages.emplace_back(image2[i]);
		i++;
	}
	x = w * posX;
	y = h * posY;
	_x = x;
	_y = y;
}

MinorRoll::MinorRoll(class Tile* tile, const TCHAR* path, std::string name, int posX, int posY)
	:Actor()
	, mTile(tile)
	, image2{ 0,0,0,0,0,0,0,0,0,0 }
	,name(name)
	, mCurrFrame(0.0f)
	, mAnimFPS(10)
	,isOverlaped(false)
	, serifPos(0)
	, isClosed(false)
	,isAppeared(true)
{
	mTile->AddActor(name,this);
	image=LoadGraph(path);
	//GetGraphSize(image, &w, &h);
	w = 32;
	h = 32;
	x = w * posX;
	y = h * posY;
	_x = x;
	_y = y;
}

MinorRoll::~MinorRoll() {
	mTile->RemoveActor(name);
}

void MinorRoll::Update(float deltaTime) {
	if (!mImages.empty()) {
		mCurrFrame += mAnimFPS * deltaTime;
		while (mCurrFrame >= mImages.size()) {
			mCurrFrame -= mImages.size();
		}
		image = mImages.at(static_cast<int>(mCurrFrame));
	}
}

void MinorRoll::Draw() {
	if (isAppeared) {
		DrawGraph(x, y, image, TRUE);
		int scene=mTile->GetTMgr()->GetScene();
		if (scene == 1) {
			isNear();
			if (isOverlaped||isAnimated) {
				if (serifPos == 0) {
					DrawFormatString(x, y - 15, GetColor(255, 255, 255), "%s", serif.c_str());
				}
				else if (serifPos == 1) {
					DrawFormatString(x + w + 5, y, GetColor(255, 255, 255), "%s", serif.c_str());
				}
			}
		}
	}
}

void MinorRoll::isNear() {
	//bool _isOver = isOverlaped;
	int px = mTile->GetTMgr()->GetPx();
	int py = mTile->GetTMgr()->GetPy();
	if (mTile->GetTMgr()->isHit2(x-w, y-h, w*3, h*3, px, py, w, h)) {
		isOverlaped = true;
	}
	else {
		isOverlaped = false;
	}
}

void MinorRoll::SetSerif(int num, int pos) {
	serif = serifs[num];
	serifPos = pos;
}

void MinorRoll::SetSerif(std::string _serif, int pos) {
	serif = _serif;
	serifPos = pos;
}


/*
例えば一秒間に５回更新を行い、ポヨイの座標を受け取る。
例えば５個の配列を用意し、ポヨイの座標を更新ごとにスタックしていく。
そして一番最後の座標にポヨミを設定する。
*/