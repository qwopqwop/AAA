#include "CRaceCourceA.h"
#include "CObj.h"
#include "CObjWall.h"
#include "CObjFloor.h"
#include "CRoadManager.h"
#include "CObjCheckPoint.h"
#include "CObjNonCol.h"
#include "CObjBoost.h"
#include "CObjGrass.h"
#include "CObjJumper.h"
#include "CSceneTitle.h"

void CRaceCourceA::Init(){
	//�V�[���̐ݒ�
	mScene = ERACE1;

	CSceneRace::Init();

	CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
	CPoint *next, *first;
	//��납��O�Ɍ������Đ������Ă���
	first = next = CEnemy::mPoint12 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f, nullptr);
	//CPoint�������Ɏ��|�C���^��ݒ肵�Ă���
	next = CEnemy::mPoint11 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f, next);
	next = CEnemy::mPoint10 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f, next);
	next = CEnemy::mPoint9 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f, next);
	next = CEnemy::mPoint8 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f, next);
	next = CEnemy::mPoint7 = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f, next);
	next = CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 500.0f, next);
	next = CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 500.0f, next);
	next = CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 500.0f, next);
	next = CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 2150.0f), 500.0f, next);
	next = CEnemy::mPoint2 = new CPoint(CVector(258.0f, 30.0f, 2000.0f), 500.0f, next);
	next = CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 500.0f, next);
	//�ŏ��ɐ��������|�C���^�̎��|�C���^�̐ݒ�
	first->Set(CVector(340.0f, 30.0f, -1182.0f), 500.0f, next);

	
	//�v���C���[�̐���
	mCarShadow[0] = mPlayer = new CPlayer();
	mPlayer->mpModel = &mCarWhite;
	//�J�����̐���
	mCamRange = new CCameraRange();
	mCam = new CCameraPos();
	//mCam->mpModel = &mCarYellow;
	//�R�[�X�̐���//���������ƕǂŕ������ď����𕪂���
	mpGrounds[0] = new CObjFloor(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
//	mpGrounds[0] = new CRoadManager(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
//	mpGrounds[0] = new CRoadManager(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), mPlayer->mPosition, CVector(0.0f, 0.0f, 1.0f), 120.0f);//
	//new CRoadManager(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), mPlayer->mPosition, CVector(0.0f, 0.0f, -1.0f));//
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
	/*�����x�̍����������ɕ`�悷��*/
	//���Ԓn�_(���ɒʂ�Ȃ���1���������ƂɂȂ�Ȃ����A���Ԃ��΂��Ă������Ȃ�)
	mpGrounds[95] = new CObjCheckPoint(&mCheckPoint, CVector(50.0f, 15.0f, 2500.0f), CVector(-90.0f, 0.0f, -50.0f), CVector(2000.0f, 31.0f, 255.0f), 1);
	mpGrounds[96] = new CObjCheckPoint(&mCheckPoint, CVector(-1800.0f, 15.0f, 20.0f), CVector(-90.0f, 180.0f, 0.0f), CVector(750.0f, 31.0f, 255.0f), 2);
	mpGrounds[97] = new CObjCheckPoint(&mCheckPoint, CVector(-1100.0f, 15.0f, -2000.0f), CVector(-90.0f, 0.0f, 110.0f), CVector(750.0f, 31.0f, 255.0f), 3);

	/*mpGrounds[98] = new CObjCheckPoint(&mCube, CVector(0.0f, 55.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(250.0f, 41.0f, 250.0f), 11);
	mpGrounds[99] = new CObjFloor(&mCube3, CVector(0.0f, 555.0f, 510.0f), CVector(0.0f, 0.0f, 0.0f), CVector(250.0f, 41.0f, 250.0f));*/
	//CObjCheckPoint�̂悤��Render()����I�[�o�[���C�h���ē����ɂ����ꍇ�F�@�e�N�X�`���ɉe�͔��f���ꂸ�A���̃��f���͉e�ɂ��Ȃ�Ȃ�
	//�����ȃe�N�X�`�����g�p�����ꍇ�F�@�e�N�X�`���ɉe�����f�����(�e�̔Z�����e�N�X�`���̕s�����x)�A���̃��f���̉e����������

	//��������
	//�V�E�R�[�X�̐���
	for (int i = 0; i < 1; i++){
		////�R�[�X�̐���//���������ƕǂŕ������ď����𕪂���
		//mpGrounds[0] = new CObjFloor(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
		////new CRoadManager(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), mPlayer->mPosition, CVector(0.0f, 0.0f, -1.0f));//
		//�Ő��̐���(�ʍs���͑��x�ቺ)
		mpGrounds[1] = new CObjGrass(&mGrass01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
		//�R�[�X�ɍ�̔z�u(�ǈ���)
		mpGrounds[2] = new CObjFloor(&mFenceTop, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f));
		mpGrounds[5] = new CObjWall(&mFenceSide, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f));
		//���H�ƎŐ��̋��ڂ̃^�C���𐶐�(�����蔻�薳��)
		mpGrounds[93] = new CObjNonCol(&mCurb01, CVector(-360.0f, 5.0f - 33.0f + 0.05f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f));
	}
	//���E���^�C���ŃS�[������
	mpGrounds[10] = new CObjNonCol(&mGoalTile01, CVector(170.0f + 590.0f + 0.0f, -13.1f + 5.5f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));
	mpGrounds[11] = new CObjNonCol(&mGoalTile01, CVector(170.0f + 190.0f + 0.0f, -13.1f + 5.5f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));
	//for (int i = 0; i < 40; i++){
	//	if (i % 2 == 0){
	//		new CObjNonCol(&mTileBlack, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));//���^�C��
	//		mpGrounds[10 + i] = new CObjNonCol(&mTileWhite, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));//���^�C��
	//	}
	//	else{
	//		new CObjNonCol(&mTileBlack, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));//���^�C��
	//		mpGrounds[10 + i] = new CObjNonCol(&mTileWhite, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f));//���^�C��
	//	}
	//}
	//�S�[��(�Q�[�g��)
	for (int i = 0; i < 20; i++){
		if (i % 2 == 0){
			new CObjNonCol(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//���^�C��
			new CObjNonCol(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//���^�C��
		}
		else{
			new CObjNonCol(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//���^�C��
			new CObjNonCol(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f));//���^�C��
		}
	}

	
	
	//�Q�[�g�̒�����
	new CObjWall(&mPole, CVector(170.0f + 20.0f * -1 + 5.0f + 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0f, 10.0f));
	new CObjWall(&mPole, CVector(170.0f + 20.0f * 40 + 5.0f - 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0, 10.0f));
	//������
	mpGrounds[51] = new CObjBoost(&mDashBoard, CVector(234.0f, -13.1f + 3.0f, -980.0f), CVector(0.0f, 180.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f));
	mpGrounds[52] = new CObjBoost(&mDashBoard, CVector(-1500.0f, -13.1f + 3.0f, -50.0f), CVector(0.0f, 0.0f, 0.0f), CVector(2.0f, 2.0f, 2.0f));
	//�W�����v��
	mpGrounds[53] = new CObjJumper(&mOnBlock, CVector(240.0f, -13.1f + 3.0f, 1110.0f), CVector(-30.0f, 0.0f, 0.0f), CVector(77.0f, 5.0f, 50.0f));
	//�����܂ł�mpGround�ɂ�����

	//�D��x�ύX
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}

	for (int i = 0; i < ENEMYS_AMOUNT; i++){
		if (mEnemys[i]->mpModel==&mCarRed){
			printf("CPU%d RED\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarBlue){
			printf("CPU%d BLUE\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarBlack){
			printf("CPU%d BLACK\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarCyan){
			printf("CPU%d CYAN\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarGreen){
			printf("CPU%d GREEN\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarGray){
			printf("CPU%d GRAY\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarPink){
			printf("CPU%d PINK\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarWhite){
			printf("CPU%d WHITE\n", i + 1);
		}
		else if (mEnemys[i]->mpModel == &mCarYellow){
			printf("CPU%d YELLOW\n", i + 1);
		}
	}
}
void CRaceCourceA::Update(){
	CSceneRace::Update();
}