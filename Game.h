#pragma once
#include <stdint.h>
#include <vector>
#include <algorithm>
#include "Actor.h"
#include "Poyoi.h"
#include "TileMgr.h"

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void LoadData();
	void UnloadData();

	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	int GetWindowW()const { return windowW; }
	int GetWindowH()const { return windowH; }

private:
	int UpdateKey();

	bool mIsRunning;
	bool mUpdatingActor;
	uint32_t mTicksCount;
	uint32_t key[256];
	int windowW;
	int windowH;

	std::vector<class Actor*> mActors;
	std::vector<class Poyoi*> mPoyos;
	class TileMgr* tMgr;
};