#include "CSceneManager.h"
#include "CSceneGame.h"
#include "CSceneGameStage2.h"
#include "CSceneRace.h"
#include "CTaskManager.h"

CSceneManager SceneManager;

CScene::EScene mScene;


//�R���X�g���N�^
CSceneManager::CSceneManager()
: mpScene(0)
{}
//�f�X�g���N�^�i�폜�����Ƃ��Ɏ��s����܂��j
CSceneManager::~CSceneManager() {
	//�V�[��������΍폜
	if (mpScene)
		//�V�[���̍폜
		delete mpScene;
	mpScene = 0;
	CTaskManager::Get()->Destroy();
}
//����������
void CSceneManager::Init() {
	////�V�[���̎��ʂ�ݒ肷��
	//mScene = CScene::ESTAGE2;

	////�V�[���𐶐����A�|�C���^��ݒ肷��
	//mpScene = new CSceneGameStage2();

	////�V�[���̎��ʂ�ݒ肷��
	//mScene = CScene::ESTAGE1;
	////�V�[���𐶐����A�|�C���^��ݒ肷��
	//mpScene = new CSceneGame();

	//�V�[���̎��ʂ�ݒ肷��
	mScene = CScene::ERACE1;
	//�V�[���𐶐����A�|�C���^��ݒ肷��
	mpScene = new CSceneRace();

	//���������N���X�̃��\�b�h���Ă΂��
	mpScene->Init();
}
//�X�V����
void CSceneManager::Update() {
	//�|�C���^��Update���Ă�
	mpScene->Update();

	//���̃V�[�����擾���قȂ邩����
	if (mScene != mpScene->GetNextScene()){
		mScene = mpScene->GetNextScene();
		delete mpScene;//���̃V�[���폜
		//�Y������V�[���𐶐�
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
