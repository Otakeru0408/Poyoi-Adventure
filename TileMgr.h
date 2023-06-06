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

	//返り値を１にするとそれは敵をたおしたか負けたことを意味する。
	int battle(class Poyoi* p, class MinorRoll* m);
	int guard(class Poyoi* p, class MinorRoll* m);
	int run(class Poyoi* p, class MinorRoll* m);
	int store(class Poyoi* p, class MinorRoll* m);
	int absorb(class Poyoi* p, class MinorRoll* m);

	//↓これは座標が中心の場合
	bool isHit(float bx, float by, float bw, float bh, float ex, float ey, float ew, float eh);
	//↓これは座標が左上の場合
	bool isHit2(float bx, float by, float bw, float bh, float ex, float ey, float ew, float eh);

private:
	class Poyoi* mPoyoi;
	//std::vector<class Actor*> mPoyos;
	std::map<std::string, class MinorRoll*>mPoyos;		//どのマップでも継続して登場するキャラ（主人公など）はここに入れる。マップ限定ならTileクラスの配列へ
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
	bool isChecked;	//マップ移動時に境界行き来するバグ対策
	int whereStage;	//フィールドか屋内か、はたまたダンジョンか、みたいなのを識別する
	int speed;		//ポヨイ君の動くスピード（なんでここで定義？）
	int scene;		//ゲーム全体のシーン管理変数　0:タイトル 1:普通の移動マップ 2:戦闘シーン 3か何か:ゲームオーバー
	bool isAnimated;

	//タイトル画面用
	class Tile* titleTile;

	//戦闘シーン用変数
	class Tile* bScene;
	int turn;		//0:ポヨイ選択　1:ポヨイ決定&処理 2:敵処理
	std::vector<std::string> command = { "戦う","防御する","土下座で許しを請う","ためる","吸収する"};
	int whichCommand;
	//battleWay bWay[3] = {battle,guard,run};
	float count;
	int result;		//戦闘をした結果（1ターン分）を入力する
	float stopTime;
	int followdist = 0;
	bool isStop;
	int maxStore;

	bool finish = 0;

	Fps mFps;
};

/*
くそーーーーー！！！！！
関数ポインタを上手く使ってやろうと思ったらクラスのメンバ関数ではなんかうまくいかないぞおーーー！！！！！！！！
やっぱりswitchを使わなきゃうまくいかんのかよぉ！！！！！　by03/23のよげ
*/

/*
「ポヨイ君の救出劇」
1 周りの人に話を聞いて妹がさらわれたことを知る。
2 妹は化け物と南にいったと聞くので下の方に行ってみると扉が空いているので行ってみる。
3 化け物と妹を見つける。
4 鍵がかけられているので鍵を探す。
5 家にいくと鍵をもらえる。(…？)
6 妹の牢屋の鍵を解除すると敵が襲い掛かってくる。
7 敵と戦う。
8 敵を倒したら家に帰る。
9 お爺「失敗したか、あいつは。」といって襲い掛かってくる。
10 お爺と戦う。
11 勝ってお爺を葬り去る。二人で仲良く暮らす。
　　　　　　　　　　　めでたしめでたし。

必要な処理
**１でおじいとあっても鍵は手に入らないようにする。
**２で南に行ったと聞くまでは下へ行く道は閉じていることにする。
**５で鍵をお爺にもらうときのセリフは変えるべき
**６で解除したあとに一定間隔あって戦闘に入るべき
**８で敵をたおしたら妹のセリフがかわって後ろをついてくるようにする。
・９で妹をつれて家に入るとお爺が襲い掛かってくるようにする。
・11でお爺に勝ったら消える前に捨て台詞をはいて消えるようにする。
・そのあと二人で会話するシーンがあるとなおよい。

・相手を攻撃するときにちょっと前にぴょっとでると面白い。
・敵の攻撃で10回に１回とか超特大魔法を打ってきて大ダメージを受けるとかにしたい。それでその前兆を表示して防御できるようにする。
・ポヨイにもクリティカルが何回かに一回出るようにしたい。

これからやるべき課題
・タイトル画面＆エンディング画面を追加する。
・戦闘システムを作り変える。（リスク＆リターンを意識した設計）
・各所に音声を追加する。
・（必要ないかもだけど）なんかのボタンを押すとダッシュするようにする？

〇戦闘のシステムについて
ポヨイは現在の「戦う」、「防御する」、「逃げる」の他に、「力を貯める」、「吸収する」を追加する。
これはリスクリターンに基づいた選択肢である。
「貯める」　では敵の攻撃をもろにうけつつ1ターン分力を貯める。そして次のターンで戦うを選択すると倍の力を放つ。
１ターン貯めただけではあまり意味はないが2ターン貯めると４倍、3ターン貯めると8倍になる。つまり2の階乗で増えていくのだ。
さらにもう一つのリスクとして貯めるでは力の反動で貯めたターン分動けなくなってしまう。
「吸収する」　これは敵の攻撃を吸収して自分のHPを回復する技である。しかし、吸収できるのは物理攻撃だけ。
敵は魔法攻撃も仕掛けてくるのでその時は吸収に失敗しダメージをそのまま受ける。

対処すべき問題
・Actorのx,yをfloatにしたのでどっかで不具合が起きてるかも。
・TIleのevent系でENDにしたときにまだeventが残っててもとりあえず戦闘に入るようになっちゃってる。
多分まだイベントが残ってたら戦闘終了後イベントが続くようになってる？
・なんかpoyotekiのセリフが途中から""が一緒に表示されちゃってる。
・poyomob1と戦闘して、逃げた後にpoyotekiに戦いを挑むとpoyomob1も表示されるバグ。
*/