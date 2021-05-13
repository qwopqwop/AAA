#include "CRaceCourceB.h"
#include "CObj.h"
#include "CSceneTitle.h"

void CRaceCourceB::Init(){
	//�V�[���̐ݒ�
	mScene = ERACE2;

	CSceneRace::Init();

	//�G�̃|�C���g(�ړI�n)�̐ݒ�
	if (CSceneTitle::mDifficulty == 1){//��Փx�FEASY
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(2377.0f, 30.0f, -753.0f), 100.0f * 2);
		CEnemy::mPoint2 = new CPoint(CVector(3068.0f, 30.0f, -355.0f), 100.0f * 2);
		CEnemy::mPoint3 = new CPoint(CVector(3763.0f, 30.0f, 510.0f), 100.0f * 2);
		CEnemy::mPoint4 = new CPoint(CVector(3330.0f, 30.0f, 2133.0f), 100.0f * 2);
		CEnemy::mPoint5 = new CPoint(CVector(1719.0f, 30.0f, 3250.0f), 100.0f * 2);
		CEnemy::mPoint6 = new CPoint(CVector(1212.0f, 30.0f, 4516.0f), 100.0f * 2);
		CEnemy::mPoint7 = new CPoint(CVector(671.0f, 30.0f, 4970.0f), 100.0f * 2);
		CEnemy::mPoint8 = new CPoint(CVector(-2528.0f, 30.0f, 1832.0f), 100.0f * 2);
		CEnemy::mPoint9 = new CPoint(CVector(-3363.0f, 30.0f, 1220.0f), 100.0f * 2);
		CEnemy::mPoint10 = new CPoint(CVector(-3248.0f, 30.0f, 480.0f), 100.0f * 2);
		CEnemy::mPoint11 = new CPoint(CVector(-1500.0f, 30.0f, -80.0f), 100.0f * 2);
		CEnemy::mPoint12 = new CPoint(CVector(609.0f - 80.0f, 30.0f, -3665.0f + 100.0f), 100.0f);
	}
	else if (CSceneTitle::mDifficulty == 2){//��Փx�FNORMAL
		//���݁A���̐ݒ�
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(2377.0f, 30.0f, -753.0f), 100.0f * 2);
		CEnemy::mPoint2 = new CPoint(CVector(3068.0f, 30.0f, -355.0f), 100.0f * 2);
		CEnemy::mPoint3 = new CPoint(CVector(3763.0f, 30.0f, 510.0f), 100.0f * 2);
		CEnemy::mPoint4 = new CPoint(CVector(3330.0f, 30.0f, 2133.0f), 100.0f * 2);
		CEnemy::mPoint5 = new CPoint(CVector(1719.0f, 30.0f, 3250.0f), 100.0f * 2);
		CEnemy::mPoint6 = new CPoint(CVector(1212.0f, 30.0f, 4516.0f), 100.0f * 2);
		CEnemy::mPoint7 = new CPoint(CVector(671.0f, 30.0f, 4970.0f), 100.0f * 2);
		CEnemy::mPoint8 = new CPoint(CVector(-2528.0f, 30.0f, 1832.0f), 100.0f * 2);
		CEnemy::mPoint9 = new CPoint(CVector(-3363.0f, 30.0f, 1220.0f), 100.0f * 2);
		CEnemy::mPoint10 = new CPoint(CVector(-3248.0f, 30.0f, 480.0f), 100.0f * 2);
		CEnemy::mPoint11 = new CPoint(CVector(-1500.0f, 30.0f, -80.0f), 100.0f * 2);
		CEnemy::mPoint12 = new CPoint(CVector(609.0f-80.0f, 30.0f, -3665.0f+100.0f), 100.0f);
	}
	else if (CSceneTitle::mDifficulty == 3){//��Փx�FHARD
		//���݁A���̐ݒ�
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(2377.0f, 30.0f, -753.0f), 100.0f * 2);
		CEnemy::mPoint2 = new CPoint(CVector(3068.0f, 30.0f, -355.0f), 100.0f * 2);
		CEnemy::mPoint3 = new CPoint(CVector(3763.0f, 30.0f, 510.0f), 100.0f * 2);
		CEnemy::mPoint4 = new CPoint(CVector(3330.0f, 30.0f, 2133.0f), 100.0f * 2);
		CEnemy::mPoint5 = new CPoint(CVector(1719.0f, 30.0f, 3250.0f), 100.0f * 2);
		CEnemy::mPoint6 = new CPoint(CVector(1212.0f, 30.0f, 4516.0f), 100.0f * 2);
		CEnemy::mPoint7 = new CPoint(CVector(671.0f, 30.0f, 4970.0f), 100.0f * 2);
		CEnemy::mPoint8 = new CPoint(CVector(-2528.0f, 30.0f, 1832.0f), 100.0f * 2);
		CEnemy::mPoint9 = new CPoint(CVector(-3363.0f, 30.0f, 1220.0f), 100.0f * 2);
		CEnemy::mPoint10 = new CPoint(CVector(-3248.0f, 30.0f, 480.0f), 100.0f * 2);
		CEnemy::mPoint11 = new CPoint(CVector(-1500.0f, 30.0f, -80.0f), 100.0f * 2);
		CEnemy::mPoint12 = new CPoint(CVector(609.0f - 80.0f, 30.0f, -3665.0f + 100.0f), 100.0f);
	}
	else{
		//�B��HARD�̗\��
	}
	//�v���C���[�̐���
	mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
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
			mEnemys[i]->mpModel = &mCarWhite;
		}
		else if (i % 8 == 7){
			mEnemys[i]->mpModel = &mCarCyan;
		}
		/*mStartPoint[0] = 2222.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -2510.0f;
		mStartRotation = 0.0f;*/
		//�����̔z�u���W��ݒ肷��
		mEnemys[i]->mPosition = CVector(2222.0f, -13.538f, -2510.0f - 30.0f - 80.0f*i);
		if (i % 2 == 0){
			mEnemys[i]->mPosition.mX -= 80.0f;
		}
		mEnemys[i]->CCharacter::Update();
	}

	/* �������x�̍����������ɕ`�悷�� */
	//���Ԓn�_(���ɒʂ�Ȃ���1���������ƂɂȂ�Ȃ����A���Ԃ��΂��Ă������Ȃ�)
	new CObj(&mCheckPoint, CVector(2893.0f, -100.0f, 2473.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 101);
	new CObj(&mCheckPoint, CVector(-1020.0f, -100.0f, 4594.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 102);
	new CObj(&mCheckPoint, CVector(-1277.0f, -100.0f, -448.0f), CVector(0.0f, 0.0f, 0.0f), CVector(220.0f, 200.0f, 220.0f), 103);
	//�R�[�X�̐���
	new CObj(&mCource02Road, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f), 1);
	new CObj(&mCource02Wall, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f), 200);
	new CObj(&mCource02Jump, CVector(0.0f, -220.0f, 0.0f), CVector(0.0f, -139.3f, 0.0f), CVector(42.0f, 13.5f, 42.0f), 202);
	//�S�[���n�_
	new CObj(&mMiniGoal, CVector(2315.0f, -1.0f, -2300.0f), CVector(0.0f, 0.0f, 0.0f), CVector(4.2f, 10.0f, 4.2f), 99);
	new CObj(&mMiniGoal, CVector(2107.0f, -1.0f - 0.05f, -2300.0f), CVector(0.0f, 180.0f, 0.0f), CVector(4.2f, 10.0f, 4.2f), 99);

	//�D��x�ύX
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
}
void CRaceCourceB::Update(){
	CSceneRace::Update();
}