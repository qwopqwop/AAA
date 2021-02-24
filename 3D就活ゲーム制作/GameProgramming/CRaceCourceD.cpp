#include "CRaceCourceD.h"
#include "CObj.h"
#include "CSceneTitle.h"

void CRaceCourceD::Init(){
	//�V�[���̐ݒ�
	mScene = ERACE4;

	CSceneRace::Init();
	
	/*�J�����ł�*/
	//�G�̃|�C���g(�ړI�n)�̐ݒ�
	if (CSceneTitle::mDifficulty == 1){//��Փx�FEASY
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f * 2 * 4);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f * 2 * 4);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f * 2 * 4);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f * 2 * 4);
		CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f * 2 * 4);
		CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f * 2 * 4);
		CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f * 2 * 4);
		CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f * 2 * 4);
	}
	else if (CSceneTitle::mDifficulty == 2){//��Փx�FNORMAL
		//���݁A���̐ݒ�
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f * 2 * 4);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f * 2 * 4);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f * 2 * 4);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f * 2 * 4);
		CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f * 2 * 4);
		CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f * 2 * 4);
		CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f * 2 * 4);
		CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f * 2 * 4);
	}
	else if (CSceneTitle::mDifficulty == 3){//��Փx�FHARD
		//���݁A���̐ݒ�
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f * 2 * 4);
		CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f * 2 * 4);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f * 2 * 4);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f * 2 * 4);
		CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 100.0f * 2 * 4);
		CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 100.0f * 2 * 4);
		CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 100.0f * 2 * 4);
		CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 100.0f * 2 * 4);
		CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 100.0f * 2 * 4);
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
			mEnemys[i]->mpModel = &mCarGray;
		}
		else if (i % 8 == 7){
			mEnemys[i]->mpModel = &mCarCyan;
		}
		//�����̔z�u���W��ݒ肷��
		mEnemys[i]->mPosition = CVector(350.0f, -13.538f, -130.0f - 80.0f*i);
		if (i % 2 == 0){
			mEnemys[i]->mPosition.mX -= 80.0f;
		}
		mEnemys[i]->CCharacter::Update();
	}

	////�؂蕨//�G���R�[�X�ɋ߂Â��Əd���Ȃ�c
	//new CObj(&mCource04, CVector(-360.0f, 5.0f - 33.0f, -230.0f), CVector(), CVector(5110.0f, 5110.0f, 5110.0f), 1);

	/*new CObj(&mCource04Water, CVector(0.0f, -60.0f - 33.0f, 0.0f), CVector(), CVector(100.0f, 100.0f, 100.0f), 99);
	new CObj(&mCource04A, CVector(0.0f, -60.0f - 33.0f, 0.0f), CVector(), CVector(40.0f, 40.0f, 40.0f), 1);*/
	//new CObj(&mCource04Water, CVector(0.0f, -260.0f - 33.0f, 0.0f), CVector(), CVector(100.0f, 100.0f, 100.0f), 99);

	//�z��ɕϊ����ăR�[�X�𐶐�
	int edit[5][4] = {
		{ 24, 12, 12, 21 },
		{ 23, 12, 21, 11 },
		{ 24, 21, 11, 11 },
		{ 11, 23, 22, 11 },
		{ 23, 12, 12, 22 }
	};
	int size = 6;//�R�[�X�̏k��
	for (int i = 0; i < 5; i++){
		for (int j = 0; j < 4; j++){
			if (edit[i][j] == 11){
				new CObj(&mTile_Straight01, CVector(-100.0f * size * j, 0.0f, -100.0f * size * i), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size), 1);//I��
			}
			else if(edit[i][j] == 12){
				new CObj(&mTile_Straight02, CVector(-100.0f * size * j, 0.0f, -100.0f * size * i), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size), 1);//�ꎚ
			}
			else if (edit[i][j] == 21){
				new CObj(&mTile_Curve03, CVector(-100.0f * size * j, 0.0f, -100.0f * size * i), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size), 1);//0���`3��
			}
			else if (edit[i][j] == 22){
				new CObj(&mTile_Curve04, CVector(-100.0f * size * j, 0.0f, -100.0f * size * i), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size), 1);//3���`6��
			}
			else if (edit[i][j] == 23){
				new CObj(&mTile_Curve01, CVector(-100.0f * size * j, 0.0f, -100.0f * size * i), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size), 1);//6���`9��
			}
			else if (edit[i][j] == 24){
				new CObj(&mTile_Curve02, CVector(-100.0f * size * j, 0.0f, -100.0f * size * i), CVector(), CVector(1.0f * size, 1.0f* size, 1.0f* size), 1);//9���`12��
			}
		}
	}
	//new CObj(&mTile_Curve01, CVector(0.0f, 0.0f, 500.0f*6), CVector(), CVector(6.0f, 6.0f, 6.0f), 1);//6���`9��
	//new CObj(&mTile_Curve02, CVector(0.0f, 0.0f, 100.0f*6), CVector(), CVector(6.0f, 6.0f, 6.0f), 1);//9���`12��
	//new CObj(&mTile_Curve03, CVector(0.0f, 0.0f, 200.0f*6), CVector(), CVector(6.0f, 6.0f, 6.0f), 1);//0���`3��
	//new CObj(&mTile_Curve04, CVector(0.0f, 0.0f, 300.0f*6), CVector(), CVector(6.0f, 6.0f, 6.0f), 1);//3���`6��
	//new CObj(&mTile_Straight01, CVector(0.0f, 0.0f, 0.0f*6), CVector(), CVector(6.0f, 6.0f, 6.0f), 1);//I��
	//new CObj(&mTile_Straight02, CVector(0.0f, 0.0f, 400.0f*6), CVector(), CVector(6.0f, 6.0f, 6.0f), 1);//�ꎚ

	//�D��x�ύX
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}
}
void CRaceCourceD::Update(){
	CSceneRace::Update();

}