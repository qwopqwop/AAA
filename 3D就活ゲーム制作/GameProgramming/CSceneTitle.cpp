#include "CSceneTitle.h"

int CSceneTitle::mMode = 1; //コースやLap数の初期設定
int CSceneTitle::mDifficulty = 2; //敵AIの強さの初期設定　　1:EASY　2:NORMAL　3:HARD

//次のシーンの取得
CScene::EScene CSceneTitle::GetNextScene(){
	return mScene;
}

void CSceneTitle::Render(){

}