#include "CSceneTitle.h"

int CSceneTitle::mMode = 1;
int CSceneTitle::mDifficulty = 2;

//���̃V�[���̎擾
CScene::EScene CSceneTitle::GetNextScene(){
	return mScene;
}

void CSceneTitle::Render(){

}