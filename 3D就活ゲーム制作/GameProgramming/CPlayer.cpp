#include "CPlayer.h"
#include "CKey.h"

#include "CItem.h"
#include "CBullet.h"

extern CSound SoundJump;
extern CSound SoundShot;
extern CSound SoundItemGet;
extern CSound SoundBoost;
extern CSound SoundEngine;
extern CSound SoundCollision;
extern CSound SoundCollisionSmall;

int CPlayer::RenderType;

CPlayer *CPlayer::mpPlayer = 0;

#define G (9.8f / 120.0f)//�d�͉����x//60.0f
#define JUMPV0 (4.0f*4.0f)//�W�����v����//4.0f

#define MAXSPEED 4.5f+3.0f //�Ԃ̍ō����x
#define MAXSPEED_BACK 1.0f*2 //�Ԃ̌�ނ���ő呬�x
#define CAR_POWER 0.05f*2 //1�t���[���ӂ�̎Ԃ̉������Ă�����
#define CAR_BREAK_POWER 0.025f*2 //�O�i���̃u���[�L�̋���

#define DECELERATE 0.05f*2 //�Ԃ̌��������
#define FIX_ANGLE_VALUE 0.5f*2 //�p�x��0�x�Ɍ����Ē���������(���X�EZ�p)

//#define MAXSPEED 7.0f //�Ԃ̍ō����x
//#define MAXSPEED_BACK 2.0f //�Ԃ̌�ނ���ő呬�x
//#define CAR_POWER 0.1f //1�t���[���ӂ�̎Ԃ̉������Ă�����
//#define CAR_BREAK_POWER 0.05f //�O�i���̃u���[�L�̋���
//
//#define DECELERATE 0.1f //�Ԃ̌��������
//#define FIX_ANGLE_VALUE 0.5f //�p�x��0�x�Ɍ����Ē���������(���X�EZ�p)

CPlayer::CPlayer()
//:mColHead(this, CVector(0.0f, 19.2f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColBody(this, CVector(0.0f, 14.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 3.0f)
//, mColLeg(this, CVector(0.0f, 9.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColKnee(this, CVector(0.0f, 5.5f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColFoot(this, CVector(0.0f, 2.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//: mColHead(this, CVector(0.0f, 6.2f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColBody(this, CVector(0.0f, 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 3.0f)
//, mColLeg(this, CVector(0.0f, -4.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColKnee(this, CVector(0.0f, -7.5f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)
//, mColFoot(this, CVector(0.0f, -11.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 2.0f)

//�ԑ̂�Y���W��0.0f�ɂ������񂾂��ǁE�E�E
//0.0f�ɂ�����ԑ̂������Ă�悤�Ɍ����Ă��܂�
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
, mColTire(this, CVector(0.0f, -16.0f+15.0f+1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f)
{
	mpPlayer = this;

	mScale = CVector(2.0f, 2.0f, 2.0f);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//�o�l�擾���2.3f�̗\��
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//�Ԃ̑��x�̏�����
	mTurnSpeed = 0.0f;

	//mRotation.mY = -90;

	mCanJump = false;
	mCanSwim = false;
	mCanFire = false;
	mIsGetKey = false;
	mHaveBoat = false;
	mFlyingMode = false;

	CanMove = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EPLAYER;
	mJumpPrio = 0;

	//�X�^�[�g�n�_�̍��W��ݒ�;
	mStartPoint[0] = 300.0f;  mStartPoint[1] = 63.0f;  mStartPoint[2] = -100.0f;
	
	mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	RenderType = 0;//�`�揈�� 0:�Q�[�����  1:�~�j�}�b�v

	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;

	SoundJump.Load("SE\\jump12.wav");
	SoundShot.Load("SE\\shot1.wav");
	SoundItemGet.Load("SE\\se_maoudamashii_system46.wav");	
	SoundBoost.Load("SE\\Shortbridge31-3.wav");
	SoundEngine.Load("SE\\SNES-Racing01-02.wav");
	SoundCollision.Load("SE\\bomb1.wav");
	SoundCollisionSmall.Load("SE\\SNES-Racing01-10(Collision).wav");
	


	isSoundEngine = false;
	//SoundEngine.Repeat();
}

void CPlayer::Update(){
	/*if (CKey::Push('A')){
		mRotation.mY++;
	}
	if (CKey::Push('D')){
		mRotation.mY--;
	}
	if (CKey::Push('W')){
		mPosition = CVector(0.0f, mVelocityJump, 0.5f) * mMatrix;
	}
	else if (CKey::Push('S')){
		mPosition = CVector(0.0f, mVelocityJump, -0.5f) * mMatrix;
	}
	else{
		mPosition = CVector(0.0f, mVelocityJump, 0.0f) * mMatrix;
	}*/

	if (CKey::Push('Q')){//�ł�
		mRotation.mY++;
	}
	if (CKey::Push('E')){//��
		mRotation.mY--;
	}

	if (CKey::Push('B')){//���}�u���[�L
		mCarSpeed = 0.0f;
	}

	////A�L�[�AD�L�[�������ɓ��͂���Ă��邩
	//if (CKey::Push('A') && CKey::Push('D')){
	//	mADMoveX = 0.0f;
	//}
	//else if (CKey::Push('A')){//A�L�[����
	//	mADMoveX = mMoveSpeed;
	//}
	//else if (CKey::Push('D')){//D�L�[����
	//	mADMoveX = -mMoveSpeed;
	//}
	//else{//�ǂ�������͂Ȃ�
	//	mADMoveX = 0.0f;
	//}
	////W�L�[�AS�L�[�������ɓ��͂���Ă��邩
	//if (CKey::Push('W') && CKey::Push('S')){
	//	mWSMoveZ = 0.0f;
	//}
	//else if (CKey::Push('W')){//W�L�[����
	//	mWSMoveZ = mMoveSpeed;
	//}
	//else if (CKey::Push('S')){//S�L�[����
	//	mWSMoveZ = -mMoveSpeed;
	//}
	//else{//�ǂ�������͂Ȃ�
	//	mWSMoveZ = 0.0f;
	//}

	

	if (FireCount > 0){
		FireCount--;
	}

	//�e�𔭎ˉ\�Ȏ�
	if (mCanFire){
		//�X�y�[�X�L�[���͂Œe����
		if (CKey::Push(VK_SPACE)){
			if (FireCount <= 0){
				CBullet *bullet = new CBullet();
				bullet->Set(2.0f, 0.1f);
				//bullet->mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
				bullet->mPosition = CVector(0.0f, 1.0f, 10.0f) * CMatrix().RotateY(90) * mMatrix;
				bullet->mRotation = mRotation;
				bullet->mScale = CVector(3.0f, 3.0f, 3.0f);
				//	TaskManager.Add(bullet);
				FireCount = 12;
				//Bullet.Set(0.1f, 1.5f);
				//Bullet.mPosition = CVector(0.0f, 0.0f, 10.0f) * mMatrix;
				//Bullet.mRotation = mRotation;

				//���[�U�[���ˉ��̍Đ�
				SoundShot.Play();
			}
		}
	}

	//��s���[�h�L����(�f�o�b�O�p)
	if (mFlyingMode){
		if (CKey::Push('W')){
			mVelocityJump = mMoveSpeed*3;
		}
		else if (CKey::Push('S')){
			mVelocityJump = -mMoveSpeed*3;
		}
		else{
			mVelocityJump = 0.0f;
		}
	}

	//�u�[�X�g�c�莞��
	if (mBoostTime > 0){
		mBoostTime--;
	}
	else{
		isBoost = false;		
	}
	//�u�[�X�g�L����
	if (isBoost){
		mBoostMaxSpeed = 10.0f;

		if (mCarSpeed < MAXSPEED + mBoostMaxSpeed){
			//�u�[�X�g���̃A�N�Z�����ʂ͎���3�{
			mCarSpeed += CAR_POWER;
			mCarSpeed += CAR_POWER;
		}
	}
	//�u�[�X�g���؂�Ă��鎞
	else{
		//�ō����x���ʏ�܂Ō���
		if (mBoostMaxSpeed > 0.0f){
			if (mBoostMaxSpeed > 0.2f){
				mBoostMaxSpeed -= 0.2f;
			}
			else{
				mBoostMaxSpeed = 0.0f;
			}
		}
		//�u�[�X�g���I�����Č��݂̑��x���ō����x�𒴉߂��Ă����ꍇ�A�ō����x�ɍ��킹�Č�������Ă���
		if (mCarSpeed > MAXSPEED + mBoostMaxSpeed){
			mCarSpeed = MAXSPEED + mBoostMaxSpeed;
		}
	}


	if (CKey::Push(VK_UP) && CanMove){
		if (mCarSpeed < MAXSPEED + mBoostMaxSpeed){
			mCarSpeed += CAR_POWER;
		}		
	}
	else if (CKey::Push(VK_DOWN) && CanMove){
		if (mCarSpeed > -MAXSPEED_BACK){
			mCarSpeed -= CAR_POWER;
			//�O�i���Ȃ�u���[�L��������A���߂Ɏ~�܂�
			if (mCarSpeed >= CAR_BREAK_POWER){
				mCarSpeed -= CAR_BREAK_POWER;
			}
		}
	}
	else{
		//�O�i��
		if (mCarSpeed > 0.0f){
			//�����l�Ń}�C�i�X�ɂȂ�Ȃ��悤�ɒ���
			if (mCarSpeed < DECELERATE){
				mCarSpeed = 0.0f;
			}
			else{
				mCarSpeed -= DECELERATE;
			}
		}
		//��ޒ�
		else if (mCarSpeed < 0.0f){
			if (mCarSpeed > -DECELERATE){
				mCarSpeed = 0.0f;
			}
			else{
				mCarSpeed += DECELERATE;
			}
		}		
	}

	//�v���C���[��X�p�x��0�ɖ߂���Ă���
	if (mRotation.mX > 0.0f){
		if (mRotation.mX > FIX_ANGLE_VALUE){
			mRotation.mX -= FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mX = 0.0f;
		}
	}
	else if (mRotation.mX < 0.0f){
		if (mRotation.mX < -FIX_ANGLE_VALUE){
			mRotation.mX += FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mX = 0.0f;
		}
	}
	//�v���C���[��Z�p�x��0�ɖ߂���Ă���
	if (mRotation.mZ > 0.0f){
		if (mRotation.mZ > FIX_ANGLE_VALUE){
			mRotation.mZ -= FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mZ = 0.0f;
		}
	}
	else if (mRotation.mZ < 0.0f){
		if (mRotation.mZ < -FIX_ANGLE_VALUE){
			mRotation.mZ += FIX_ANGLE_VALUE;
		}
		else{
			mRotation.mZ = 0.0f;
		}
	}

	if (CKey::Push(VK_LEFT) && CanMove){ //�n���h�������ɁI
		//mRotation.mY++;
		if (mTurnSpeed>=0.0f&&mTurnSpeed<0.5f){
			mTurnSpeed = 0.5f;
		}
		if (mTurnSpeed < 0.0f){
			mTurnSpeed += 0.11f;
		}
		mTurnSpeed += 0.04f;
	}
	else if (CKey::Push(VK_RIGHT) && CanMove){//�n���h�����E�ɁI
		//mRotation.mY--;
		if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
			mTurnSpeed = -0.5f;
		}
		if (mTurnSpeed > 0.0f){
			mTurnSpeed -= 0.11f;
		}
		mTurnSpeed -= 0.04f;
	}
	else{
		if (mTurnSpeed > 0.0f){
			mTurnSpeed -= 0.05f;
		}
		else if (mTurnSpeed < 0.0f){
			mTurnSpeed += 0.05f;
		}
		if (mTurnSpeed<0.04f && mTurnSpeed>-0.04f){
			mTurnSpeed = 0.0f;
		}
	}
	if (mTurnSpeed > 1.0f){
		mTurnSpeed = 1.0f;
	}
	else if (mTurnSpeed < -1.0f){
		mTurnSpeed = -1.0f;
	}
	mRotation.mY += mTurnSpeed;
	

	if (CKey::Push('Z')){
		mRotation.mX++;
	}
	if (CKey::Push('X')){
		mRotation.mX--;
	}
	if (CKey::Push('C')){
		mRotation.mZ+=2;
	}
	if (CKey::Push('V')){
		mRotation.mZ-=2;
	}
	if (mRotation.mZ > 180){
		mRotation.mZ = -180;
	}
	else if (mRotation.mZ < -180){
		mRotation.mZ = 180;
	}

	//�O�ɎԂ��i��ł��鎞
	if (mCarSpeed > 0.0f){
		if (isSoundEngine == false){
			SoundEngine.Repeat();
			isSoundEngine = true;
		}		
	}
	//�Ԃ���~���Ă��鎞
	else if (mCarSpeed == 0.0f){
		SoundEngine.Stop();
		isSoundEngine = false;
	}
	//�Ԃ��o�b�N���Ă��鎞
	else if (mCarSpeed < 0.0f){
		if (isSoundEngine == false){
			//�o�b�N�͈Ⴄ����炷�\��
			SoundEngine.Repeat();
			isSoundEngine = true;
		}
	}

	//mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ)  * CMatrix().RotateY(mRotation.mY) * mMatrix;

	/*mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ) * CMatrix().RotateY(90) * mMatrix;*/
	//mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ+mCarSpeed) * CMatrix().RotateY(90) * mMatrix;

	/*��ɒn�ʂɐ����ɗ�������悤�ɂ���(�v���C���[�̐^���ł͂Ȃ�)*/
	//mPosition = CVector(mADMoveX, mVelocityJump, mWSMoveZ + mCarSpeed) * mMatrix;

	//mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrix;
	mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();
	//Y����(�d��)�͕�����
	//mPosition = CVector(0.0f, mVelocityJump, 0.0f) * mMatrix;//�ł��ĂȂ�
	mPosition = CVector(0.0f, mVelocityJump*2.0f, 0.0f) * //mMatrixScale * 
		CMatrix().RotateZ(0) *
		CMatrix().RotateX(0) *
		CMatrix().RotateY(0)
		*mMatrixTranslate;//�ł��Ă�H
	//mMatrix = mMatrixScale * mMatrixRotate * mMatrixTranslate;

	//�]�����Ă��܂�����(R�L�[�ő����X�^�[�g)
	if (mPosition.mY < -150.0f || CKey::Once('R')){
		//�����̐�����0�ɂ���
		mVelocityJump = 0.0f;
		//�Ԃ̑��x��0��
		mCarSpeed = 0.0f;

		mRotation = CVector(0.0f, 0.0f, 0.0f);
		if (mChecks == 0){
			//�X�^�[�g�������̈ʒu�A�����ɖ߂����
			mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
			mRotation.mY = 0.0f;
		}
		else if (mChecks == 1){
			mPosition = CVector(-80.0f, mStartPoint[1], 2175.0f);
			mRotation.mY = -55.0f;
		}
		else if (mChecks == 2){
			mPosition = CVector(-1620.0f, mStartPoint[1], 450.0f);
			mRotation.mY = -175.0f;
		}
		else if (mChecks == 3){
			mPosition = CVector(-1212.0f, mStartPoint[1], -1616.0f);
			mRotation.mY = 120.0f;
		}
	}

	CCharacter::Update();
	
	//�d�͂̉e���𔽉f����
	mVelocityJump -= G;
	if (CKey::Once('K')){
		if (mCanJump){
			//�{�[�g��D���̓W�����v�s��
			if (mHaveBoat){
			}
			else{
				mCanJump = false;
				mVelocityJump = mJumpV0;
				mJumpPrio = 2;

				////�W�����v���Đ�
				SoundJump.Play();
			}
		}
	}
	if (mJumpPrio > 0){
		mJumpPrio--;
	}
	
}

void CPlayer::Collision(CCollider *mc, CCollider *yc){
	//���g�̃R���C�_�^�C�v�̔���
	switch (mc->mType){
	case CCollider::ESPHERE://���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (yc->mType == CCollider::ETRIANGLE){
			////���j�\�Ȏ��͐���(���̏�)��ʉ߂ł���
			//if (yc->mTag == EWATER && mCanSwim){
			//}
			//else{
			//}

			////�`�F�b�N�|�C���g�֘A�̏���
			//if (CCollider::Collision(mc, yc)){
			//	if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//���Ԓn�_1
			//		if (mChecks == 0){
			//			//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
			//			mChecks = 1;
			//		}
			//	}
			//	if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//���Ԓn�_2
			//		if (mChecks == 1){
			//			//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
			//			mChecks = 2;
			//		}
			//	}
			//	if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//���Ԓn�_3
			//		if (mChecks == 2){
			//			//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
			//			mChecks = 3;
			//		}
			//	}
			//}
			
			//�����̃R���C�_���{�̂̎�
			if (mc->mTag == CCollider::EBODY){
				//�Ő��ʉߒ��̏���
				if (yc->mpParent->mTag == CCharacter::EGRASS){
					CVector aiueo;//���̃x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//�u�[�X�g���ʂ̕����D�悳���
						if (isBoost == false){
							printf("speed down�c\n");
							//��葬�x�܂ŃX�s�[�h�ቺ
							if (mCarSpeed > 3.0f){
								if (mCarSpeed > 3.3f){
									mCarSpeed -= 0.3f;
								}
								else{
									mCarSpeed = 3.0f;
								}
							}
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//���Ԓn�_1
					if (mChecks == 0){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector aiu;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 1;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//���Ԓn�_2
					if (mChecks == 1){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector aiu;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 2;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//���Ԓn�_3
					if (mChecks == 2){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector aiu;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 3;
						}
					}
				}
				////�������ɏ�������̏���
				//if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
				//	CVector aiueo;//�Ƃ�܃x�N�g��
				//	if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
				//		//mCarSpeed += 10.0f;
				//		isBoost = true;
				//		mBoostTime = 45;
				//		printf("SPEED UP!\n");
				//	}
				//}



				if (mCanSwim && yc->mpParent->mTag == CCharacter::EWATER){
					//�ʉ߉\�A�X�e�[�W1�̐�
				}
				else if (yc->mpParent->mTag == CCharacter::ECLEARWATER){
					//�ʉ߉\�A�X�e�[�W2�̐�
				}
				else if (yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD){
					//�����͍s���邪�A�����̃p�l���͒ʉ߉\
				}
				else{
					if (mJumpPrio < 1){

						CVector adjust;//�����p�x�N�g��
						//		//�O�p�`�Ƌ��̏Փ˔���
						//		CCollider::CollisionTriangleSphere(yc, mc, &adjust);
						if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
							//�ʒu�̍X�V
							mPosition = mPosition - adjust * -1;
							//�s��̍X�V
							CCharacter::Update();

							if (yc->mpParent->mTag == CCharacter::EWALL){
								//�Փ˂����̂��ǂ������ꍇ�͕ǂɂ͈��������炸����
								//�ǂɂԂ���ƏՓˉ������A�Ԃ���������
								//�������ɏՓ˂Ō����A�x�����̏Փ˂͓��ɕω��Ȃ�
								if (mCarSpeed > 4.5f){
									mCarSpeed = -0.4f;
									//mCarSpeed /= 2.0f;
									SoundCollision.Play();
									printf("��ݯ");
								}
								else if (mCarSpeed > 3.0f){
									mCarSpeed = -0.4f;
									//mCarSpeed /= 2.0f;
									SoundCollisionSmall.Play();
									printf("�޽�");
								}
								else{
									//�ǂɂ�����Ȃ���̈ړ����A���x���x���Ȃ�
									if (mCarSpeed > 2.0f){
										mCarSpeed = 2.0f;
									}
								}
								//mCarSpeed = -mCarSpeed * 1.0;
								//mVelocityJump = 2.0f;
								
							}
							else{
								mVelocityJump = 0;
								mCanJump = true;

								/*if (mRotation.mX < yc->mpParent->mRotation.mX){
								mRotation.mX++;
								}
								else if (mRotation.mX > yc->mpParent->mRotation.mX){
								mRotation.mX--;
								}*/

								int rotateofycmx = yc->mpParent->mRotation.mX;
								rotateofycmx %= 360; //-360�x����360�x�܂ł̐��l�ɕϊ�
								//-235=125 300=-60 -180�x�������A180�x�ȏ�̊p�x��
								if (rotateofycmx < -180){
									rotateofycmx += 360;
								}
								else if (rotateofycmx >= 180){
									rotateofycmx -= 360;
								}
								mRotation.mX = rotateofycmx;
								//if (mRotation.mX < yc->mpParent->mRotation.mX){
								//	mRotation.mX = yc->mpParent->mRotation.mX;
								//}
								//else if (mRotation.mX > yc->mpParent->mRotation.mX){
								//	mRotation.mX = yc->mpParent->mRotation.mX;
								//}
								//mRotation = yc->mpParent->mRotation;
							}
						}
					}

				}
			}
			//�����̃R���C�_������̎�
			if (mc->mTag == CCollider::ESEARCH){
				//�������ɏ�������̏���
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector aiueo;//�Ƃ�܃x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//mCarSpeed += 10.0f;
						isBoost = true;
						mBoostTime = 45;
						printf("SPEED UP!\n");
						SoundBoost.Play();
					}
				}
			}

			

			
			/*if(yc->mpParent->mTag == CCharacter::ESPRING){
				mJumpV0 = 2.3f;
			}*/
			/*if (CItem::mpItem->mItemNumber == 2){
				mJumpV0 = 2.3f;
			}*/
		}
		if (yc->mType == CCollider::ESPHERE){
			if (CCollider::Collision(mc, yc)){
				if (yc->mpParent->mTag == CCharacter::ESPRING){
					mJumpV0 = 2.2f;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::ESCREW){
					mCanSwim = true;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::EGUN){
					mCanFire = true;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::EKEY){
					mIsGetKey = true;
					SoundItemGet.Play();
				}
				if (yc->mpParent->mTag == CCharacter::EBOAT){//�{�[�g��D��
					mHaveBoat = true;
					mMoveSpeed = 0.85f;
				}
			}
		}

		break;
	}

	////���R���ƎO�p�`�̎�
	//if (mc->mType == CCollider::ESPHERE && yc->mType == CCollider::ETRIANGLE){
	//	CVector adjust;//�����p�x�N�g��
	//	//�R���C�_��m��y���Փ˂��Ă��邩����
	//	//�O�p�`�Ƌ��̏Փ˔���
	//	if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
	//		//�ʒu�̍X�V
	//		mPosition = mPosition - adjust * -1;
	//		//�s��̍X�V
	//		CCharacter::Update();
	//	}
	//}

}