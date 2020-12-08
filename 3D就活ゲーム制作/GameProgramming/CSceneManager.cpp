#include "CSceneManager.h"
#include "CSceneGame.h"
#include "CSceneGameStage2.h"
#include "CSceneRace.h"
#include "CTaskManager.h"

CSceneManager SceneManager;

CScene::EScene mScene;


//コンストラクタ
CSceneManager::CSceneManager()
: mpScene(0)
{}
//デストラクタ（削除されるときに実行されます）
CSceneManager::~CSceneManager() {
	//シーンがあれば削除
	if (mpScene)
		//シーンの削除
		delete mpScene;
	mpScene = 0;
	CTaskManager::Get()->Destroy();
}
//初期化処理
void CSceneManager::Init() {
	////シーンの識別を設定する
	//mScene = CScene::ESTAGE2;

	////シーンを生成し、ポインタを設定する
	//mpScene = new CSceneGameStage2();

	////シーンの識別を設定する
	//mScene = CScene::ESTAGE1;
	////シーンを生成し、ポインタを設定する
	//mpScene = new CSceneGame();

	//シーンの識別を設定する
	mScene = CScene::ERACE1;
	//シーンを生成し、ポインタを設定する
	mpScene = new CSceneRace();

	//生成したクラスのメソッドが呼ばれる
	mpScene->Init();
}
//更新処理
void CSceneManager::Update() {
	//ポインタのUpdateを呼ぶ
	mpScene->Update();

	//次のシーンを取得し異なるか判定
	if (mScene != mpScene->GetNextScene()){
		mScene = mpScene->GetNextScene();
		delete mpScene;//今のシーン削除
		//該当するシーンを生成
		switch (mScene){
		case CScene::ESTAGE1:
			mpScene = new CSceneGame();
			mpScene->Init();
			break;
		case CScene::ESTAGE2:
			mpScene = new CSceneGameStage2();
			mpScene->Init();
			break;
		case CScene::ERACE1:
			mpScene = new CSceneRace();
			mpScene->Init();
			break;
		}
	}
}
