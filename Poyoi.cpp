#include "Poyoi.h"

Poyoi::Poyoi(TileMgr* tMgr, const TCHAR* path) 
	:Actor()
	,tMgr(tMgr)
	,image2{0,0}
	,mCurrFrame(0.0f)
	,mAnimFPS(10)
	,storeCount(0)
{
	LoadDivGraph(path, 2, 2, 1, 32, 32, &image2[0]);
	GetGraphSize(image2[0], &w, &h);
	image = image2[0];
	x = w * 5;
	y = h * 11;
	//tMgr->AddActor("poyoi",this);
	serif = "散歩しよう";
}

//Poyoi::Poyoi(Game* game,const TCHAR* path) 
//	:Actor(game)
//	,mCurrFrame(0.0f)
//	,mAnimFPS(10)
//{
//	LoadDivGraph(path, 2, 2, 1, 32, 32, image1->data());
//	GetGraphSize(image1->front(), &w, &h);
//	image = image1->front();
//	x = w * 5;
//	y = h * 3;
//}

void Poyoi::Update(float deltaTime) {
	mCurrFrame += mAnimFPS * deltaTime;
	while (mCurrFrame >= 2) {		//2はimage1の配列の個数
		mCurrFrame -= 2;
	}
	image = image2[static_cast<int>(mCurrFrame)];
	//image = image1->at(static_cast<int>(mCurrFrame));
}

void Poyoi::Draw() {
	DrawGraph(x, y, image, TRUE);
	//DrawFormatString(10, 10, GetColor(0, 0, 0), "x:%d,y:%d", x, y);
	if (tMgr->GetScene() == 1) {
		int strLen1 = strlen("ポヨイの気持ち：");
		int strLen2 = strlen(serif.c_str());
		int strWidth = GetDrawStringWidth("ポヨイの気持ち：", strLen1) + GetDrawStringWidth(serif.c_str(), strLen2);
		DrawBox(5, 5, strWidth + 15, 30, GetColor(100, 200, 100), TRUE);
		DrawFormatString(10, 10, GetColor(255, 255, 255), "ポヨイの気持ち：%s", serif.c_str());
	}
}

void Poyoi::AddItem(std::string name,Item* item) {
	auto iter = mItems.find(name);
	if (iter == mItems.end()) {
		mItems[name] = item;
	}
}

void Poyoi::RemoveItem(std::string name) {
	auto iter = mItems.find(name);
	if (iter != mItems.end()) {
		mItems.erase(iter);
	}
}

void Poyoi::DrawItems() {
	int num = 0;			//アイテムを順番に並べるための変数
	int margin = 30;		//枠の位置を画面端から離す距離
	int moreMar = 5;		//枠内のアイテムから枠への余白
	int wH = tMgr->GetWindowH();	//画面の高さを取得
	int iw = 0;						//アイテム一つの幅（高さ）
	if (!mItems.empty()) {
		iw = mItems.begin()->second->w;
	}
	std::vector<class Item*> _items;
	for (auto item : mItems) {
		if (item.second->GetAdded()) {
			_items.emplace_back(item.second);
		}
	}
	if (!_items.empty()) {
		DrawBox(margin - moreMar, wH - (w + margin+moreMar), margin + (w + moreMar) * _items.size(), wH - margin + moreMar, GetColor(255, 255, 255), TRUE);
		DrawBox(margin - moreMar, wH - (w + margin+moreMar), margin + (w + moreMar) * _items.size(), wH - margin + moreMar, GetColor(0, 0, 0), FALSE);
	}
	for (auto item : _items) {
		DrawRotaGraph(margin+num * (iw+moreMar)+iw/2, wH-(iw+margin)+iw/2, 1.0, 0.0, item->image, TRUE);
		num++;
	}
}