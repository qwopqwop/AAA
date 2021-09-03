#ifndef CSCENETITLE_H
#define CSCENETITLE_H
#include "CText.h"
#include "CScene.h"
#include "CKey.h"
#include <Windows.h>
#include "CSound.h"
//・・・
#include "CSceneRace.h"

class CSceneTitle : public CScene {
private:
	int mVariable1;
	int mPrevVariable1;
	int mVariable2;
	int mPrevVariable2;
	bool mStart;
	int mStartWaitTime;
	int mPrevDifficulty;
	int mSelectScene_Level;//選択画面の何段階目か
public:
	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画処理
	void Render();

	void RenderRect(int x, int y, int w, int h);

	//次のシーンの取得
	EScene GetNextScene();

	
	CSound SoundMoveCarsol;
	CSound SoundDecide;
	CSound SoundCancel;
	
	static int mMode;//コースNo.
	static int mDifficulty;//CPUの強さ	
};

#endif