#include "Item.h"

Item::Item(const TCHAR* path, int whichMap, bool isAppeared) 
	:Actor()
	,isAppeared(isAppeared)
	,isClosed(false)
	,isAdded(false)
{
	image = LoadGraph(path);
	GetGraphSize(image, &w, &h);
}

void Item::Draw() {
	DrawGraph(x, y, image, TRUE);
}