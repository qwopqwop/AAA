#include "CSceneRace.h"
//
#include "CSceneTitle.h"
//
#include "CCamera.h"
//
#include "CText.h"
//
#include "CTaskManager.h"
//
#include "CCollisionManager.h"
////
#include "CRock.h"
//
#include "CObj.h"
//
#include "CItem.h"
//
#include "CKey.h"
//
#include "CBullet.h"

//�X�}�[�g�|�C���^�̐���
std::shared_ptr<CTexture> TextureExp(new CTexture());
std::shared_ptr<CTexture> TextureHit(new CTexture());

extern CSound BGM;
extern CSound SoundCountDown;
extern CSound SoundStart;
extern CSound SoundGoal;

//�x�X�g�^�C���̏����L�^��2��00�b00
int CSceneRace::mBestTime = 20000;
int CSceneRace::mRecord_A = 10000;
int CSceneRace::mRecord_B = 13000;
int CSceneRace::mRecord_C = 20000;
int CSceneRace::mRecord_D = 30000;

CSceneRace::~CSceneRace() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneRace::Init() {
	//�V�[���̐ݒ�
	mScene = ERACE1;
	

	////�|�C���g�̐ݒ�
	//CEnemy::mPointSize = 5;//�|�C���g���̐ݒ�
	//CEnemy::mPoint = new CPoint[CEnemy::mPointSize];
	//CEnemy::mPoint[0].Set(CVector(135.0f, 30.0f, 200.0f), 40.0f);
	//CEnemy::mPoint[1].Set(CVector(735.0f, 30.0f, 200.0f), 40.0f);
	//CEnemy::mPoint[2].Set(CVector(435.0f, 30.0f, 500.0f), 40.0f);
	//CEnemy::mPoint[3].Set(CVector(435.0f, 30.0f, -100.0f), 40.0f);
	//CEnemy::mPoint[4].Set(CVector(435.0f, 30.0f, 200.0f), 40.0f);//[0]����[3]�̒��S
	
	if (CSceneTitle::mDifficulty == 1){
		CEnemy::mPointSize = 6;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 40.0f);
		CEnemy::mPoint2 = new CPoint(CVector(558.0f, 30.0f, 1800.0f), 40.0f);
		//CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1700.0f), 40.0f);
		//CEnemy::mPoint2 = new CPoint(CVector(-958.0f, 30.0f, 2600.0f), 40.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 1700.0f), 40.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 40.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 40.0f);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 40.0f);
	}
	else if (CSceneTitle::mDifficulty == 2){
		CEnemy::mPointSize = 6;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1300.0f), 40.0f);
		CEnemy::mPoint2 = new CPoint(CVector(558.0f, 30.0f, 1800.0f), 40.0f);
		//CEnemy::mPoint = new CPoint(CVector(413.0f, 30.0f, 1700.0f), 40.0f);
		//CEnemy::mPoint2 = new CPoint(CVector(-958.0f, 30.0f, 2600.0f), 40.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-1350.0f, 30.0f, 1700.0f), 40.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-1601.0f, 30.0f, -1702.0f), 40.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-500.0f, 30.0f, -2000.0f), 40.0f);
		CEnemy::mPoint6 = new CPoint(CVector(340.0f, 30.0f, -1182.0f), 40.0f);
	}
	else if (CSceneTitle::mDifficulty == 3){//��Փx�FHARD
		CEnemy::mPointSize = 12;//�|�C���g���̐ݒ�
		CEnemy::mPoint = new CPoint(CVector(260.0f, 30.0f, 777.0f), 40.0f);
		CEnemy::mPoint2 = new CPoint(CVector(259.0f, 30.0f, 1300.0f), 50.0f);
		CEnemy::mPoint3 = new CPoint(CVector(-150.0f, 30.0f, 2058.0f), 65.0f);
		CEnemy::mPoint4 = new CPoint(CVector(-555.0f, 30.0f, 2111.0f), 65.0f);
		CEnemy::mPoint5 = new CPoint(CVector(-1039.0f, 30.0f, 2062.0f), 65.0f);
		CEnemy::mPoint6 = new CPoint(CVector(-1400.0f, 30.0f, 1700.0f), 60.0f);
		CEnemy::mPoint7 = new CPoint(CVector(-1511.0f, 30.0f, -317.0f), 40.0f);
		CEnemy::mPoint8 = new CPoint(CVector(-1400.0f, 30.0f, -1079.0f), 40.0f);
		CEnemy::mPoint9 = new CPoint(CVector(-913.0f, 30.0f, -1637.0f), 40.0f);
		CEnemy::mPoint10 = new CPoint(CVector(-500.0f, 30.0f, -1700.0f), 40.0f);
		CEnemy::mPoint11 = new CPoint(CVector(-160.0f, 30.0f, -1600.0f), 40.0f);
		CEnemy::mPoint12 = new CPoint(CVector(193.0f, 30.0f, -1182.0f), 50.0f);
	}
	else{
		//���̘g�͌����Y�ɂ���\��(�B���v�f�ɂ���\��)
	}

	/*for (int i = 0; i < CEnemy::mPointSize; i++){
		CEnemy::mPoint.Set
	}*/

	/*new CPoint(CVector(135.0f, 30.0f, 200.0f), 40.0f, 1);
	new CPoint(CVector(135.0f, 30.0f, 200.0f), 40.0f, 1);
	new CPoint(CVector(135.0f, 30.0f, 200.0f), 40.0f, 1);
	new CPoint(CVector(135.0f, 30.0f, 200.0f), 40.0f, 1);*/

	/*CEnemy::mPoint[0].Set(CVector(35.0f, 25.0f, 100.0f), 10.0f);
	CEnemy::mPoint[1].Set(CVector(35.0f, 5.0f, 0.0f), 10.0f);
	CEnemy::mPoint[2].Set(CVector(-35.0f, 45.0f, 50.0f), 10.0f);*/

	

	//new CObj(&mOnBlock, CVector(0.0f, 0.0f, 750.0f), CVector(-40.0f, 270.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);


	//�I�̎c���̏�����
	CItem::mTargetAmount = 0;

	//�����e�N�X�`���̓ǂݍ���
	TextureExp->Load("exp.tga");
	//�Փ˃e�N�X�`���̓ǂݍ���
	TextureHit->Load("effect\\[Attack]Hit01_panop.tga");

	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	CText::mFont2.Load("font2nd.tga");
	CText::mFont2.SetRowCol(8, 256 / 16);
	//mFont3:�Ō��mFont3�ɐݒ肵���V�[���ł̃t�H���g(��Title��ʂ̎�)

	//�w�i�̓ǂݍ���
	mSky.Load("sky.obj", "sky.mtl");
	//��̓ǂݍ���
	mRock.Load("Rock1.obj", "Rock1.mtl");
	//�Ԃ̓ǂݍ���
	mRover.Load("Rover1.obj", "Rover1.mtl");
	mCarRed.Load("Rover1.obj", "material\\racing_mat\\single_color\\red.mtl");
	mCarBlue.Load("Rover1.obj", "material\\racing_mat\\single_color\\blue.mtl");
	mCarGreen.Load("Rover1.obj", "material\\racing_mat\\single_color\\green.mtl");
	mCarYellow.Load("Rover1.obj", "material\\racing_mat\\single_color\\yellow.mtl");
	mCarPink.Load("Rover1.obj", "material\\racing_mat\\single_color\\pink.mtl");
	mCarCyan.Load("Rover1.obj", "material\\racing_mat\\single_color\\cyan.mtl");
	mCarWhite.Load("Rover1.obj", "material\\racing_mat\\single_color\\white.mtl");
	mCarBlack.Load("Rover1.obj", "material\\racing_mat\\single_color\\black.mtl");
//	mRover.Load("sphere.obj", "sphere.mtl");
	//�����̂̓ǂݍ���
	mCube.Load("cube.obj", "material\\cube.mtl");
	//�n�ʂ̓ǂݍ���
	mPlane.Load("plane.obj", "plane.mtl");
	//�K�i�̓ǂݍ���
	mStairs.Load("cube.obj", "material\\stairs.mtl");
	//�I�̓ǂݍ���
	mTarget.Load("Rock1.obj", "material\\target.mtl");
	//ON�u���b�N(�H)�̓ǂݍ���
	mOnBlock.Load("cube.obj", "material\\on.mtl");

	mItem.Load("Rock1.obj", "Rock1.mtl");
	

	mCube2.Load("cube.obj", "cube2.mtl");
	mCube3.Load("cube.obj", "cube3.mtl");
	//���Ԓn�_�̓ǂݍ���(�����A�|���S��1���̂�)
	mCheckPoint.Load("plane.obj", "cube2.mtl");

	mWater.Load("cube.obj", "water_sumple.mtl");


	//�o�l�̓ǂݍ���
	mSpringS.Load("�΂�.obj", "cube.mtl");
	mSpringL.Load("�΂ːL��.obj", "cube.mtl");

	//���^�C���̓ǂݍ���
	mTileBlack.Load("cube.obj", "material\\racing_mat\\tile_black.mtl");
	mTileWhite.Load("cube.obj", "material\\racing_mat\\tile_white.mtl");
	mRWTile.Load("material\\racing_mat\\NewNewR-W.obj", "material\\racing_mat\\NewNewR-W.mtl");
	//�R�[�X�̓ǂݍ���
	//mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");
	mCource01.Load("material\\racing_mat\\CourceNew01.obj", "material\\racing_mat\\CourceNew01.mtl");
	//�Ő��̓ǂݍ���
	mGrass01.Load("material\\racing_mat\\GrassNew01.obj", "material\\racing_mat\\GrassNew01.mtl");
	//��(��)�̓ǂݍ���
	mFence01.Load("material\\racing_mat\\Cource01Wall.obj", "material\\racing_mat\\Cource01Wall.mtl");
	//��̓ǂݍ���
	mFenceTop.Load("material\\racing_mat\\FenceTopNew.obj", "material\\racing_mat\\FenceTopNew.mtl");
	mFenceSide.Load("material\\racing_mat\\FenceSideNew.obj", "material\\racing_mat\\FenceSideNew.mtl");

	mPole.Load("cube.obj", "material\\soil.mtl");

	mDashBoard.Load("material\\racing_mat\\dashboard.obj", "material\\racing_mat\\dashboard.mtl");

	mCarsol.Load("material\\racing_mat\\minicarsol.obj", "material\\racing_mat\\minicarsol.mtl");
	mMiniGoal.Load("material\\racing_mat\\minigoal.obj", "material\\racing_mat\\minigoal.mtl");

	CBullet::mBullet.Load("�e�e.obj", "�e�e.mtl");

	//�X�e�[�W1BGM�̓ǂݍ���
	if (CSceneTitle::mMode == 1){
		BGM.Load("BGM\\(���ʒ�����)Pops�M�^�[_No.01.wav");
		mMaxLap = 1;
		mBestTime = mRecord_A;
	}
	else if (CSceneTitle::mMode == 2){
		BGM.Load("BGM\\game_maoudamashii_1_battle34.wav");
		mMaxLap = 2;
		mBestTime = mRecord_B;
	}
	else if (CSceneTitle::mMode == 3){
		BGM.Load("BGM\\Crazy_Machine.wav");
		mMaxLap = 3;
		mBestTime = mRecord_C;
	}
	else if (CSceneTitle::mMode == 4){
		BGM.Load("BGM\\(������)bgm_maoudamashii_neorock33.wav");
		mMaxLap = 5;
		mBestTime = mRecord_D;
	}

	//���ʉ��̓ǂݍ���
	SoundCountDown.Load("SE\\Countdown01-5.wav");
	SoundStart.Load("SE\\Countdown01-6.wav");
	SoundGoal.Load("SE\\tm2_whistle000.wav");

	mSumple.Load("UnderGround\\passage2.obj", "UnderGround\\passage2.mtl");

	msumple2.Load("material\\racing_mat\\stage2\\tekitou.obj", "material\\racing_mat\\stage2\\tekitou.mtl");

	//�����x�̍����������ɕ`�悷��

	/*for (int i = 0; i < 12; i++){
	new CObj(&mCube3, CVector(43.0f, 15.0f + i * 7.0f, 118.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 2.0f, 10.0f), 9);//��^���[
	}*/
	//new CObj(&mCube3, CVector(-73.0f, 5.0f, 100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(30.0f, 45.0f, 1.0f), 13);
	//new CObj(&mCube3, CVector(10.0f, 23.0f, 100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(40.0f, 3.0f, 10.5f), 9);//�������̃K���X

	mPlayer = new CPlayer();
	mPlayer->mpModel = &mRover;

	mEnemy1 = new CEnemy();
	mEnemy1->mpModel = &mCarBlack;
	mEnemy1->mPosition = CVector(250.0f, -13.538f, -100.0f);
	mEnemy1->CCharacter::Update();

	mEnemy2 = new CEnemy();
	mEnemy2->mpModel = &mCarWhite;
	mEnemy2->mPosition = CVector(350.0f, -13.538f, -100.0f);
	mEnemy2->CCharacter::Update();

	for (int i = 0; i < 5; i++){
		mEnemys[i] = new CEnemy();
		if (i % 5 == 0){
			mEnemys[i]->mpModel = &mCarBlue;
		}
		else if (i % 5 == 1){
			mEnemys[i]->mpModel = &mCarPink;
		}		
		else if (i % 5 == 2){
			mEnemys[i]->mpModel = &mCarRed;
		}
		else if (i % 5 == 3){
			mEnemys[i]->mpModel = &mCarGreen;
		}
		else if (i % 5 == 4){
			mEnemys[i]->mpModel = &mCarYellow;
		}
		//�����̔z�u���W��ݒ肷��
		mEnemys[i]->mPosition = CVector(350.0f - 25.0f*i, -13.538f, -140.0f);
		if (i % 2 == 1){
			mEnemys[i]->mPosition.mZ -= 7.0f;
		}
		mEnemys[i]->CCharacter::Update();
	}

	//��̐����@���f��mRock�@�ʒu|-20.0 0.0 20.0|
	//��]|0.0 0.0 0.0|�@�g��|5.0 5.0 5.0|
	//	new CRock(&mRock, CVector(-20.0f, 0.0f, 20.0f), CVector(), CVector(5.0f, 5.0f, 5.0f));
	//�����̂̐���
	//	new CObj(&mCube, CVector(0.0f, 0.0f, 60.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 10.0f, 1.0f), 1);
	//�n�ʂ̐���
//	new CObj(&mPlane, CVector(0.0f, 0.0f, 0.0f), CVector(), CVector(100.0f, 1.0f, 100.0f), 1);
	//��H�̐���
	
		
	////���R�[�X�̐���
	//new CObj(&mCube, CVector(0.0f, -20.0f, 0.0f + 100.0f * 29), CVector(0.0f, 0.0f, 0.0f), CVector(100.0f, 10.0f, 100.0f * 29 + 100.0f), 1);
	////�S�[���n�_�̏��^�C��
	//for (int i = 0; i < 10; i++){
	//	if (i % 2 == 0){
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, -2.0f + 0.1f, 5700.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, -2.0f + 0.1f, 5700.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
	//	}
	//	else{
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, -2.0f + 0.1f, 5700.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, -2.0f+0.1f, 5700.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
	//	}
	//}
	////�S�[��(�Q�[�g��)
	//for (int i = 0; i < 10; i++){
	//	if (i % 2 == 0){
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, 110.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//���^�C��
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, 110.0f + 10.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//���^�C��
	//	}
	//	else{
	//		new CObj(&mTileBlack, CVector(-90.0f + 20.0f*i, 110.0f + 10.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//���^�C��
	//		new CObj(&mTileWhite, CVector(-90.0f + 20.0f*i, 110.0f, 5700.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 5.0f), 99);//���^�C��
	//	}
	//}

	// 1 2 3 4 5
	//����������
	//����������
	//�`�F�b�N�������₷�����郁�\�b�h�H
	//CChecker(�p�^�[��1,�p�^�[��2,�ǂ��܂Ŗ͗l���J��Ԃ���,,,�����`�̃T�C�Y,����)
	//CChecker(&mTileBlack ,&mTileWhite, 5, 10.0f, 0.5f)

	//new CObj(&mCube, CVector(0.0f, -5.55f, 13.0f), CVector(-5.4f, 0.0f, 0.0f), CVector(10.0f, 4.0f, 28.0f), 1);//���H
	////for (int l = 0; l < 5; l++){
	////	new CObj(&mStairs, CVector(0.0f, -1.0f, 43.0f + l * 6.0f), CVector(50.0f - 10.0f*l , 0.0f, 0.0f), CVector(10.0f, 3.0f + 3.0f*l, 3.0f), 1);//�K�i
	////}
	//for (int l = 0; l < 5; l++){
	//	new CObj(&mStairs, CVector(-40.0f, -1.0f + l * 3.0f, 43.0f + l * 10.0f), CVector(-10.0f - 10.0f*l, 0.0f, 0.0f), CVector(10.0f, 3.0f, 3.0f + 3.0f*l), 1);//�K�i
	//}
	
	//���Ԓn�_(���ɒʂ�Ȃ���1���������ƂɂȂ�Ȃ����A���Ԃ��΂��Ă������Ȃ�)
//	new CObj(&mCube3, CVector(0.0f, -30.0f, 2100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(155.0f, 155.0f, 155.0f), 101);
//	new CObj(&mCube3, CVector(-1650.0f, -30.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(155.0f, 155.0f, 155.0f), 102);
//	new CObj(&mCube3, CVector(100.0f, -30.0f, -1700.0f), CVector(0.0f, 0.0f, 0.0f), CVector(155.0f, 155.0f, 155.0f), 103);

	/*new CObj(&mPlane, CVector(0.0f, 0.0f, -220.0f), CVector(0.0f, 0.0f, 0.0f), CVector(55.0f, 1.0f, 55.0f), 1);
	new CObj(&mCheckPoint, CVector(0.0f, 15.0f, -400.0f), CVector(0.0f, 0.0f, 0.0f), CVector(55.0f, 1.0f, 55.0f), 1);*/
	new CObj(&mCheckPoint, CVector(50.0f, 15.0f, 2500.0f), CVector(-90.0f, 0.0f, -50.0f), CVector(777.0f, 1.0f, 255.0f), 101);
	new CObj(&mCheckPoint, CVector(-1800.0f, 15.0f, 20.0f), CVector(-90.0f, 180.0f, 0.0f), CVector(750.0f, 1.0f, 255.0f), 102);
	new CObj(&mCheckPoint, CVector(-1100.0f, 15.0f, -2000.0f), CVector(-90.0f, 0.0f, 110.0f), CVector(750.0f, 1.0f, 255.0f), 103);

	//new CObj(&mCheckPoint, CVector(0.0f, 20.0f, 2100.0f), CVector(90.0f, 0.0f, 0.0f), CVector(255.0f, 1.0f, 255.0f), 25);

	//�W�����v��
	new CObj(&mOnBlock, CVector(0.0f, 0.0f, 450.0f), CVector(-40.0f, 0.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);

	new CObj(&mOnBlock, CVector(0.0f, 0.0f, 550.0f), CVector(-40.0f, 90.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);
	new CObj(&mOnBlock, CVector(0.0f, 0.0f, 650.0f), CVector(-40.0f, 180.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);
	new CObj(&mOnBlock, CVector(0.0f, 0.0f, 750.0f), CVector(-40.0f, 270.0f, 0.0f), CVector(60.0f, 5.0f, 40.0f), 1);

	

//	new CObj(&mCarsol, CVector(0.0f, 4000.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 10.0f, 10.0f), 575);//�~�j�}�b�v�̃J�[�\��

	//�΂�
	new CItem(&mSpringL, CVector(0.0f, -5.0f, 80.0f), CVector(), CVector(11.0f, 11.0f, 11.0f), 2);//�o�l

	//new CItem(&mBoat, CVector(35.0f, 7.0f, 30.0f), CVector(), CVector(2.5f, 2.5f, 2.5f), 7);//�{�[�g
	//new CObj(&mOnBlock, CVector(37.5f, 0.0f, 30.0f), CVector(), CVector(23.0f, 12.0f, 13.0f), 10);//�{�[�g���ޔ�
	//new CObj(&mCube, CVector(30.0f, 0.0f, 30.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 2.0f, 10.0f), 2);

	//�V�E�R�[�X�̐���
	for (int i = 0; i < 1; i++){
		//�R�[�X�̐���//���������ƕǂŕ������ď����𕪂���\��
		new CObj(&mCource01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), 1);
		//old CObj(&mCource01, CVector(-350.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(20.0f, 1.0f, 20.0f), 1);
		mXXX = -360.0f; mYYY = 5.0f - 33.0f; mZZZ = 230.0f;

		//�Ő��̐���(�ʍs���͑��x�ቺ)
		new CObj(&mGrass01, CVector(-360.0f, 5.0f - 33.0f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), 112);
		
		//�R�[�X�ɍ�̔z�u(�ǈ���)
	//	new CObj(&mFence01, CVector(-360.0f, 5.0f - 60.0f, 230.0f), CVector(), CVector(50.0f, 4.0f, 50.0f), 1);
	//	new CObj(&mFence01, CVector(-360.0f, -50.0f, 230.0f), CVector(), CVector(50.0f, 3.5f, 50.0f), 200);

		new CObj(&mFenceTop, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f), 1);
		new CObj(&mFenceSide, CVector(-360.0f, -70.0f - 35.0f, 230.0f), CVector(), CVector(50.0f, 5.5f + 1.5f, 50.0f), 200);

		//���H�ƎŐ��̋��ڂ̃^�C���𐶐�(�����蔻�薳��)
		new CObj(&mRWTile, CVector(-360.0f, 5.0f - 33.0f+0.05f, 230.0f), CVector(), CVector(50.0f, 2.0f, 50.0f), 99);
	}
	//���E���^�C���ŃS�[������
	for (int i = 0; i < 40; i++){
	//	mStartPoint[0] = 300.0f;  mStartPoint[1] = 63.0f;  mStartPoint[2] = -50.0f;
		if (i % 2 == 0){
			new CObj(&mTileBlack, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
			new CObj(&mTileWhite, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
		}
		else{
			new CObj(&mTileBlack, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f + 20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
			new CObj(&mTileWhite, CVector(170.0f + 20.0f*i, -13.1f + 0.5f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
			//new CObj(&mTileWhite, CVector(300.0f + 20.0f*i, -2.0f + 0.1f, -20.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 1.0f, 10.0f), 99);//���^�C��
		}
	}
	////�S�[��(�Q�[�g��)
	//for (int i = 0; i < 40; i++){
	//	if (i % 2 == 0){
	//		new CObj(&mTileBlack, CVector(170.0f + 20.0f*i + 5.0f, 110.0f + 74.0f * 2 * 2, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 4.0f, 5.0f), 99);//���^�C��
	//		new CObj(&mTileWhite, CVector(170.0f + 20.0f*i + 5.0f, 110.0f + 10.0f + 74.0f * 2 * 2, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 4.0f, 5.0f), 99);//���^�C��
	//	}
	//	else{
	//		new CObj(&mTileBlack, CVector(170.0f + 20.0f*i + 5.0f, 110.0f + 10.0f + 74.0f * 2 * 2, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 4.0f, 5.0f), 99);//���^�C��
	//		new CObj(&mTileWhite, CVector(170.0f + 20.0f*i + 5.0f, 110.0f + 74.0f * 2 * 2, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(10.0f, 4.0f, 5.0f), 99);//���^�C��
	//	}
	//}
	//�S�[��(�Q�[�g��)
	for (int i = 0; i < 20; i++){
		if (i % 2 == 0){
			new CObj(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//���^�C��
			new CObj(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//���^�C��
		}
		else{
			new CObj(&mTileBlack, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 10.0f + 200.0f - 5.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//���^�C��
			new CObj(&mTileWhite, CVector(170.0f + 40.0f*i + 5.0f + 10.0f, 110.0f + 200.0f - 5.0f - 10.0f - 20.0f - 10.0f, -14.0f), CVector(90.0f, 0.0f, 0.0f), CVector(20.0f, 4.9f, 20.0f), 99);//���^�C��
		}
	}
	//�|�[��
	new CObj(&mTileWhite, CVector(170.0f + 20.0f * -1 + 5.0f + 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0f, 10.0f), 1);//��
	new CObj(&mTileWhite, CVector(170.0f + 20.0f * 40 + 5.0f - 5.0f, -13.1f - 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 174.0, 10.0f), 1);//��
	

	//������
	new CObj(&mDashBoard, CVector(260.0f, -13.1f + 3.0f, 800.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 111);

	new CObj(&mDashBoard, CVector(234.0f, -13.1f + 3.0f, -980.0f), CVector(0.0f, 180.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 111);

	new CObj(&mDashBoard, CVector(-1500.0f, -13.1f + 3.0f, -200.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 111);

	new CObj(&mDashBoard, CVector(-500.0f, -13.1f + 3.0f, -1900.0f), CVector(0.0f, -90.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 111);

	new CObj(&mTileWhite, CVector(0.0f, -11.1f, 900.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 150.0f, 10.0f), 200);//��

	//new CObj(&mTileWhite, CVector(230.0f, -11.1f, 1300.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 50.0f, 50.0f), 200);//��

	//new CObj(&mTileWhite, CVector(330.0f, -11.1f, 1110.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 1000.0f, 10.0f), 200);//��

//	new CObj(&mTileBlack, CVector(0.0f, -1010.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50000.0f, 1.0f, 50000.0f), 99);//���^�C��

	//�V�F�[�_�[�����ƌ��d
	//new CObj(&mSumple, CVector(430.0f, 10.0f + 3123.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(50.0f, 50.0f, 50.0f), 99);
	//�����蔻������Ȃ���Α��v

	/*for (int i = 0; i < 16; i++){
		new CObj(&mDashBoard, CVector(260.0f - 40.0f*i , 13.1f + 10.0f, 800.0f), CVector(0.0f, 270.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 31);
		new CObj(&mDashBoard, CVector(260.0f - 40.0f*i-20.0f, 13.1f + 10.0f, 800.0f), CVector(0.0f, 270.0f, 0.0f), CVector(0.9f, 0.9f, 0.9f), 31);
	}*/


	//�X�e�[�W2�̃}�e���A��
	if (CSceneTitle::mMode == 2){
		new CObj(&msumple2, CVector(320.0f, -58.0f, 450.0f), CVector(0.0f, 180.0f, 0.0f), CVector(4.0f, 3.0f, 3.0f), 1);
	}

	
	mCamY = 0.0f;
	mPutCol = false;

	mTextBlinkTime = 0;

	mFrame = 0;
	mCountDown = 3+1;

	//���[�X�̓J�E���g�_�E�����I����Ă���J�n
	isStartRace = isGoal = false;
	//���Ԃ̃��Z�b�g
	mTime = 0;
		
	//���b�v���̏�����
	mLap = 1;
	//�L�^�X�V���ĂȂ����
	isNewRecord = false;
	
	//�J�������_
	mCamPoV = 1;

	//TaskManager.ChangePriority(&mPlayer, 15);
	CTaskManager::Get()->ChangePriority(mPlayer, 15);
	//CTaskManager::Get()->ChangePriority(, 15);
	CTaskManager::Get()->ChangePriority(mEnemy1, 15);
	CTaskManager::Get()->ChangePriority(mEnemy2, 15);
	for (int i = 0; i < 5; i++){
		CTaskManager::Get()->ChangePriority(mEnemys[i], 15);
	}

	

	BGM.Repeat();
}


void CSceneRace::Update() {
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	//���_�����߂�
//	e = CVector(-2.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrix   * mPlayer->mMatrixScale * mPlayer->mMatrixRotate * mPlayer->mMatrixTranslate;
//	e = CVector(-2.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale * mPlayer->mMatrixRotate * mPlayer->mMatrixTranslate;
	if (mCamPoV == 1){
		e = CVector(0.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////�����_�����߂�
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}
	else if (mCamPoV == 2){
		e = CVector(0.0f, 0.0f + 0.5f, -40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////�����_�����߂�
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}
	else if (mCamPoV == 3){//������f�����_
		e = CVector(0.0f, 17.0f, 40.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY)
			* mPlayer->mMatrixTranslate
			+ CVector(0.0f, 0.0f, 0.0f);
		////�����_�����߂�
		//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
		c = mPlayer->mPosition + CVector(0.0f, 0.0f, -40.0f)* mPlayer->mMatrixScale
			* CMatrix().RotateY(mPlayer->mRotation.mY);
			//* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	}

	//CMatrix().RotateZ(mRotation.mZ) *CMatrix().RotateX(mRotation.mX) *CMatrix().RotateY(mRotation.mY);

	//////�����_�����߂�
	////c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)*mPlayer->mMatrixRotate;
	//c = mPlayer->mPosition + CVector(0.0f, 0.0f, 40.0f)
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* CMatrix().RotateZ(mPlayer->mRotation.mZ);
	
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);// *mPlayer->mMatrixRotate;
	


	//e = CVector(0.0f+mXXX, 5000.0f, mZZZ+0.0f);
	//	/*CVector(0.0f, 3000.0f, 0.0f) * CMatrix().RotateY(mCamY) * mPlayer->mMatrixScale
	//	* CMatrix().RotateY(mPlayer->mRotation.mY)
	//	* mPlayer->mMatrixTranslate
	//	+ CVector(0.0f, 0.0f, 0.0f);*/
	////�����_�����߂�
	//c = CVector(0.0f+mXXX, 0.0f, mZZZ+0.0001f);
	//	//mPlayer->mPosition + CVector(0.0f, 0.0f, 0.01f)*mPlayer->mMatrixRotate;
	////����������߂�
	//u = CVector(0.0f, 1.0f, 0.0f) * mPlayer->mMatrixRotate;

	//�J�����̐ݒ�
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);
	Camera.mEye = e;

	////�w�i�̕`��
	//mSky.Render();
	////�^�X�N�}�l�[�W���̍X�V
	//TaskManager.Update();
	////�^�X�N�}�l�[�W���̕`��
	//TaskManager.Render();

	//mPlayer->mpModel = &mRover;
	mPlayer->mpModel = &mRover;
	mPlayer->mScale = CVector(2.5f, 2.5f, 2.5f);

	//�^�X�N�}�l�[�W���̍X�V�E�`��
	CTaskManager::Get()->Update();
	CTaskManager::Get()->Render();

	////��̕`��
	//mRock.Render(CMatrix().Scale(5.0f, 5.0f, 5.0f));
	////�Ԃ̕`��
	//mRover.Render(CMatrix().Translate(-20.0f, 0.0f, 10.0f));

	////���J
	////�R���W�����}�l�[�W���̏Փˏ���
	//CollisionManager.Collision();
	//TaskManager.TaskCollision();
	CTaskManager::Get()->TaskCollision();

	//TaskManager.Delete();
	CTaskManager::Get()->Delete();

	//�f�o�b�O�p
#ifdef _DEBUG
	if (CKey::Once('9')){
		if (mPutCol){
			mPutCol = false;
		}
		else{
			mPutCol = true;
		}
	}
	if (mPutCol){
		//�Փ˔���̕`��
		CollisionManager.Render();	
	}
#endif

	if (CKey::Once('0')){
		//mCamPoV = 1;
		if (mCamPoV == 1){
			mCamPoV = 2;
		}
		else if (mCamPoV == 2){
			mCamPoV = 3;
		}
		else if (mCamPoV == 3){
			mCamPoV = 1;
		}
	}


	if (isStartRace){
		//59:59:59���ő厞��
		if (mTime < 595959){
			if (mTime % 10000 == 5959){
				mTime += 1 + 40 + 4000;
			}
			else if (mTime % 100 == 59){
				mTime += 1 + 40;
			}
			else{
				mTime += 1;
			}
		}
	}
	
	RenderMiniMap();
	//2D�`��J�n
	Start2D(0, 800, 0, 600);

	CText::DrawString("COURCE 1", 20, 20, 10, 12);

	//���Ԃ̕\��
	char mti[70];// :���܂߂��ő啶�����̐ݒ�
//	sprintf(mti, "%06d", mTime);
	sprintf(mti, "TIME:%02d:%02d:%02d", mTime / 10000 % 100, mTime / 100 % 100, mTime % 100);
	CText::DrawString(mti, 20, 530, 10, 12);
	//�x�X�g�^�C���̕\��
	char mbestti[70];// :���܂߂��ő啶�����̐ݒ�
	//	sprintf(mti, "%06d", mTime);
	sprintf(mbestti, "BEST:%02d:%02d:%02d", mBestTime / 10000 % 100, mBestTime / 100 % 100, mBestTime % 100);
	CText::DrawString(mbestti, 20, 580, 10, 12);
	
	//�J�E���g�_�E���J�n�AGO!�ő���̎�t�J�n
	if (mFrame < 60){
		mFrame++;
	}
	else{
		if (mCountDown >= 0){
			mCountDown--;
			if (mCountDown == 0){
				isStartRace = true;
				SoundStart.Play();
			}
			else if (mCountDown > 0){
				SoundCountDown.Play();
			}
		}
		mFrame = 0;
	}
	//�J�E���g�_�E���\��
	char mcountd[7];
	//�c��3�b�܂ł̎������\��
	sprintf(mcountd, "%d", mCountDown);
	if (mCountDown > 0 && mCountDown <= 3){
		CText::DrawString(mcountd, 400, 300, 25, 30);
	}
	else if (mCountDown == 0){
		CText::DrawString("GO!", 400-40, 300, 25, 30);
		CPlayer::mpPlayer->CanMove = true;
		mEnemy1->CanMove = true;
		mEnemy2->CanMove = true;
		for (int i = 0; i < 5; i++){
			mEnemys[i]->CanMove = true;
		}
	}
	/*SoundCountDown.Play();
	SoundStart.Play();*/


	if (isStartRace == false){
		if (mCountDown > 0){
			//�J�E���g�_�E����(�S�[���ȑO�ɁA�X�^�[�g���炵�ĂȂ�)
		}
		else{
			CText::DrawString("FINISH!", 400 - 25 * 6, 300, 25, 30);
		}
	}

	char lap[19];
	sprintf(lap, "LAP%d/%d", mLap, mMaxLap);
	CText::DrawString(lap, 20, 500, 10, 12, 2);

	//�S�[����A�p�����Ď��s���鏈��
	if (mLap == mMaxLap && isStartRace == false && isGoal){
		//60f�ňꏄ
		if (mTextBlinkTime < 60){
			mTextBlinkTime++;
		}
		else{
			mTextBlinkTime = 0;
		}
		//Enter�L�[���͂Ń^�C�g����ʂɖ߂�邱�Ƃ�`����e�L�X�g
		if (mTextBlinkTime < 30){
			CText::DrawString("Press Enter to Title", 222, 77, 10, 12, 2);
		}
		//�V�L�^���������o������
		if (isNewRecord){
			//CText::DrawString("FINISH!", 400 - 20 * 6, 300, 20, 24);
			if (mTextBlinkTime < 15 || mTextBlinkTime >= 30 && mTextBlinkTime < 45){
				CText::DrawString("NEW RECORD!", 55, 551, 8, 9, 2);
			}
		}
	}

	//�~�j�}�b�v�E���ݒn�̕\��
//	CText::DrawString("+", -(CPlayer::mpPlayer->mPosition.mX / 50)+600, CPlayer::mpPlayer->mPosition.mZ / 50 + 100, 10, 10);

	char carspeed[33];
	sprintf(carspeed, "SPEED:%4.1f", CPlayer::mpPlayer->mCarSpeed);
	CText::DrawString(carspeed, 20+560, 20, 10, 12);

	
	
	//2D�`��I��
	End2D();

	if (CKey::Push('Y')){//�ł�
		mCamY += 1.0f;
	}
	if (CKey::Push('U')){//��
		mCamY += -1.0f;
	}


	if ((CPlayer::mpPlayer->mPosition.mX > 155.0f && CPlayer::mpPlayer->mPosition.mX < 975.0f)
		&& (CPlayer::mpPlayer->mPosition.mZ > -3.1f - 5.0f && CPlayer::mpPlayer->mPosition.mZ < -3.1f + 5.0f)
		&& (CPlayer::mpPlayer->mChecks == 3)
		&& (isStartRace)){
		//new CObj(&mCube, CVector(0.0f, 0.0f, 5700.0f), CVector(-90.0f, 0.0f, 0.0f), CVector(100.0f, 13.0f, 211.0f), 1);//�S�[��
		//new CObj(&mTileBlack, CVector(170.0f + 20.0f * -1 + 5.0f, -13.1f + 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(5.0f, 64.0f, 5.0f), 99);//��
		//new CObj(&mTileBlack, CVector(170.0f + 20.0f * 40 + 5.0f, -13.1f + 10.0f, -10.0f), CVector(0.0f, 0.0f, 0.0f), CVector(5.0f, 64.0f, 5.0f), 99);//��

		if (mLap == mMaxLap){
			//�x�X�g�^�C���X�V��
			if (mTime < mBestTime){
				mBestTime = mTime;
				isNewRecord = true;
				//�R�[�X�ɂ���ĐV�����L�^����
				if (CSceneTitle::mMode == 1){
					mRecord_A = mBestTime;
				}
				else if (CSceneTitle::mMode == 2){				
					mRecord_B = mBestTime;
				}
				else if (CSceneTitle::mMode == 3){					
					mRecord_C = mBestTime;
				}
				else if (CSceneTitle::mMode == 4){					
					mRecord_D = mBestTime;
				}
			}
			isStartRace = false;
			isGoal = true;
			BGM.Stop();
			SoundGoal.Play();
			//CPlayer::mpPlayer->CanMove = false;//�����X�g�b�v
			CPlayer::mpPlayer->mChecks = 0;
		}
		else{
			mLap++;
			CPlayer::mpPlayer->mChecks = 0;
		}
	}
	

	//	new CObj(&mWarpPoint, CVector(-143.0f, 0.0f, 94.0f), CVector(0.0f, 0.0f, 0.0f), CVector(6.0f, 20.0f, 6.0f), 20);
	//����̒n�_(��(�̂���)�͈͓̔�)�ɍs����Stage2�ֈڍs
	/*if ((CPlayer::mpPlayer->mPosition.mX > -149.0f && CPlayer::mpPlayer->mPosition.mX < -137.0f)
		&& (CPlayer::mpPlayer->mPosition.mZ > 88.0f && CPlayer::mpPlayer->mPosition.mZ < 100.0f)
		&& (CPlayer::mpPlayer->mHaveBoat)){
		mScene = ESTAGE2;
	}*/
	////�f�o�b�O�R�}���h
	//if (CKey::Once(VK_RETURN)){
	//	mScene = ESTAGE2;
	//}

	if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
		printf("�x�X�g�^�C��:%d\n", mBestTime);
	}
	if (CKey::Once('2')){//Player�̍��W���o�͂���
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Once('3')){//�����I�ɓI�̎c����0�ɂ���(�{���̓I�͏����Ȃ�)
		CItem::mTargetAmount = 0;
	}
	if (CKey::Once('4')){//�]��̌v�Z���A���鐔(�E�̐���)�͐��̒l�Ōv�Z�����H
		printf("%d\n", -13 % 5);//��%��=��
		printf("%d\n", -12 % -5);//��%��=��
		printf("%d\n", 14 % -5);//��%��=��
	}
	if (CKey::Push('5')){
		printf("%f:%f:%f\n", CPlayer::mpPlayer->mRotation.mX, CPlayer::mpPlayer->mRotation.mY, CPlayer::mpPlayer->mRotation.mZ);
	}
	if (CKey::Once('6')){
		printf("%d\n", CPlayer::mpPlayer->mChecks);
	}
	if (CKey::Once('7')){
		if (CPlayer::mpPlayer->mFlyingMode){
			CPlayer::mpPlayer->mFlyingMode = false;
		}
		else{
			CPlayer::mpPlayer->mFlyingMode = true;
		}
	}
	
	if (CKey::Once('M')){
		BGM.Stop();
	}

	if (CKey::Once(VK_RETURN)){
		//���̃V�[���̓Q�[��
		mScene = ETITLE;
	}


	return;
}

/* �}�b�v�ォ��̎��_ */
void CSceneRace::RenderMiniMap() {
	glPushMatrix();
	glViewport(600 + 20-30, 450 - 440, 200, 150); //��ʂ̕`��G���A�̎w��
	glLoadIdentity();
	gluLookAt(0, 4800, 0, 0, 0, 0, 0, 0, 1);
	glDisable(GL_DEPTH_TEST);
//	BackGround.Render(CMatrix());
	//�^�X�N�}�l�[�W���̕`��
//	TaskManager.Render();

//	mPlayer->mpModel = &mCarsol;
//	mPlayer->mScale = CVector(10.0f, 1.0f, 10.0f);
	CTaskManager::Get()->Render();
	//�~�j�}�b�v�ɃS�[���A�C�R����`��
	CMatrix matminig;
	matminig = CMatrix().Scale(20.0f, 1.0f, 20.0f)
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(0)
		* CMatrix().RotateZ(0)
		* CMatrix().Translate(550.0f, 0.0f, -10.0f);
	mMiniGoal.Render(matminig);
	//�~�j�}�b�v�ɃJ�[�\����`��
	CMatrix mat;
	mat = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
		//* mPlayer->mMatrixRotate
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(mPlayer->mRotation.mY)
		* CMatrix().RotateZ(0)
		* mPlayer->mMatrixTranslate;
	mCarsol.Render(mat);
	//�G�����̃J�[�\��
	CMatrix matenemy;
	matenemy = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(mEnemy1->mRotation.mY)
		* CMatrix().RotateZ(0)
		* mEnemy1->mMatrixTranslate;
	mCarsol.Render(matenemy);

	CMatrix matenemy2;
	matenemy2 = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
		* CMatrix().RotateX(0)
		* CMatrix().RotateY(mEnemy2->mRotation.mY)
		* CMatrix().RotateZ(0)
		* mEnemy2->mMatrixTranslate;
	mCarsol.Render(matenemy);

	CMatrix matenemys[5];
	for (int i = 0; i < 5; i++){		
		matenemys[i] = CMatrix().Scale(35.0f, 1.0f, 35.0f) //* mPlayer->mMatrixScale
			* CMatrix().RotateX(0)
			* CMatrix().RotateY(mEnemys[i]->mRotation.mY)
			* CMatrix().RotateZ(0)
			* mEnemys[i]->mMatrixTranslate;
		mCarsol.Render(matenemys[i]);
	}

	/*�f�o�b�O�p*/
	CMatrix point;
	for (int i = 1; i <= 12; i++){//�|�C���g�̐������������s
		point = CMatrix().Scale(111.0f, 1.0f, 111.0f)
			* CMatrix().RotateY(45);
			//* CEnemy::mPoint->mMatrixTranslate;
		//1��菬�������͑����
		if (i < 1){
			break;
		}
		if (i == 1){
			point = point * CEnemy::mPoint->mMatrixTranslate;
		}
		else if(i == 2){
			point = point * CEnemy::mPoint2->mMatrixTranslate;
		}
		else if (i == 3){
			point = point * CEnemy::mPoint3->mMatrixTranslate;
		}
		else if (i == 4){
			point = point * CEnemy::mPoint4->mMatrixTranslate;
		}
		else if (i == 5){
			point = point * CEnemy::mPoint5->mMatrixTranslate;
		}
		else if (i == 6){
			point = point * CEnemy::mPoint6->mMatrixTranslate;
		}		
		//�n�[�h���[�h�ł͂���ɖڕW�n�_���ׂ����ݒ肳���
		else if (CSceneTitle::mDifficulty == 3){
			if (i == 7){
				point = point * CEnemy::mPoint7->mMatrixTranslate;
			}
			else if (i == 8){
				point = point * CEnemy::mPoint8->mMatrixTranslate;
			}
			else if (i == 9){
				point = point * CEnemy::mPoint9->mMatrixTranslate;
			}
			else if (i == 10){
				point = point * CEnemy::mPoint10->mMatrixTranslate;
			}
			else if (i == 11){
				point = point * CEnemy::mPoint11->mMatrixTranslate;
			}
			else if (i == 12){
				point = point * CEnemy::mPoint12->mMatrixTranslate;
			}
			//mTileWhite.Render(point);
		}
		else{
			break;
		}
		mTileWhite.Render(point);
	}

	//point = CMatrix().Scale(95.0f, 1.0f, 95.0f) //* mPlayer->mMatrixScale
	//	* CMatrix().RotateX(0)
	//	* CMatrix().RotateY(CEnemy::mPoint->mRotation.mY)
	//	* CMatrix().RotateZ(0)
	//	* CEnemy::mPoint->mMatrixTranslate;
	//mTileWhite.Render(point);	
	//point = CMatrix().Scale(95.0f, 1.0f, 95.0f) //* mPlayer->mMatrixScale
	//	* CMatrix().RotateX(0)
	//	* CMatrix().RotateY(CEnemy::mPoint2->mRotation.mY)
	//	* CMatrix().RotateZ(0)
	//	* CEnemy::mPoint2->mMatrixTranslate;
	//mTileWhite.Render(point);
	//point = CMatrix().Scale(95.0f, 1.0f, 95.0f) //* mPlayer->mMatrixScale
	//	* CMatrix().RotateX(0)
	//	* CMatrix().RotateY(CEnemy::mPoint3->mRotation.mY)
	//	* CMatrix().RotateZ(0)
	//	* CEnemy::mPoint3->mMatrixTranslate;
	//mTileWhite.Render(point);
	//point = CMatrix().Scale(95.0f, 1.0f, 95.0f) //* mPlayer->mMatrixScale
	//	* CMatrix().RotateX(0)
	//	* CMatrix().RotateY(CEnemy::mPoint4->mRotation.mY)
	//	* CMatrix().RotateZ(0)
	//	* CEnemy::mPoint4->mMatrixTranslate;
	//mTileWhite.Render(point);
	//point = CMatrix().Scale(95.0f, 1.0f, 95.0f) //* mPlayer->mMatrixScale
	//	* CMatrix().RotateX(0)
	//	* CMatrix().RotateY(CEnemy::mPoint5->mRotation.mY)
	//	* CMatrix().RotateZ(0)
	//	* CEnemy::mPoint5->mMatrixTranslate;
	//mTileWhite.Render(point);
	//point = CMatrix().Scale(95.0f, 1.0f, 95.0f) //* mPlayer->mMatrixScale
	//	* CMatrix().RotateX(0)
	//	* CMatrix().RotateY(CEnemy::mPoint6->mRotation.mY)
	//	* CMatrix().RotateZ(0)
	//	* CEnemy::mPoint6->mMatrixTranslate;
	//mTileWhite.Render(point);

	//mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;//�Q�l
	


	glPopMatrix();
	glViewport(0, 0, 800, 600); //��ʂ̕`��G���A�̎w��
	glEnable(GL_DEPTH_TEST);
	
	//Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);

	
}


//���̃V�[���̎擾
CScene::EScene CSceneRace::GetNextScene(){
	return mScene;
}

