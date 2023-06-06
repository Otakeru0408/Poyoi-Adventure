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
Itemクラスではマップにこだわらず指定したマップに描画し、更にMinorRollのフラグに依存し現れるか現れないかを決める。
描画関数ではisAppearedによって描画するかしないかする
例えばあるキャラと接触することで鍵がとあるマップに描画され、その鍵に触れるとポヨイ君のアイテム欄に追加される
アイテム欄はポヨイ君のクラスでdrawするか。
*/