#include "CSceneTitle.h"

int CSceneTitle::mMode = 1;
int CSceneTitle::mDifficulty = 2;

//次のシーンの取得
CScene::EScene CSceneTitle::GetNextScene(){
	return mScene;
}

void CSceneTitle::Render(){

}