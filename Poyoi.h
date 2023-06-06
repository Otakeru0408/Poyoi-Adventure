#pragma once
#include "Actor.h"
#include "TileMgr.h"
#include <string>
#include <vector>
#include<unordered_map>
#include "Item.h"

class Poyoi :public Actor {
public:
	Poyoi(class TileMgr* tMgr,const TCHAR* path);
	void Update(float deltaTime)override;
	void Draw()override;

	void AddItem(std::string name,class Item* item);
	void RemoveItem(std::string name);
	void DrawItems();
	class Item* GetItem(std::string name) { return mItems[name]; }
	void SetSerif(std::string s) { serif = s; }
	void SetStoreCount(int i) { storeCount = i; }
	int GetStoreCount()const { return storeCount; }
	
private:
	class TileMgr* tMgr;
	std::vector<int32_t> image1[2];
	std::unordered_map<std::string, class Item*> mItems;
	int image2[2];

	float mCurrFrame;
	float mAnimFPS;

	std::string serif;
	int storeCount;
};