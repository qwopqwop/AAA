#include "CSceneTitle.h"

int CSceneTitle::mMode = 1; //�R�[�X��Lap���̏����ݒ�
int CSceneTitle::mDifficulty = 2; //�GAI�̋����̏����ݒ�@�@1:EASY�@2:NORMAL�@3:HARD

//���̃V�[���̎擾
CScene::EScene CSceneTitle::GetNextScene(){
	return mScene;
}

void CSceneTitle::Render(){

}