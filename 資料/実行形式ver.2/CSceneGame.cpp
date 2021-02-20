#include "CSceneGame.h"
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

extern CSound BGM;

CSceneGame::~CSceneGame() {
	CTaskManager::Get()->Disabled();
	CTaskManager::Get()->Delete();
}


void CSceneGame::Init() {
	//�V�[���̐ݒ�
	mScene = ESTAGE1;
	
	//�I�̎c���̏�����
	CItem::mTargetAmount = 0;

	//�e�L�X�g�t�H���g�̓ǂݍ��݂Ɛݒ�
	CText::mFont.Load("FontG.tga");
	CText::mFont.SetRowCol(1, 4096 / 64);
	//�w�i�̓ǂݍ���
	mSky.Load("sky.obj", "sky.mtl");
	//��̓ǂݍ���
	mRock.Load("Rock1.obj", "Rock1.mtl");
	//�Ԃ̓ǂݍ���
	mRover.Load("untitled.obj", "cube.mtl");
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
	//�e�̓ǂݍ���
	mItemGun.Load("�e.obj", "material\\gun.mtl");

	mCube2.Load("cube.obj", "cube2.mtl");
	mCube3.Load("cube.obj", "cube3.mtl");

	mWater.Load("cube.obj", "water_sumple.mtl");

	//�{�[�g�̓ǂݍ���
	mBoat.Load("�ӂ�.obj", "cube.mtl");
	//���[�v�n�_�̌�
	mWarpPoint.Load("cube.obj", "material\\warp.mtl");

	//�o�l�̓ǂݍ���
	mSpringS.Load("�΂�.obj", "cube.mtl");
	mSpringL.Load("�΂ːL��.obj", "cube.mtl");

	CBullet::mBullet.Load("�e�e.obj", "�e�e.mtl");

	//�X�e�[�W1BGM�̓ǂݍ���
	BGM.Load("BGM\\game_maoudamashii_7_event46.wav");

	//�����x�̍����������ɕ`�悷��
	
	/*for (int i = 0; i < 12; i++){
		new CObj(&mCube3, CVector(43.0f, 15.0f + i * 7.0f, 118.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 2.0f, 10.0f), 9);
	}*/
	//new CObj(&mCube3, CVector(-73.0f, 5.0f, 100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(30.0f, 45.0f, 1.0f), 13);

	new CObj(&mCube3, CVector(10.0f, 23.0f, 100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(40.0f, 3.0f, 10.5f), 9);//�������̃K���X

	new CObj(&mWater, CVector(-60.0f, -1.8f, 60.0f), CVector(0.0f, 0.0f, 0.0f), CVector(140.0f, 1.0f, 65.0f), 9);//����

	for (int i = 0; i < 18; i++){ 
		new CObj(&mWarpPoint, CVector(-143.0f, -0.5f, 94.0f), CVector(0.0f, 0.0f + 10.0f * i, 0.0f), CVector(6.0f, 20.0f, 6.0f), 18);//���[�v�n�_�̌�
	}


	//new CObj(&mCube3, CVector(37.5f, 0.0f, 30.0f), CVector(), CVector(23.0f, 10.0f, 13.0f), 9);//�{�[�g�̃P�[�X

	mPlayer = new CPlayer();
	mPlayer->mpModel = &mRover;

	//��̐����@���f��mRock�@�ʒu|-20.0 0.0 20.0|
	//��]|0.0 0.0 0.0|�@�g��|5.0 5.0 5.0|
//	new CRock(&mRock, CVector(-20.0f, 0.0f, 20.0f), CVector(), CVector(5.0f, 5.0f, 5.0f));
	//�����̂̐���
//	new CObj(&mCube, CVector(0.0f, 0.0f, 60.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 10.0f, 1.0f), 1);
	//�n�ʂ̐���
	new CObj(&mPlane, CVector(0.0f, 0.0f, 0.0f), CVector(), CVector(200.0f, 1.0f, 200.0f), 1);



	for (int i = 0; i < 3; i++){
		for (int j = 0; j < 3; j++){
			for (int k = 0; k < 3; k++){
				if ((i == 1 && j == 1) || (j == 1 && k == 1) || (k == 1 && i == 1) || (k == 2 && j != 1) || (j == 1 && k == 0)) {
					

				}
				else if (k == 0){
					new CObj(&mCube, CVector(-20.0f + 20.0f * i, -1.0f + 20.0f * k, 80.0f + 20.0f * j), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 15.0f, 10.0f), 1);
				}
				else{
					new CObj(&mCube, CVector(-20.0f + 20.0f * i, 9.0f + 20.0f * k, 80.0f + 20.0f * j), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 10.0f, 10.0f), 1);
				}

			}
		}
	}
	new CObj(&mOnBlock, CVector(0.0f, 60.0f, 100.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 10.0f, 10.0f), 10);
	new CItem(&mItemGun, CVector(0.0f, 80.0f, 100.0f), CVector(), CVector(2.0f, 2.0f, 2.0f), 4);//�e
	for (int i = 0; i < 5; i++){
		new CItem(&mTarget, CVector(-10.0f + i * 7.5f, 93.0f, 150.0f), CVector(), CVector(2.1f, 2.1f, 2.1), 1);//�I(��)
	}
	//�I�̌�ɐ�������
	new CObj(&mOnBlock, CVector(5.0f, 89.0f, 155.0f), CVector(), CVector(22.0f, 5.0f, 1.0f), 10);



//	new CItem(&mItem, CVector(43.0f, 111.0f, 118.0f), CVector(), CVector(2.0f, 2.0f, 2.0f), 3);//�X�N�����[
	
	new CObj(&mCube, CVector(0.0f,-5.55f, 13.0f), CVector(-5.4f, 0.0f, 0.0f), CVector(10.0f, 4.0f, 28.0f), 1);//���H
	for (int l = 0; l < 5; l++){
		new CObj(&mStairs, CVector(0.0f, -1.0f, 43.0f + l * 6.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 3.0f + 3.0f*l , 3.0f), 1);//�K�i
	}
	new CItem(&mSpringL, CVector(0.0f, 30.0f, 80.0f), CVector(), CVector(5.0f, 5.0f, 5.0f), 2);//�o�l

	new CItem(&mBoat, CVector(35.0f, 7.0f, 30.0f), CVector(), CVector(2.5f, 2.5f, 2.5f), 7);//�{�[�g
	new CObj(&mOnBlock, CVector(37.5f, 0.0f, 30.0f), CVector(), CVector(23.0f, 12.0f, 13.0f), 10);//�{�[�g���ޔ�


	//new CObj(&mCube, CVector(30.0f, 0.0f, 30.0f), CVector(0.0f, 0.0f, 0.0f), CVector(10.0f, 2.0f, 10.0f), 2);

	mCamY = 0.0f;
	mPutCol = false;

	//TaskManager.ChangePriority(&mPlayer, 15);
	CTaskManager::Get()->ChangePriority(mPlayer, 15);

	BGM.Repeat();
}


void CSceneGame::Update() {
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(-2.0f, 17.0f, -40.0f) * CMatrix().RotateY(mCamY+90) * mPlayer->mMatrix;
	//�����_�����߂�
	c = mPlayer->mPosition;
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f) * mPlayer->mMatrixRotate;

	//�J�����̐ݒ�
	Camera3D(e.mX, e.mY, e.mZ, c.mX, c.mY, c.mZ, u.mX, u.mY, u.mZ);

	//�w�i�̕`��
	mSky.Render();

	////�^�X�N�}�l�[�W���̍X�V
	//TaskManager.Update();
	////�^�X�N�}�l�[�W���̕`��
	//TaskManager.Render();

	//�^�X�N�}�l�[�W���̍X�V�E�`��
	CTaskManager::Get()->Update();
	CTaskManager::Get()->Render();

	////��̕`��
	//mRock.Render(CMatrix().Scale(5.0f, 5.0f, 5.0f));
	////�Ԃ̕`��
	//mRover.Render(CMatrix().Translate(-20.0f, 0.0f, 10.0f));

//���J
	//�R���W�����}�l�[�W���̏Փˏ���
	CollisionManager.Collision();
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


	//2D�`��J�n
	Start2D(0, 800, 0, 600);

	CText::DrawString("STAGE 1", 20, 20, 10, 12);

	//2D�`��I��
	End2D();


	//if (CKey::Push('Q')){//�ł�
	//	mCamY += 1.0f;
	//}
	//if (CKey::Push('E')){//��
	//	mCamY += -1.0f;
	//}


//	new CObj(&mWarpPoint, CVector(-143.0f, 0.0f, 94.0f), CVector(0.0f, 0.0f, 0.0f), CVector(6.0f, 20.0f, 6.0f), 20);
	//����̒n�_(��(�̂���)�͈͓̔�)�ɍs����Stage2�ֈڍs
	if ((CPlayer::mpPlayer->mPosition.mX > -149.0f && CPlayer::mpPlayer->mPosition.mX < -137.0f)
		&& (CPlayer::mpPlayer->mPosition.mZ > 88.0f && CPlayer::mpPlayer->mPosition.mZ < 100.0f)
		&& (CPlayer::mpPlayer->mHaveBoat)){
		mScene = ESTAGE2;
	}
	//�f�o�b�O�R�}���h
	if (CKey::Once(VK_RETURN)){
		mScene = ESTAGE2;
	}

	if (CKey::Once('1')){
		printf("%d\n", CItem::mTargetAmount);
	}
	if (CKey::Once('2')){//Player�̍��W���o�͂���
		printf("X:%f Y:%f Z:%f\n", CPlayer::mpPlayer->mPosition.mX, CPlayer::mpPlayer->mPosition.mY, CPlayer::mpPlayer->mPosition.mZ);
	}
	if (CKey::Once('3')){//�����I�ɓI�̎c����0�ɂ���(�{���̓I�͏����Ȃ�)
		CItem::mTargetAmount = 0;
	}


	return;
}


//���̃V�[���̎擾
CScene::EScene CSceneGame::GetNextScene(){
	return mScene;
}

