#include "TileMgr.h"
#include "Tile.h"
#include "MinorRoll.h"
#include "Item.h"
#include <string.h>


TileMgr::TileMgr(int windowW, int windowH)
	: w(16)
	, h(16)
	, windowW(windowW)
	, windowH(windowH)
	,maptipNum(7)
	, mUpdatingActor(false)
	, mapNumW(2)		//マップのx方向の枚数
	, mapNumH(3)		//マップのy方向の枚数
	, whichMap(0)		//初期スポーン地点設定
	, whichMapMax(mapNumW* mapNumH)	//最大フィールドマップ数
	, isChecked(false)
	, whereStage(0)		//0:field  1:in house  2:...
	, _whichMap(0)
	, speed(5)
	,scene(0)
	,turn(0)
	,whichCommand(0)
	,count(0)
	,result(0)
	,stopTime(1.f)
	,isAnimated(false)
	,isStop(false)
	,maxStore(10)
{
	mPoyoi = new Poyoi(this, "Assets/poyoi.png");

	mPoyoi->attack = 5;

	LoadDivGraph("Assets/maptip2.png", maptipNum, maptipNum, 1, w, h, &images[0]);

	//タイトル画面読み込み
	titleTile = new Tile(this, "Stages/title.csv");

	//フィールドマップ読み込み
	Tile* t = new Tile(this, "Stages/stage.csv");
	mTiles.emplace_back(t);
	Tile* t2 = new Tile(this, "Stages/stage2.csv");
	mTiles.emplace_back(t2);
	Tile* t3 = new Tile(this, "Stages/stage3.csv");
	mTiles.emplace_back(t3);
	Tile* t4 = new Tile(this, "Stages/stage4.csv");
	mTiles.emplace_back(t4);
	Tile* g1 = new Tile(this, "Stages/stage-g1.csv");
	mTiles.emplace_back(g1);
	Tile* g2 = new Tile(this, "Stages/stage-g2.csv");
	mTiles.emplace_back(g2);

	//家マップ読み込み
	Tile* h1 = new Tile(this, "Stages/house1.csv");
	mHouseMaps.emplace_back(h1);

	//戦闘シーンマップ読み込み
	bScene = new Tile(this, "Stages/battleScene.csv");
	MinorRoll* physics = new MinorRoll(bScene, "Assets/PhysicEffect.png", "", 2, 1, 10, 5);
	bScene->AddEffect("physics",physics);
	MinorRoll* magics = new MinorRoll(bScene, "Assets/MagicEffect.png", "", 2, 1, 10, 5);
	bScene->AddEffect("magics", magics);

	MinorRoll* mRoll = new MinorRoll(h1, "Assets/poyoOji.png","poyoOji",2,1,10,3);
	mRoll->SetSerif(0);
	MinorRoll* mRoll2 = new MinorRoll(t2, "Assets/poyomob1.png", "poyomob1", 2, 1, 5, 5);
	mRoll2->SetSerif(1);
	MinorRoll* mRoll3 = new MinorRoll(t4, "Assets/poyomob2.png", "poyomob2", 2, 1, 12, 12);
	mRoll3->SetSerif(2);
	MinorRoll* mRoll4 = new MinorRoll(g2, "Assets/poyomi.png", "poyomi", 2, 1, 12, 12);
	mRoll4->SetSerif(3,1);
	MinorRoll* mRoll5 = new MinorRoll(g2, "Assets/rouya.png", "rouya", 12, 12);
	mRoll5->SetSerif(4);
	MinorRoll* mRoll6 = new MinorRoll(g2, "Assets/poyoteki.png", "poyoteki", 2, 1, 11, 12);
	mRoll6->SetSerif(5);

	//アイテム読み込み
	Item* key = new Item("Assets/key1.png", 1, false);
	mPoyoi->AddItem("key",key);
	Item* rouya = new Item("Assets/rouya.png", 1, false);
	mPoyoi->AddItem("rouya", rouya);

	//イベントシーン読み込み
	g2->AddEvent("EventScripts/Stage-g2-event.txt");
	h1->AddEvent("EventScripts/House1-event.txt");
	h1->AddEvent("EventScripts/House1-event2.txt");


	SetFontSize(25);
}
//フィールドと家のマップの切り替え問題は、正の方向にフィールド、負の方向に家の配列とすることで解決

/*void TileMgr::AddActor(class Actor* actor) {
	if (!mUpdatingActor) {
		mPoyos.emplace_back(actor);
	}
}
 
void TileMgr::RemoveActor(class Actor* actor) {
	auto iter = std::find(mPoyos.begin(), mPoyos.end(), actor);
	if (iter != mPoyos.end())
	{
		mPoyos.erase(iter);
	}
}*/

void TileMgr::AddActor(std::string name, MinorRoll* mRoll) {
	if (!mUpdatingActor) {
		auto iter = mPoyos.find(name);
		if (iter == mPoyos.end()) {
			mPoyos[name] = mRoll;
		}
	}
}

void TileMgr::RemoveActor(std::string name) {
	auto iter = mPoyos.find(name);
	if (iter != mPoyos.end()) {
		mPoyos.erase(iter);
	}
}

void TileMgr::Draw() {
	if (scene == 0) {
		titleTile->Draw();
		DrawString(270, 270, "ヨイの冒険", GetColor(0, 0, 0));
	}
	else if (scene==1) {
		if (whichMap >= 0) {		//正の数なら（0含む）
			mTiles[whichMap]->Draw();
		}
		else {						//負の数なら
			mHouseMaps[-whichMap - 1]->Draw();
		}
		for (auto& actor : mPoyos) {
			actor.second->Draw();
		}
		mPoyoi->Draw();
		mPoyoi->DrawItems();
	}
	else if(scene==2){
		bScene->Draw();
		mPoyoi->Draw();
		DrawUI();
	}
	else if (scene == 3) {

	}
	/*DrawFormatString(10, 10, GetColor(255, 255, 255), "posX:%d", (mPoyoi->x+w*2)/w);
	DrawBox(39 * w + w / 2, 0, 39 * w + w / 2+w, h * 30, GetColor(255, 255, 255), FALSE);
	DrawBox(0, 0, w / 2, h * 30, GetColor(255, 255, 255), FALSE);
	DrawBox(0, 0, w * 40, h / 2, GetColor(255, 255, 255), FALSE);
	DrawBox(0, h * 29 + h / 2, w * 40, h * 29 + h / 2+h, GetColor(255, 255, 255), FALSE);*/

	//mFps.Draw();
	/*switch (mTiles[5]->GetPlayingEvent()) {
	case Tile::eventState::START:
		DrawBox(10, 10, 20, 20, GetColor(255, 0, 0), TRUE);
		break;
	case Tile::eventState::PLAYING:
		DrawBox(10, 10, 20, 20, GetColor(0, 255, 0), TRUE);
		break;
	case Tile::eventState::END:
		DrawBox(10, 10, 20, 20, GetColor(0, 0, 255), TRUE);
		break;
	}*/
}

void TileMgr::DrawUI() {
	int thickness = 5;
	DrawBox(50, windowH / 3 * 2, windowW - 50, windowH - 50, GetColor(200, 200, 255), TRUE);
	DrawBox(50, windowH / 3 * 2, 50 + thickness, windowH - 50, GetColor(100, 100, 255), TRUE);	//右線
	DrawBox(50, windowH / 3 * 2, windowW - 50, windowH / 3 * 2 + thickness, GetColor(100, 100, 255), TRUE);	//上線
	DrawBox(50, windowH - 50, windowW - 50, windowH - 50 - thickness, GetColor(100, 100, 255), TRUE);	//下線
	DrawBox(windowW - 50, windowH / 3 * 2, windowW - 50 - thickness, windowH - 50, GetColor(100, 100, 255), TRUE);	//左線

	if (turn == 0) {		//選択中
		if (isStop) {
			turn = 1;
			count = 0;
		}
		else {
			if ((int)count == 0) {
				//敵の攻撃属性を決める
				int type = GetRand(100);
				if (type > 20) {
					bScene->GetmRolls().begin()->second->attackType = Actor::PHYSICS;
				}
				else {
					bScene->GetmRolls().begin()->second->attackType = Actor::MAGICS;
				}
			}
			count++;
			int green = GetColor(0, 200, 0);
			int black = GetColor(0, 0, 0);
			int strLen, strWidth = 0;
			int i = 0;
			for (auto c : command) {
				if (i == whichCommand) {
					DrawString(90 + strWidth, windowH - 110, c.c_str(), green);
				}
				else {
					DrawString(90 + strWidth, windowH - 110, c.c_str(), black);
				}
				strLen = strlen(c.c_str());
				strWidth += GetDrawStringWidth(c.c_str(), strLen);
				strWidth += 20;
				i++;
			}
			switch (bScene->GetmRolls().begin()->second->attackType) {
			case Actor::PHYSICS:DrawString(10, 10, "physics", GetColor(0, 0, 0));
				break;
			case Actor::MAGICS:DrawString(10, 10, "magics", GetColor(0, 0, 0));
				break;
			default:break;
			}
		}
	}
	else if (turn == 1) {//選択終了時
		if (isStop) {
			if (mPoyoi->GetStoreCount() > 0) {
				DrawFormatString(90, windowH - 110, GetColor(0, 0, 0), "ポヨイは反動で動けない！あと%dターン", mPoyoi->GetStoreCount());
			}
			else if (mPoyoi->GetStoreCount() == 0) {
				DrawString(90, windowH - 110, "ポヨイは反動で動けない！つぎはいけるぞ",GetColor(0, 0, 0));
			}
		}
		else {
			switch (whichCommand) {
			case 0:
				if (result == 0 || (result == 1 && count <= 60 * stopTime)) {	//戦闘中か勝ってstopTime秒間はこれ
					DrawFormatString(90, windowH - 110, GetColor(0, 0, 0), "ポヨイは敵に%dのダメージを与えた！", bScene->GetmRolls().begin()->second->damage);
				}
				else if (result == 1 && (count >= 60 * stopTime && count < 60 * stopTime * 3)) {	//選択終了時、2秒間表示
					DrawString(90, windowH - 110, "ポヨイは戦闘に勝利した！", GetColor(0, 0, 0));
				}
				break;
			case 1:
				DrawString(90, windowH - 110, "ポヨイは防御した！", GetColor(0, 0, 0));
				break;
			case 2:
				if (result == 0) {
					DrawString(90, windowH - 110, "ポヨイは逃げられなかった！", GetColor(0, 0, 0));
				}
				else {
					DrawString(90, windowH - 110, "ポヨイは逃げた！", GetColor(0, 0, 0));
				}
				break;
			case 3:
				if (result == 0) {
					DrawString(90, windowH - 110, "ポヨイは力をためた。攻撃力倍だ！", GetColor(0, 0, 0));
				}
				if (result == 1) {
					DrawString(90, windowH - 110, "これ以上貯められない！", GetColor(0, 0, 0));
				}
				break;
			case 4:
				if (result == 0) {
					DrawString(90, windowH - 110, "ポヨイは敵の攻撃を吸収する！",GetColor(0,0,0));
				}
				else {
					DrawString(90, windowH - 110, "ポヨイは敵の攻撃の吸収に失敗した！", GetColor(0, 0, 0));
				}
			}
		}
	}
	else if (turn == 2) {
		switch (bScene->GetmRolls().begin()->second->attackType) {
		case Actor::PHYSICS:
			if (whichCommand == 4) {
				DrawFormatString(90, windowH - 110, GetColor(0, 0, 0), "%sは%dの物理攻撃を吸収された！", bScene->GetmRolls().begin()->first.c_str(), mPoyoi->damage);
			}
			else {
				DrawFormatString(90, windowH - 110, GetColor(0, 0, 0), "%sは%dの物理攻撃を与えた！", bScene->GetmRolls().begin()->first.c_str(), mPoyoi->damage);
			}
			break;
		case Actor::MAGICS:
			DrawFormatString(90, windowH - 110, GetColor(0, 0, 0), "%sは%dの魔法攻撃を与えた！", bScene->GetmRolls().begin()->first.c_str(), mPoyoi->damage);
		}
	}

	//ポヨイのHPバー
	int scale = 2;
	int mHP = mPoyoi->maxHP * scale / 10;
	if (mPoyoi->HP > 0) {
		DrawBox(10, 150, 10 + mPoyoi->HP * scale, 150 + 10 * scale, GetColor(0, 255, 0), TRUE);
	}
	DrawBox(10, 150, 10 + mPoyoi->maxHP * scale, 150 + 10 * scale, GetColor(0, 0, 0), FALSE);
	for (int i = 1; i < 10; i++) {
		DrawLine(10 + i * mHP, 150, 10 + i * mHP, 150 + 10 * scale, GetColor(100, 100, 100));
	}

	//敵のHPバー

	int eHP = bScene->GetmRolls().begin()->second->maxHP * scale / 10;
	if (bScene->GetmRolls().begin()->second->HP > 0) {
		DrawBox(430, 150, 430 + bScene->GetmRolls().begin()->second->HP * scale, 150 + 10 * scale, GetColor(0, 255, 0), TRUE);
	}
	DrawBox(430, 150, 430 + bScene->GetmRolls().begin()->second->maxHP * scale, 150 + 10 * scale, GetColor(0, 0, 0), FALSE);
	for (int i = 1; i < 10; i++) {
		DrawLine(430 + i * eHP, 150, 430 + i * eHP, 150 + 10 * scale, GetColor(100, 100, 100));
	}
}

void TileMgr::Update(float deltaTime) {
	if (scene == 1) {
		switch (whereStage) {
		case 0:FieldUpdate(deltaTime); break;
		case 1:HouseUpdate(deltaTime); break;
		default:break;
		}
	}
	else if (scene == 2) {
		BattleUpdate(deltaTime);
	}
	for (auto& actor : mPoyos) {
		actor.second->Update(deltaTime);
	}
	mPoyoi->Update(deltaTime);
	mFps.Update();
}

/*
FieldUpdateの中身
・上下左右端に行ったときあったらそのマップに移動する
・キャラやアイテムとの接触判定と処理
・特定のマップのお家マスにいったらお家マップに移動

HouseUpdateの中身
・そのマップのmRollたちのUpdateをする（アニメーションとか）
・戻れるマスに行ったら戻れる
*/

void TileMgr::FieldUpdate(float deltaTime) {
	if (isHit2(mPoyoi->x, mPoyoi->y, mPoyoi->w, mPoyoi->h, 39 * w+w/2, 0, w, h * 30)) {	//右方向
		if ((whichMap + 1) % mapNumW != 0 && !isChecked) {
			whichMap++;
			mPoyoi->x = w;
			for (auto& p : mPoyos) {
				if (p.second->isFollowed) {
					p.second->clearPos();
				}
				p.second->x = mPoyoi->x;
			}
			isChecked = true;
		}
		else {
			isChecked = false;
		}
	}
	if (isHit2(mPoyoi->x, mPoyoi->y, mPoyoi->w, mPoyoi->h, 0, 0, w/2, h * 30)) {	//左方向
		if ((whichMap) % mapNumW != 0 && !isChecked) {
			whichMap--;
			mPoyoi->x = w * 37;
			for (auto& p : mPoyos) {
				if (p.second->isFollowed) {
					p.second->clearPos();
				}
				p.second->x = mPoyoi->x;
			}
			isChecked = true;
		}
		else {
			isChecked = false;
		}
	}
	if (isHit2(mPoyoi->x, mPoyoi->y, mPoyoi->w, mPoyoi->h, 0, 0, w * 40, h/2)) {	//上方向
		if ((whichMap - mapNumW) >= 0 && !isChecked) {
			whichMap -= mapNumW;
			mPoyoi->y = h * 27;
			for (auto& p : mPoyos) {
				if (p.second->isFollowed) {
					p.second->clearPos();
				}
				p.second->y = mPoyoi->y;
			}
			isChecked = true;
		}
		else {
			isChecked = false;
		}
	}
	if (isHit2(mPoyoi->x, mPoyoi->y, mPoyoi->w, mPoyoi->h, 0, h * 29+h/2, w * 40, h)) {	//下方向
		if ((whichMap + mapNumW) < whichMapMax && !isChecked) {
			whichMap += mapNumW;
			mPoyoi->y = h;
			for (auto& p : mPoyos) {
				if (p.second->isFollowed) {
					p.second->clearPos();
				}
				p.second->y = mPoyoi->y;
			}
			isChecked = true;
		}
		else {
			isChecked = false;
		}
	}

	//キャラやアイテムの設定(フィールド編) isClosedは一度でも近づいた時　isOverlapedは重なった時
	mTiles[whichMap]->UpdateAll(deltaTime);

	if (mTiles[3]->GetmRolls("poyomob2")->GetOverlaped()) {
		mTiles[3]->GetmRolls("poyomob2")->SetClosed(true);
		if (!mTiles[5]->GetmRolls("poyoteki")->GetClosed()) {
			if (mHouseMaps[0]->GetmRolls("poyoOji")->GetClosed()) {
				mPoyoi->SetSerif("妹は南の方へ行ったようだ！");
			}
			else {
				mPoyoi->SetSerif("妹になにかあったのか！？");
			}
		}
	}

	if (mTiles[3]->GetmRolls("poyomob2")->GetClosed()&&mHouseMaps[0]->GetmRolls("poyoOji")->GetClosed()) {
		mTiles[2]->SetTile(12, 29, 0);
		mTiles[2]->SetTile(13, 29, 0);
		mTiles[2]->SetTile(14, 29, 0);
		mTiles[2]->SetTile(15, 29, 0);
	}

	if (mTiles[5]->GetmRolls("rouya")->GetOverlaped()) {
		if (mPoyoi->GetItem("key")->GetAdded()) {					//ろうやのカギを持っていたら
			mTiles[5]->GetmRolls("rouya")->SetAppeared(false);
			mTiles[5]->GetmRolls("rouya")->SetOverlaped(false);		//isAppearedがオフになるとoverlapは変更できなくなるのでtrueであり続ける
			mTiles[5]->GetmRolls("poyomi")->SetSerif(6,1);
			mTiles[5]->GetmRolls("poyoteki")->SetSerif(7);
			mPoyoi->GetItem("key")->SetAdded(false);
			mTiles[5]->GetmRolls("poyoteki")->SetClosed(false);
		}
	}

	if (mTiles[5]->GetmRolls("poyoteki")->GetAppeared()) {				//poyotekiが表示されていたら
		if (mTiles[5]->GetmRolls("rouya")->GetAppeared()) {		//牢屋解除してなかったら
			if (mTiles[5]->GetmRolls("poyoteki")->GetOverlaped()) {
				mTiles[5]->GetmRolls("poyoteki")->SetClosed(true);
				mHouseMaps[0]->GetmRolls("poyoOji")->SetSerif("このカギ　つかえ！");
				mPoyoi->SetSerif("妹がとらわれたろうやのカギをさがそう！");
			}
		}
		else {					//牢屋解除してたら
			//ポヨおじにカギをもらった時tekiのclosedをfalseにする
			if (mTiles[5]->GetmRolls("poyoteki")->GetOverlaped() && !mTiles[5]->GetmRolls("poyoteki")->GetClosed()) {
				//poyotekiに接触し、isClosedがfalseなら
				if (mTiles[5]->GetPlayingEvent() == Tile::eventState::START) {
					mTiles[5]->UpdateEvent(deltaTime);
					mPoyoi->SetSerif("敵がおそってきた！戦うしかない！");
				}
			}
			else if (!mTiles[5]->GetmRolls("poyoteki")->GetOverlaped()) {
				mTiles[5]->GetmRolls("poyoteki")->SetClosed(false);
			}

			//接触したときにGetPlayingEventでSTARTからPLAYINGにかえる
			if (mTiles[5]->GetPlayingEvent() == Tile::eventState::PLAYING) {
				mTiles[5]->UpdateEvent(deltaTime);
			}
			else if (mTiles[5]->GetPlayingEvent() == Tile::eventState::END) {
				mTiles[5]->UpdateEvent(deltaTime);
				if (!mTiles[5]->GetmRolls("poyoteki")->GetClosed()) {
					SetinBattleF(5, "poyoteki");
				}
				mTiles[5]->GetmRolls("poyoteki")->SetClosed(true);
			}
			else if(mTiles[5]->GetPlayingEvent()==Tile::eventState::NONE) {
				if (mTiles[5]->GetmRolls("poyoteki")->GetOverlaped()) {
					SetinBattleF(5, "poyoteki");
				}
			}
		}
	}
	else {
		if (mTiles[5]->findmRoll("poyomi")) {
			//ポヨミをmpoyosに追加
			AddActor("poyomi", mTiles[5]->GetmRolls("poyomi"));
			//ポヨミをmTiles[5]から削除する
			mTiles[5]->RemoveActor("poyomi");
			//ポヨミをついてくるようにする。
			mPoyos["poyomi"]->isFollowed = true;
			mPoyos["poyomi"]->SetSerif(4);
			mHouseMaps[0]->GetmRolls("poyoOji")->SetSerif("");
		}
	}

	if (findActor("poyomi")) {
		if (mPoyos["poyomi"]->isFollowed) {
			followdist=mPoyos["poyomi"]->Following(mPoyoi->x, mPoyoi->y);
		}
	}

	if (!mTiles[1]->GetmRolls("poyomob1")->GetOverlaped()) {
		mTiles[1]->GetmRolls("poyomob1")->SetClosed(false);
	}

	if (mTiles[1]->GetmRolls("poyomob1")->GetOverlaped()&&mTiles[1]->GetmRolls("poyomob1")->GetAppeared()) {
		if (!mTiles[1]->GetmRolls("poyomob1")->GetClosed()) {
			SetinBattleF(1, "poyomob1");
		}
		mTiles[1]->GetmRolls("poyomob1")->SetClosed(true);
	}


	//お家マスに行ったらお家マップへ
	if (Intersect(mPoyoi, w * 4, h * 17, w * 2, h * 2) && whichMap == 0) {
		_whichMap = whichMap;
		whichMap = -1;
		mPoyoi->x = w * 19;
		mPoyoi->y = h * 22;
		whereStage = 1;
		if (findActor("poyomi")) {
			if (mPoyos["poyomi"]->isFollowed) {
				mPoyos["poyomi"]->clearPos();
			}
			mPoyos["poyomi"]->x = mPoyoi->x;
			mPoyos["poyomi"]->y = mPoyoi->y;

			if (mHouseMaps[0]->GetPlayingEvent() == Tile::eventState::START) {
				mHouseMaps[0]->UpdateEvent(deltaTime);
				mPoyoi->SetSerif("おじさん...?");
			}
		}
	}
}

void TileMgr::HouseUpdate(float deltaTime) {
	mHouseMaps[-whichMap - 1]->UpdateAll(deltaTime);

	//キャラやアイテムの設定(お家編)

	if (mHouseMaps[0]->GetmRolls("poyoOji")->GetAppeared()) {
		if (mHouseMaps[0]->GetmRolls("poyoOji")->GetOverlaped()) {
			mHouseMaps[0]->GetmRolls("poyoOji")->SetClosed(true);
			if (!mPoyoi->GetItem("key")->GetAdded()) {
				mPoyoi->SetSerif("妹がさらわれた！？");
			}
		}

		if (mHouseMaps[0]->GetPlayingEvent() == Tile::eventState::PLAYING) {
			mHouseMaps[0]->UpdateEvent(deltaTime);
		}
		else if (mHouseMaps[0]->GetPlayingEvent() == Tile::eventState::END) {
			mHouseMaps[0]->UpdateEvent(deltaTime);
			if (!finish) {
				mHouseMaps[0]->GetmRolls("poyoOji")->SavePos(0);
				SetinBattleH(0, "poyoOji");
			}
			else {
				scene = 3;
			}
		}

		//敵と接触済みでまだ敵がいておじさんと接触したとき
		if (mHouseMaps[0]->GetmRolls("poyoOji")->GetOverlaped() && mTiles[5]->GetmRolls("poyoteki")->GetClosed() && mTiles[5]->GetmRolls("poyoteki")->GetAppeared()) {
			mPoyoi->GetItem("key")->SetAdded(true);
			mTiles[5]->GetmRolls("poyoteki")->SetClosed(false);
			mPoyoi->SetSerif("カギをもらった！しかしなぜおじさんが...？");
		}
	}
	if (findActor("poyomi")) {
		if (mPoyos["poyomi"]->isFollowed) {
			mPoyos["poyomi"]->Following(mPoyoi->x, mPoyoi->y);
		}
	}
	if (finish) {
		mHouseMaps[0]->GetmRolls("poyoOji")->SetAppeared(true);
		mHouseMaps[0]->SetPlayingEvent(Tile::START);
		mHouseMaps[0]->UpdateEvent(deltaTime);
	}
	

	if (Intersect(mPoyoi, w * 19, h * 25, w * 2, h)) {
		whichMap = _whichMap;
		mPoyoi->x = w * 4;
		mPoyoi->y = h * 19;
		whereStage = 0;
		if (findActor("poyomi")) {
			if (mPoyos["poyomi"]->isFollowed) {
				mPoyos["poyomi"]->clearPos();
			}
			mPoyos["poyomi"]->x = mPoyoi->x;
			mPoyos["poyomi"]->y = mPoyoi->y;
		}
	}
}

void TileMgr::BattleUpdate(float deltaTime) {
	bScene->UpdateAll(deltaTime);
	if (turn == 1) {			//turn=1は選択が終わり処理の段階
		//bWay[whichCommand](mPoyoi, bScene->GetmRolls().begin()->second);
		//int result = 0;
		if (count == 0) {
			mPoyoi->isGuard = false;
			if (isStop) {
				result = 0;
				mPoyoi->SetStoreCount(mPoyoi->GetStoreCount() - 1);	//待機カウントを-1する
			}
			else {
				switch (whichCommand) {
				case 0:result = battle(mPoyoi, bScene->GetmRolls().begin()->second); break;		//相手のHP0になればreturn1
				case 1:result = guard(mPoyoi, bScene->GetmRolls().begin()->second); break;		//p->isGuard=true
				case 2:result = run(mPoyoi, bScene->GetmRolls().begin()->second); break;		//確率で逃げれたら1
				case 3:result = store(mPoyoi, bScene->GetmRolls().begin()->second); break;		//力をためる。１０回が限界。
				case 4:result=absorb(mPoyoi, bScene->GetmRolls().begin()->second); break;		//敵の攻撃を吸収して回復する。
				default:break;
				}
			}
		}
		//諸事情によりここに置かせて
		count += 60 * deltaTime;	//なんとなく60にしてるけど実際なんでもいいのかも
		if (result == 0&&count>=60*stopTime) {
			turn = 2;
			count = 0;
			if (whichCommand == 0 && mPoyoi->GetStoreCount() > 0) {
				isStop = true;
			}
		}
		else if (result == 1&&count>=60*stopTime) {		//相手のHPを0にしたor逃げれたら
			if ((whichCommand == 0 || whichCommand == 2) && count >= 60 * stopTime*3) {
				turn = 3;
			}
			else if (whichCommand == 4 || whichCommand == 3) {
				turn = 2;
				count = 0;
			}
		}
	}
	if (turn == 2) {				//敵のターン
		if (count == 0) {
			int attack = bScene->GetmRolls().begin()->second->attack - 5 + GetRand(10);
			if (bScene->GetmRolls().begin()->second->attackType == Actor::MAGICS) {
				attack *= 2;
			}
			if (mPoyoi->isGuard) {
				attack /= 2;
			}
			if (whichCommand == 4 && result == 0) {
				mPoyoi->HP += attack;
				if (mPoyoi->HP >= mPoyoi->maxHP) {
					mPoyoi->HP = mPoyoi->maxHP;
				}
			}
			else {
				mPoyoi->HP -= attack;
			}
			mPoyoi->damage = attack;
			mPoyoi->damageType = bScene->GetmRolls().begin()->second->attackType;

			if (isStop) {
				if (mPoyoi->GetStoreCount() == 0) {
					isStop = false;
				}
			}
		}

		if (count >= 60*stopTime) {
			if (mPoyoi->HP <= 0) {	//もしもポヨイが負けたら->逃げると同じ判定にする。
				whichCommand = 2;
				turn = 3;
				count = 0;
				mPoyoi->SetStoreCount(0);
				isStop = false;
			}
			else {
				turn = 0;				//ターン終了
				count = 0;
			}
		}
		count += 60 * deltaTime;
	}
	if (turn == 3) {
		scene = 1;
		mPoyoi->SavePos(1);
		if (whichCommand == 0) {		//戦って勝った場合
			mPoyoi->SetStoreCount(0);
			isStop = false;
			bScene->GetmRolls().begin()->second->SetAppeared(false);
			if (bScene->findmRoll("poyoOji")) {
				finish = true;
			}
			bScene->GetmRolls().begin()->second->SavePos(1);
			bScene->deleteAll();
			if (findActor("poyomi")) {
				if (mPoyos["poyomi"]->isFollowed) {
					mPoyos["poyomi"]->clearPos();
				}
				mPoyos["poyomi"]->x = mPoyoi->x;
				mPoyos["poyomi"]->y = mPoyoi->y;
			}
			mPoyoi->SetSerif("妹を取り戻した！おじさんの家へ帰ろう");
			if (!mHouseMaps[0]->GetmRolls("poyoOji")->GetAppeared()) {
				mPoyoi->SetSerif("おじさんは悪の手先だった...これで村は平和になった！");
			}
		}
		else if (whichCommand == 2) {	//逃げた場合
			std::map<std::string, class MinorRoll*> _mRoll = bScene->GetmRolls();
			auto iter = _mRoll.begin();
			while (iter != _mRoll.end()) {
				iter->second->SavePos(1);
				iter++;
			}
			bScene->GetmRolls().begin()->second->SavePos(1);
			bScene->deleteAll();
			if (findActor("poyomi")) {
				if (mPoyos["poyomi"]->isFollowed) {
					mPoyos["poyomi"]->clearPos();
				}
				mPoyos["poyomi"]->x = mPoyoi->x;
				mPoyos["poyomi"]->y = mPoyoi->y;
			}
		}
	}
}
/*
DrawUIでなにを描画するか
このゲームの戦闘システム　ターン制
・戦う　・逃げる　・防御する

選択したらenterまたはspaceで決定
	if(turn==1){
		bWays[whichCommand](mPoyoi->HP);
	}

	battleWay bWays[]={
		battle,guard,run
	};

	void battle(int& HP){
		
	}
*/

bool TileMgr::Intersect(class Actor* a, int x2, int y2, int w2, int h2) {
	if (isHit2(a->x, a->y, a->w, a->h, x2, y2, w2, h2) && !a->isFirst) {
		a->isFirst = true;
		return true;
	}
	else if (!isHit2(a->x, a->y, a->w, a->h, x2, y2, w2, h2) && a->isFirst) {
		a->isFirst = false;
	}
	return false;
}

void TileMgr::ProcessInput(uint32_t* key) {
	if (scene == 0) {
		if (key[KEY_INPUT_SPACE] != 0) {
			scene = 1;
			SetFontSize(16);
		}
	}
	else if (scene == 1&&!isAnimated) {
		FieldInput(key);
	}
	else if (scene == 2) {
		BattleInput(key);
	}
}

void TileMgr::FieldInput(uint32_t* key) {
	int _x = mPoyoi->x;
	int _y = mPoyoi->y;
	if (key[KEY_INPUT_D] != 0||key[KEY_INPUT_RIGHT]!=0) {
		_x += speed;
	}
	if (key[KEY_INPUT_A] != 0||key[KEY_INPUT_LEFT]!=0) {
		_x -= speed;
	}
	if (key[KEY_INPUT_S] != 0||key[KEY_INPUT_DOWN]!=0) {
		_y += speed;
	}
	if (key[KEY_INPUT_W] != 0||key[KEY_INPUT_UP]!=0) {
		_y -= speed;
	}

	if (_x < 0) {
		_x = 0;
	}
	if (_x > w * 38) {
		_x = w * 38;
	}
	if (_y < 0) {
		_y = 0;
	}
	if (_y > h * 28) {
		_y = h * 28;
	}

	if (checkTile(_x, _y)) {
		_x = mPoyoi->x;
		_y = mPoyoi->y;
	}

	mPoyoi->x = _x;
	mPoyoi->y = _y;
}

void TileMgr::BattleInput(uint32_t* key) {
	if (turn == 0) {
		if (key[KEY_INPUT_D] == 1) {
			whichCommand = (whichCommand + 1) % command.size();
		}
		else if (key[KEY_INPUT_A] == 1) {
			whichCommand = (whichCommand + command.size() - 1) % command.size();
		}
		if (key[KEY_INPUT_RETURN] == 1 || key[KEY_INPUT_SPACE] == 1) {
			turn = 1;
			count = 0;
		}
	}
}

//注意点　判定を4隅ギリギリではなくw/2分内側に持ってきているのでちょっと壁にめり込む。
bool TileMgr::checkTile(int x, int y) {		//ポヨイ君は32*32の4マス状態なので重なる4マスそれぞれについてチェックする
	//int _x[4] = { x / w,(x + w*2) / w,x / w,(x + w*2) / w };
	//int _y[4] = { y / h,y / h,(y + h*2) / h,(y + h*2) / h };
	int _x[4] = { (x + w / 2) / w,(x + w / 2 * 3) / w,(x + w / 2) / w,(x + w / 2 * 3) / w };	//四隅：左上(0,0) 右上(w*2,0) 左下(0,h*2) 右下(w*2,h*2)
	int _y[4] = { (y+h/2)/h,(y + h / 2) / h,(y + h / 2*3)/h,(y + h / 2 * 3)/h };
	for (int i = 0; i < 4; i++) {
		if (_x[i] < 40 && _y[i] < 40) {			//もしもチェックするタイルが範囲外なら除外する
			if (whichMap >= 0) {
				if (mTiles[whichMap]->CheckTile(_x[i], _y[i])) {
					return true;
				}
			}
			else {
				if (mHouseMaps[-whichMap - 1]->CheckTile(_x[i], _y[i])) {
					return true;
				}
			}
		}
	}
	return false;
}

TileMgr::~TileMgr() {
	mPoyos.clear();				//mapはclearだけで全部メモリ解放されるそうです。
	while (!mTiles.empty()) {
		delete mTiles.back();
	}
	while (!mHouseMaps.empty()) {
		delete mHouseMaps.back();
	}
}

bool TileMgr::isHit2(float bx, float by, float bw, float bh, float ex, float ey, float ew, float eh) {
	return (bx < (ex + ew) && ex < (bx + bw) && by < (ey + eh) && ey < (by + bh));
}

bool TileMgr::isHit(float bx, float by, float bw, float bh, float ex, float ey, float ew, float eh) {
	return ((bx - bw / 2) < (ex + ew / 2) && (ex - ew / 2) < (bx + bw / 2)) && ((by - bh / 2) < (ey + eh / 2) && (ey - eh / 2) < (by + bh / 2));
}

int TileMgr::GetPx() {
	return mPoyoi->x;
}

int TileMgr::GetPy() {
	return mPoyoi->y;
}

int TileMgr::battle(class Poyoi* p, class MinorRoll* m) {
	int attack = p->attack;
	int count = p->GetStoreCount();
	if (count > 0) {
		for (int i = 0; i < count; i++) {
			attack *= 2;
		}
		//isStop = true;	//ここでするべきではない？
	}
	m->damage = attack;		//敵のdamageにうけた攻撃力を保存する。
	m->HP -= attack;
	if (m->HP <= 0) {
		return 1;
	}
	return 0;
}

int TileMgr::guard(class Poyoi* p, class MinorRoll* m) {
	p->isGuard = true;
	return 0;
}

int TileMgr::run(class Poyoi* p, class MinorRoll* m) {
	int num = GetRand(9);		//0～9の内8未満ならつまり80%の確率で逃げれる
	if (num < 8) {
		return 1;
	}
	return 0;
}

int TileMgr::store(class Poyoi* p, class MinorRoll* m) {
	if (p->GetStoreCount() < maxStore) {
		p->SetStoreCount(p->GetStoreCount()+1);
		return 0;
	}
	else {
		return 1;	//1つまり失敗
	}
}

int TileMgr::absorb(class Poyoi* p, class MinorRoll* m) {
	//もし次の敵の攻撃属性が物理なら吸収させてもらう。
	if (bScene->GetmRolls().begin()->second->attackType == Actor::PHYSICS) {
		return 0;
	}
	else {
		return 1;
	}
}

void TileMgr::SetinBattleF(int _whichMap, std::string name) {
	scene = 2;															//戦闘シーンへ移動命令(下二つも必要)
	turn = 0;
	count = 0;
	mPoyoi->SavePos(0);
	mPoyoi->x = 100;													//ポヨイの位置設定
	mPoyoi->y = windowH / 2 - mPoyoi->h / 2;
	mPoyoi->HP = mPoyoi->maxHP;
	bScene->AddActor(name.c_str(), mTiles[_whichMap]->GetmRolls(name.c_str()));		//接触した敵を持ってくる
	MinorRoll* _mRoll = bScene->GetmRolls(name.c_str());
	_mRoll->SavePos(0);
	_mRoll->x = windowW - (_mRoll->w + 100);
	_mRoll->y = windowH / 2 - _mRoll->w / 2;
	for (auto ef : bScene->GetmEffects()) {
		ef.second->x = _mRoll->x;
		ef.second->y = _mRoll->y;
	}
}

void TileMgr::SetinBattleH(int _whichMap, std::string name) {
	scene = 2;															//戦闘シーンへ移動命令(下二つも必要)
	turn = 0;
	count = 0;
	mPoyoi->SavePos(0);
	mPoyoi->x = 100;													//ポヨイの位置設定
	mPoyoi->y = windowH / 2 - mPoyoi->h / 2;
	mPoyoi->HP = mPoyoi->maxHP;
	bScene->AddActor(name.c_str(), mHouseMaps[_whichMap]->GetmRolls(name.c_str()));		//接触した敵を持ってくる
	MinorRoll* _mRoll = bScene->GetmRolls(name.c_str());
	_mRoll->SavePos(0);
	_mRoll->x = windowW - (_mRoll->w + 100);
	_mRoll->y = windowH / 2 - _mRoll->w / 2;
}

bool TileMgr::findActor(std::string name) {
	auto iter = mPoyos.find(name);
	if (iter != mPoyos.end()) {
		return true;
	}
	else {
		return false;
	}
}