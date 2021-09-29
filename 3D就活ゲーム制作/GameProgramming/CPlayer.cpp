#include "CPlayer.h"
#include "CKey.h"

#include "CSceneTitle.h"

//�Փˎ��̃G�t�F�N�g�ǉ�
#include "CEffect.h"
extern std::shared_ptr<CTexture> TextureExp;
extern std::shared_ptr<CTexture> TextureHit;
extern std::shared_ptr<CTexture> TextureBoost;
#include "CTaskManager.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592

extern CSound SoundJump;
extern CSound SoundShot;
extern CSound SoundItemGet;
extern CSound SoundBoost;
extern CSound SoundEngine;
extern CSound SoundHorn;
extern CSound SoundCollision;
extern CSound SoundCollisionSmall;
extern CSound SoundSink;

CPlayer *CPlayer::mpPlayer = 0;

#define G (9.8f / 90.0f)//�d�͉����x//60.0f
#define JUMPV0 (16.0f)//�W�����v����//4.0f
#define MAXSPEED 20.0f //�Ԃ̍ō����x
#define MAXSPEED_BACK 4.0f//�Ԃ̌�ނ���ő呬�x
#define CAR_POWER 0.1f //*2//1�t���[���ӂ�̎Ԃ̉������Ă�����
#define CAR_BREAK_POWER 0.1f//�O�i���̃u���[�L�̋���
#define DECELERATE 0.1f //�Ԃ̌��������
#define FIX_ANGLE_VALUE 1.0f //�p�x��0�x�Ɍ����Ē���������(���X�EZ�p)
#define JUMPER01_POWER 3.0f;//�W�����v��1�ɂ��W�����v�̋���

CPlayer::CPlayer()
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f*3)
, mColTire(this, CVector(0.0f, -16.0f + 15.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
{
	mpPlayer = this;

	mScale = CVector(7.5f, 7.5f, 7.5f);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//�o�l�擾���2.3f�̗\��
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//�Ԃ̑��x�̏�����
	mTurnSpeed = 0.0f;

	mFlyingMode = false;

	CanMove = false;
	isRespawn = false;
	isSink = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EPLAYER;
	mJumpPrio = 0;
	
	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;

	SoundJump.Load("SE\\jump12.wav");
	SoundShot.Load("SE\\shot1.wav");
	SoundItemGet.Load("SE\\se_maoudamashii_system46.wav");	
	SoundBoost.Load("SE\\Shortbridge31-3.wav");
	SoundEngine.Load("SE\\SNES-Racing01-02.wav");
	SoundEngine_Turf.Load("SE\\SNES-Racing02-02.wav");
	SoundHorn.Load("SE\\car-horn1.wav");
	SoundCollision.Load("SE\\bomb1.wav");
	SoundCollisionSmall.Load("SE\\SNES-Racing01-10(Collision).wav");
	SoundSink.Load("SE\\Diving.wav");

	isSoundEngine = false;	
	isTouchGoal = false;
	mGoalTime = 0; mRank = 1;

	mSound_Engine = mSound_Engine_Prev = ENONE;
}

void CPlayer::Update(){
	//�f�o�b�O�p
#ifdef _DEBUG
	//��s���[�h
	if (mFlyingMode){
		if (CKey::Push('W')){
			mVelocityJump = mMoveSpeed * 3;
		}
		else if (CKey::Push('S')){
			mVelocityJump = -mMoveSpeed * 3;
		}
		else{
			mVelocityJump = 0.0f;
		}
	}
#endif

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
	
	if (CKey::Push(VK_LEFT) && CanMove){//�n���h�������ɁI
		/*�o�b�N���͋t�����ɋȂ���*/
		if (mCarSpeed > 0.0f){
			if (mTurnSpeed >= 0.0f&&mTurnSpeed<0.5f){
				mTurnSpeed = 0.5f;
			}
			if (mTurnSpeed < 0.0f){
				mTurnSpeed += 0.11f;
			}
			mTurnSpeed += 0.04f;
		}
		else if (mCarSpeed < 0.0f){
			if (mTurnSpeed >= 0.0f&&mTurnSpeed<0.5f){
				mTurnSpeed = -0.5f;
			}
			if (mTurnSpeed < 0.0f){
				mTurnSpeed += -0.11f;
			}
			mTurnSpeed += -0.04f;
		}
	}
	else if (CKey::Push(VK_RIGHT) && CanMove){//�n���h�����E�ɁI		
		/*�o�b�N���͋t�����ɋȂ���*/
		if (mCarSpeed > 0.0f){
			if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
				mTurnSpeed = -0.5f;
			}
			if (mTurnSpeed > 0.0f){
				mTurnSpeed -= 0.11f;
			}
			mTurnSpeed -= 0.04f;
		}
		else if(mCarSpeed < 0.0f){
			if (mTurnSpeed <= 0.0f&&mTurnSpeed>-0.5f){
				mTurnSpeed = 0.5f;
			}
			if (mTurnSpeed > 0.0f){
				mTurnSpeed -= -0.11f;
			}
			mTurnSpeed -= -0.04f;
		}
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
	float turnspd = mTurnSpeed;
	if (mCarSpeed > -4.0f && mCarSpeed < 4.0f){
		if (mCarSpeed >= 0.0f){
			turnspd = mTurnSpeed * mCarSpeed / 4.0f;
		}
		else{
			turnspd = mTurnSpeed * -mCarSpeed / 4.0f;
		}		
	}
	else{
		turnspd = mTurnSpeed;
	}
	mRotation.mY += turnspd;
	
	if (mRotation.mZ > 180){
		mRotation.mZ = -180;
	}
	else if (mRotation.mZ < -180){
		mRotation.mZ = 180;
	}

	mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();
	//Y����(�d��)�͕�����
	//mPosition = CVector(0.0f, mVelocityJump, 0.0f) * mMatrix;//�ł��ĂȂ�
	mPosition = CVector(0.0f, mVelocityJump*2.0f, 0.0f) * //mMatrixScale * 
		CMatrix().RotateZ(0) *
		CMatrix().RotateX(0) *
		CMatrix().RotateY(0)
		*mMatrixTranslate;

	//�]�����Ă��܂�����(R�L�[�ő����X�^�[�g)
	if (mPosition.mY < -700.0f || CKey::Once('R')){
		//�����̐�����0�ɂ���
		mVelocityJump = 0.0f;
		//�Ԃ̑��x��0��
		mCarSpeed = 0.0f;
		//���W�A��]�l��ύX����
		mPosition = mVCheckPositions[mChecks];
		mRotation = mVCheckRotations[mChecks];
		isSink = false;
		isRespawn = true;		
	}
	CCharacter::Update();
	
	//�d�͂̉e���𔽉f����
	mVelocityJump -= G;
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
			//�����̃R���C�_���{�̂̎�
			if (mc->mTag == CCollider::EBODY){				
				//�Ő��ʉߒ��̏���
				if (yc->mpParent->mTag == CCharacter::EGRASS){
					CVector tmpVec;//���x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
						//�u�[�X�g���ʂ̕����D�悳���
						if (isBoost == false){
							//��葬�x�܂ŃX�s�[�h�ቺ
							if (mCarSpeed > 3.2f + 1.8f){
								if (mCarSpeed > 4.0f + 1.8f){
									mCarSpeed -= 0.8f;
								}
								else{
									mCarSpeed = 3.2f +1.8f;
								}
							}
						}
					}
				}
				//���Ԓn�_�ʉߎ��̏���
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//���Ԓn�_1
					if (mChecks == 0){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector tmpVec;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							mChecks = 1;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//���Ԓn�_2
					if (mChecks == 1){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector tmpVec;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							mChecks = 2;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//���Ԓn�_3
					if (mChecks == 2){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector tmpVec;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							mChecks = 3;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::EGOALPOINT){//���Ԓn�_3
					if (mChecks == 3){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector tmpVec;//�����킹�̂��߂����̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							isTouchGoal = true;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EGOALPOINT
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD
					|| yc->mpParent->mTag == CCharacter::EWATER){
					//�����̃^�O���t�����̂͒ʉ߉\
					if (yc->mpParent->mTag == CCharacter::EWATER){
						CVector tmpVec;//�l��Ԃ����߂̃x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
							if (isSink == false){
								SoundSink.Play();
								isSink = true;
							}
						}
					}
				}
				else{
					if (mJumpPrio < 1){
						CVector adjust;//�����p�x�N�g��
						//�O�p�`�Ƌ��̏Փ˔���
						if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
							//�ʒu�̍X�V
							mPosition = mPosition - adjust * -1;
							//�s��̍X�V
							CCharacter::Update();

							if (yc->mpParent->mTag == CCharacter::EGRASS){
								mSound_Engine = EONGRASS;
							}
							else{
								mSound_Engine = ENOTONGRASS;
							}							
							
							//�G���W������炷
							if (mCarSpeed > 0.0f){
								if (isSoundEngine == false){
									mSound_Engine_Prev = mSound_Engine;
									if (mSound_Engine == EONGRASS){
										SoundEngine_Turf.Repeat();										
									}
									else if (mSound_Engine == ENOTONGRASS){
										SoundEngine.Repeat();
									}									
									isSoundEngine = true;
								}
								else{
									//�Ő��A��Ő��̐؂�ւ��
									if (mSound_Engine_Prev != mSound_Engine){
										mSound_Engine_Prev = mSound_Engine;
										if (mSound_Engine_Prev == EONGRASS){
											SoundEngine_Turf.Repeat();
											SoundEngine.Stop();
										}
										else if (mSound_Engine_Prev == ENOTONGRASS){
											SoundEngine.Repeat();
											SoundEngine_Turf.Stop();
										}
									}
								}
							}
							//�Ԃ���~���Ă��鎞
							else if (mCarSpeed == 0.0f){
								SoundEngine.Stop();
								SoundEngine_Turf.Stop();
								isSoundEngine = false;
							}
							//�Ԃ��o�b�N���Ă��鎞
							else if (mCarSpeed < 0.0f){
								if (isSoundEngine == false){
									mSound_Engine_Prev = mSound_Engine;
									if (mSound_Engine == EONGRASS){
										SoundEngine_Turf.Repeat();
									}
									else if (mSound_Engine == ENOTONGRASS){
										SoundEngine.Repeat();
									}
									isSoundEngine = true;
								}
								else{
									//�Ő��A��Ő��̐؂�ւ��
									if (mSound_Engine_Prev != mSound_Engine){
										mSound_Engine_Prev = mSound_Engine;
										if (mSound_Engine_Prev == EONGRASS){
											SoundEngine_Turf.Repeat();
											SoundEngine.Stop();
										}
										else if (mSound_Engine_Prev == ENOTONGRASS){
											SoundEngine.Repeat();
											SoundEngine_Turf.Stop();
										}
									}
								}
							}
							if (yc->mpParent->mTag == CCharacter::EWALL){								
								//�ǏՓˏ����̏C����
								if (mCarSpeed > 2.0f){
									mCarSpeed *= 0.95f;
									if (mCarSpeed < 2.0f){
										mCarSpeed = 2.0f;
									}
								}
							}
							else if(yc->mpParent->mTag == CCharacter::EJUMPER){//�W�����v��ɐڐG������
								mVelocityJump = JUMPER01_POWER;
								SoundJump.Play();
							}
							else{
								mVelocityJump = 0;
								mRotation = CCollider::CalculateEulerAngle(mc, yc, mMatrixRotate, PI);
							}
						}
					}

				}
			}
			//�����̃R���C�_������̎�
			if (mc->mTag == CCollider::ESEARCH){
				//�������ɏ�������̏���
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector tmpVec;//�l��Ԃ����߂̃x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &tmpVec)){
						//mCarSpeed += 10.0f;
						//mCarSpeed = MAXSPEED+10.0f;
						if (isBoost == false){
							SoundBoost.Play();
							new CEffect(mPosition + CVector(0.0f, 15.5f, 0.0f), 60.0f, 60.0f, TextureBoost, 3, 5, 1, 1);
						}
						isBoost = true;
						mBoostTime = 45;
					}
				}
			}
		}
		if (yc->mType == CCollider::ESPHERE){
			if (CCollider::Collision(mc, yc)){
				if (mc->mTag == CCollider::EBODY){
					if (yc->mpParent->mTag == CCharacter::EENEMY){
						if (yc->mTag == CCollider::EBODY){
							CVector adjust;//�����p�x�N�g��
							//�����m�̏Փ˔���
							if (CCollider::Collision(mc, yc, &adjust)){
								//�ʒu�̍X�V
								mPosition = mPosition - adjust * -1;
								//�s��̍X�V
								CCharacter::Update();
							}
						}
					}
				}
				
			}
		}
		break;
	}
}

//�e�`�F�b�N�|�C���g�ʉߎ��̃��X�|�[���n�_�����߂�
void CPlayer::SetRespawnPoint(int checknumber, CVector position, CVector rotation){
	mVCheckPositions[checknumber] = position;
	mVCheckRotations[checknumber] = rotation;
}

//�X�^�[�g�n�_�ɒ���(��Ƀ��[�X�J�n���ɌĂяo��)
void CPlayer::GetReady(){
	mPosition = mVCheckPositions[0];
	mRotation = mVCheckRotations[0];
	CCharacter::Update();
}

void CPlayer::TaskCollision()
{
	mColBody.ChangePriority();
	mColTire.ChangePriority();
	CollisionManager.Collision(&mColBody);
	CollisionManager.Collision(&mColTire);
}