#include "Game.h"
#include "DxLib.h"

Game::Game() 
	:mIsRunning(true)
	,mTicksCount(0)
	,windowW(640)		//2^5*4*5=640  2^4*3*4*5=960 16*15 *4  yoko40 tate 30
	,windowH(480)		//2^5*3*5=480  2^4*3*3*5=720 16*15 *3
	,mUpdatingActor(false)
{

}

bool Game::Initialize() {
	if (ChangeWindowMode(TRUE) != 0) {
		return false;
	}

	SetBackgroundColor(200, 255, 200);
	/*SetWindowSizeChangeEnableFlag(TRUE, FALSE);		//1つ目：可変にするか、2つ目：サイズ固定にするか
	SetGraphMode(windowW, windowH,200);
	//SetWindowSizeExtendRate(1.0);
	SetWindowSize(960, 100);*/

	SetGraphMode(windowW,windowH, 32);
	SetWindowSizeExtendRate(2.0);
	//プロパティ→構成プロパティ→C/C++→コマンドライン　に/execution-charset:utf-8を追加しないと普通の""が使えなくなるみたい
	SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

	if (DxLib_Init() != 0) {
		return false;
	}
	if (SetDrawScreen(DX_SCREEN_BACK) != 0) {
		return false;
	}
	mTicksCount = GetNowCount();

	LoadData();
	return true;
}

void Game::RunLoop() {
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 && UpdateKey() == 0&&mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown() {
	//UnloadData();
	DxLib_End();
}

void Game::ProcessInput() {
	if (key[KEY_INPUT_ESCAPE]) {
		mIsRunning = false;
	}
	tMgr->ProcessInput(&key[0]);
}

void Game::UpdateGame() {
	if (GetNowCount() < mTicksCount + 16) {
		int waitTime = (mTicksCount + 16) - GetNowCount();
		Sleep(waitTime);
	}

	float deltaTime = (GetNowCount() - mTicksCount) / 1000.0f;

	//nakami
	mUpdatingActor = true;
	tMgr->Update(deltaTime);
	mUpdatingActor = false;


	mTicksCount = GetNowCount();
}

void Game::GenerateOutput() {
	tMgr->Draw();
}

void Game::LoadData() {
	tMgr = new TileMgr(windowW,windowH);
}

void Game::UnloadData() {
	while (!mActors.empty()) {
		delete mActors.back();
	}
}

void Game::AddActor(Actor* actor) {
	if (!mUpdatingActor) {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	auto iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()) {
		mActors.erase(iter);
	}
}

int Game::UpdateKey() {
	char tmpKey[256];
	GetHitKeyStateAll(tmpKey);
	for (int i = 0; i < 256; i++) {
		if (tmpKey[i] != 0) {
			key[i]++;
		}
		else {
			key[i] = 0;
		}
	}
	return 0;
}