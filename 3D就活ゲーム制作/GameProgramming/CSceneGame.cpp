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

	mPlayer = new CPlayer();
	mPlayer->mpModel = &mRover;

	mCamY = 0.0f;
	mPutCol = false;

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

	//�^�X�N�}�l�[�W���̍X�V�E�`��
	CTaskManager::Get()->Update();
	CTaskManager::Get()->Render();

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

