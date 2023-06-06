#pragma once
#include <DxLib.h>
#include <stdint.h>
#include<queue>
#include <utility>

class Actor {
public:
	enum DamageType {
		PHYSICS,
		MAGICS,
	};

	Actor();
	virtual void Update(float deltaTime){}
	virtual void Draw(){}
	virtual void ProcessInput(uint32_t* key){}

	void SavePos(int flag);
	void SavePos(int flag,int px,int py);
	int Following(int _x, int _y);
	void clearPos();
	int culcDist(int x1, int y1, int x2, int y2) {
		return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	}

	float x;
	float y;
	int w;
	int h;
	int image;
	bool isFirst;	//tilemgrでの接触判定に使用する。一度接触しているかどうか
	int HP;
	int maxHP;
	int attack;		//攻撃力
	DamageType attackType;
	int damage;		//受けたダメージ量
	DamageType damageType;	//受けたダメージのタイプ	0:物理 1:魔法
	bool isGuard;
	bool isFollowed;	//キャラがついてくるかどうか
protected:
	int _x;					//別マップに行ったときに元の座標を保存しておく用
	int _y;
	std::queue<std::pair<int, int>> _pos;
};

/*
接触判定についての機能を実装したい

具体例：Actor1とActor2のx,y,w,hをそれぞれ受け取って範囲が接触しているかを確認する。
もしも接触していたら１を返す。もしそうでなければ０を返す。

問題：二つのActorが接触した際何かを動作させるが、その後も二つは接触したままなので反応し続けてしまう。
フラグを用いて解決できるが煩雑になる。

そこでこの機能をカプセル化して楽に使えるようにしたい。

使い方としては、
if(intersect(x1,y1,w1,h1,x2,y2,w2,h2)){←これは二つの領域が初めて接触した場合のみtrueを返す
	//なんか処理をする
}

bool Intersect(class Actor* a,x2,y2,w2,h2){
	if(isHit2(*a.x,*a.y,*a.w,*a.h,x2,y2,w2,h2)&&!*a.isFirst){
		isFirst=true;
		return true;
	}else if(!isHit2(x1,y1,w1,h1,x2,y2,w2,h2)&&isFirst){
		isFirst=false;
	}
	return false;
}
*/