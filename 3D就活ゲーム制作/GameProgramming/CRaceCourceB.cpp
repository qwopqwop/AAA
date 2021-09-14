#include "CRaceCourceB.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CRoadManager.h"
#include "CObjCheckPoint.h"
#include "CObjNonCol.h"
#include "CObjJumper.h"
#include "CSceneTitle.h"

void CRaceCourceB::Init(){
	//�V�[���̐ݒ�
	mScene = ERACE2;

	CSceneRace::Init();
	
	//�v���C���[�̐���
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//�J�����̐���
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();

	//�R�[�X�S�̂̃T�C�Y����ݒ�
	float mtsize = 42.0f;
	float height = 13.5f;
	mpGrounds[0] = new CRoadManager(&mCource02Road, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(mtsize, height, mtsize), mPlayer->mPosition, CVector(0.0f, 0.0f, 1.0f), 120.0f, 0.0f);
	
	//�G�Ԃ̐���
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		mEnemys[i] = new CEnemy();
		if (i % 8 == 0){
			mEnemys[i]->mpModel = &mCarBlue;
		}
		else if (i % 8 == 1){
			mEnemys[i]->mpModel = &mCarPink;
		}
		else if (i % 8 == 2){
			mEnemys[i]->mpModel = &mCarRed;
		}
		else if (i % 8 == 3){
			mEnemys[i]->mpModel = &mCarGreen;
		}
		else if (i % 8 == 4){
			mEnemys[i]->mpModel = &mCarYellow;
		}
		else if (i % 8 == 5){
			mEnemys[i]->mpModel = &mCarBlack;
		}
		else if (i % 8 == 6){
			mEnemys[i]->mpModel = &mCarGray;
		}
		else if (i % 8 == 7){
			mEnemys[i]->mpModel = &mCarCyan;
		}
		//�����̔z�u���W��ݒ肷��
		mEnemys[i]->mPosition = CVector(2222.0f, -13.538f, -2510.0f - 30.0f - 80.0f*i);
		if (i % 2 == 0){
			mEnemys[i]->mPosition.mX -= 80.0f;
		}
		mEnemys[i]->CCharacter::Update();
	}

	/* �������x�̍����������ɕ`�悷�� */
	//���Ԓn�_(���ɒʂ�Ȃ���1���������ƂɂȂ�Ȃ����A���Ԃ��΂��Ă������Ȃ�)
	new CObjCheckPoint(&mCheckPoint, CVector(2893.0f, -100.0f, 2473.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 1);
	new CObjCheckPoint(&mCheckPoint, CVector(-1020.0f, -100.0f, 4594.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 2);
	new CObjCheckPoint(&mCheckPoint, CVector(-1277.0f, -100.0f, -448.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 3);

	//��������
	//�R�[�X�̐���
	mpGrounds[1] = new CObjWall(&mCource02Wall, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f));
	mpGrounds[2] = new CObjJumper(&mCource02Jump, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f));
	//�S�[���n�_�̃^�C��
	mpGrounds[3] = new CObjNonCol(&mMiniGoal, CVector(2315.0f, -1.0f, -2300.0f), CVector(0.0f, 0.0f, 0.0f), CVector(4.2f, 10.0f, 4.2f));
	mpGrounds[4] = new CObjNonCol(&mMiniGoal, CVector(2107.0f, -1.0f - 0.05f, -2300.0f), CVector(0.0f, 180.0f, 0.0f), CVector(4.2f, 10.0f, 4.2f));

	//�D��x�ύX
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		int cpu_number = i + 1;
		if (mEnemys[i]->mpModel == &mCarRed){
			printf("CPU%d RED\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarBlue){
			printf("CPU%d BLUE\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarBlack){
			printf("CPU%d BLACK\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarCyan){
			printf("CPU%d CYAN\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarGreen){
			printf("CPU%d GREEN\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarGray){
			printf("CPU%d GRAY\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarPink){
			printf("CPU%d PINK\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarWhite){
			printf("CPU%d WHITE\n", cpu_number);
		}
		else if (mEnemys[i]->mpModel == &mCarYellow){
			printf("CPU%d YELLOW\n", cpu_number);
		}
	}
}
void CRaceCourceB::Update(){
	CSceneRace::Update();
}