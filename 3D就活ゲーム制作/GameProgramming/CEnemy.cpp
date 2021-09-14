#include "CEnemy.h"
#include "CKey.h"

#include "CBullet.h"
#include "CSceneTitle.h"

//��������������C���N���[�h�Q
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//�O�p�֐��֘A�̃C���N���[�h
#include <math.h>
#define PI 3.141592

//�Փˎ��̃G�t�F�N�g�ǉ�
#include "CEffect.h"
extern std::shared_ptr<CTexture> TextureExp;
extern std::shared_ptr<CTexture> TextureHit;
extern std::shared_ptr<CTexture> TextureBoost;
#include "CTaskManager.h"

CEnemy *CEnemy::mpEnemy = 0;

#define G (9.8f / 90.0f)//�d�͉����x
#define JUMPV0 (16.0f)//�W�����v����

#define MAXSPEED 20.0f//�Ԃ̍ō����x
#define MINSPEED 1.0f//�Ԃ̍Œᑬ�x
#define MAXSPEED_BACK 4.0f//�Ԃ̌�ނ���ő呬�x
#define CAR_POWER 0.1f//1�t���[���ӂ�̎Ԃ̉������Ă�����
#define CAR_BREAK_POWER 0.1f//�O�i���̃u���[�L�̋���

#define DECELERATE 0.1f //�Ԃ̌��������
#define FIX_ANGLE_VALUE 1.0f //�p�x��0�x�Ɍ����Ē���������(��]�lX,Z�p)
#define JUMPER01_POWER 3.0f //�W�����v��1�ł̃W�����v��
#define RESTART_TIME 15*60 //�G���l�܂������A��莞�Ԍo�߂Ń��X�^�[�g������(�b��*60fps)

CEnemy::CEnemy()
//�ԑ̂�Y���W��0.0f�ɂ������񂾂��ǁE�E�E
//0.0f�ɂ�����ԑ̂������Ă�悤�Ɍ����Ă��܂�
:mColBody(this, CVector(0.0f, 4.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
, mColTire(this, CVector(0.0f, -16.0f + 15.0f + 1.0f, 0.5f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
, mSearch(this, CVector(0.0f, 20.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 15)
{
	srand(time(NULL));

	mpEnemy = this;

	mScale = CVector(2.5f * 3, 2.5f * 3, 2.5f * 3);

	mVelocityJump = 0.0f;
	mJumpV0 = 1.1f;//�o�l�擾���2.3f�̗\��
	mMoveSpeed = 0.5f;
	mADMoveX = 0.0f;  mWSMoveZ = 0.0f;
	mCarSpeed = 0.0f;//�Ԃ̑��x�̏�����
	mTurnSpeed = 0.0f;

	mCanJump = false;
	CanMove = false;

	mChecks = 0;

	isBoost = false;
	mBoostMaxSpeed = 0.0f;
	mBoostTime = 0;

	mTag = EENEMY;
	mEnemyAI = EPRO;

	//�X�^�[�g�n�_�̍��W��ݒ�;
	mStartPoint[0] = 350.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -100.0f;

	mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	mColBody.mTag = CCollider::EBODY;
	mColTire.mTag = CCollider::ESEARCH;
	mSearch.mTag = CCollider::ESEARCH;
	
	mPointTime = 0;//���|�C���g�Ɉڂ��Ă���̌o�ߎ���

	mpPoint = mPoint;
	mVPoint = mpPoint->mPosition;//��ԍŏ��͕��U����
	mVPoint_prev = mVPoint;

	mMaxSpeed_PtoP = 20.0f;

	mEnemyLap = 1;//�G�̃��b�v�����P���ڂɐݒ肷��
	isTouchGoal = false;
	isEnemyGoaled = false;//�܂��S�[�����ĂȂ���Ԃɂ���
	mGoalTime = 0; mRank = 1;
}

void CEnemy::Update(){

	//���x����
	if (CSceneTitle::mCource_Number == 1){
		//���̃|�C���g���玟�̎��̃|�C���g�ւ̃x�N�g��
		CVector vNext = mpPoint->GetNextPoint()->mPosition - mPosition;
		//���݂̌���
		CVector vLeft = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//���ς���Ȃ��������߂�(0:90���@1.0�F�^�������j
		float corve = abs(vLeft.Dot(vNext.Normalize()));
		if (corve > 0.9f){
			corve = 0.7f;
		}
		else if (corve < 0.7f){
			corve = 1.0f;
		}
		//���x����̌v�Z
		mMaxSpeed_PtoP = MAXSPEED * corve;
	}
	else if(CSceneTitle::mCource_Number == 2){
		//���̃|�C���g���玟�̎��̃|�C���g�ւ̃x�N�g��
		CVector vNext = mpPoint->GetNextPoint()->mPosition - mPosition;
		//���݂̌���
		CVector vLeft = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//���ς���Ȃ��������߂�(0:90���@1.0�F�^�������j
		float corve = abs(vLeft.Dot(vNext.Normalize()));
		/*if (corve > 0.9f){
			corve = 0.05f;
		}*/
		if (corve < 0.5f){
			corve = 1.0f;
		}
		//���x����̌v�Z
		mMaxSpeed_PtoP = MAXSPEED * corve;
	}
	else if (CSceneTitle::mCource_Number == 5){
		//���̃|�C���g���玟�̎��̃|�C���g�ւ̃x�N�g��
		CVector vNext = mpPoint->GetNextPoint()->mPosition - mPosition;
		//���݂̌���
		CVector vLeft = CVector(1.0f, 0.0f, 0.0f) * mMatrixRotate;
		//���ς���Ȃ��������߂�(0:90���@1.0�F�^�������j
		float corve = abs(vLeft.Dot(vNext.Normalize()));
		if (corve > 0.6f){
			corve = 0.05f;
		}
		else if (corve < 0.5f){
			corve = 1.0f;
		}
		//���x����̌v�Z
		mMaxSpeed_PtoP = MAXSPEED * corve;
	}
	else{		
		mMaxSpeed_PtoP = 20.0f;
	}
	//�X�s�[�h�͍Œᑬ�x�������Ȃ�
	if (mMaxSpeed_PtoP < MINSPEED){
		mMaxSpeed_PtoP = MINSPEED;
	}

	//�|�C���g�ւ̃x�N�g�������߂�
	CVector dir = mVPoint - mPosition;
	//�������ւ̃x�N�g�������߂�
	CVector left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);

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


	if (CKey::Push(VK_UP) && CanMove && mCarSpeed < mMaxSpeed_PtoP || mChecks >= 0 && CanMove && mCarSpeed < mMaxSpeed_PtoP){
		if (mCarSpeed < MAXSPEED + mBoostMaxSpeed){
			/*if (left.Dot(dir) > -5.0f && left.Dot(dir) < 5.0f){
				mCarSpeed += CAR_POWER;
			}*/
			mCarSpeed += CAR_POWER;
		}
	}
	/*else if (CKey::Push(VK_DOWN) && CanMove || left.Dot(dir) < -5.0f && CanMove || left.Dot(dir) > 5.0f && CanMove)*/
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

	//�ړI�n�������ɂ���A����\�Ȏ�
	if (left.Dot(dir) > 0.0f && CanMove){ //�n���h�������ɁI
		//mRotation.mY++;
		if (mTurnSpeed >= 0.0f&&mTurnSpeed<0.5f){
			mTurnSpeed = 0.5f;
		}
		if (mTurnSpeed < 0.0f){
			mTurnSpeed += 0.11f;
		}
		mTurnSpeed += 0.04f;
	}
	//���邢�͖ړI�n���E���ʂŁA����\�Ȏ�
	else if (left.Dot(dir) < 0.0f && CanMove){//�n���h�����E�ɁI
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

	if (mMaxSpeed_PtoP<2.0f){
		mTurnSpeed *= 2.0f;//�ᑬ���͉�]���\�A�b�v����~����Ɖ�]�s��
	}

	mRotation.mY += mTurnSpeed;

	if (mRotation.mZ > 180){
		mRotation.mZ = -180;
	}
	else if (mRotation.mZ < -180){
		mRotation.mZ = 180;
	}
		
	//X,Z�����̈ړ���Y������(�d��)�̈ړ��͕ʁX�ɍs��
	mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	CCharacter::Update();
	//��ɒn�ʂɑ΂��Đ����ɗ���
	mPosition = CVector(0.0f, mVelocityJump*2.0f, 0.0f) *
		CMatrix().RotateZ(0) *
		CMatrix().RotateX(0) *
		CMatrix().RotateY(0)
		*mMatrixTranslate;

	//�R�[�X�A�E�g�������A�������͕ǂɋl�܂铙���ĖڕW�n�_����莞�ԍX�V����Ȃ�������
	if (mPosition.mY < -700.0f || mPointTime > RESTART_TIME){
		mPointTime = 0;
		//�����̐�����0�ɂ���
		mVelocityJump = 0.0f;
		//�Ԃ̑��x��0��
		mCarSpeed = 0.0f;
		//1�O�̖ڕW�n�_�ɖ߂����
		mPosition = mVPoint_prev;
	}
	CCharacter::Update();

	//�d�͂̉e���𔽉f����
	mVelocityJump -= G;
	
	if (CKey::Push('I')){
		mpPoint->mRotation.mY++;
	}	
}

void CEnemy::Collision(CCollider *mc, CCollider *yc){
	//���g�̃R���C�_�^�C�v�̔���
	switch (mc->mType){
	case CCollider::ESPHERE://���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (yc->mType == CCollider::ETRIANGLE){
			//�����̃R���C�_���{�̂̎�
			if (mc->mTag == CCollider::EBODY){
				//�Ő��ʉߒ��̏���
				if (yc->mpParent->mTag == CCharacter::EGRASS){
					CVector aiueo;//���̃x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						//�u�[�X�g���ʂ̕����D�悳���
						if (isBoost == false){
							//��葬�x�܂ŃX�s�[�h�ቺ
							if (mCarSpeed > 3.2f + 1.8f){
								if (mCarSpeed > 4.0f + 1.8f){
									mCarSpeed -= 0.8f;
								}
								else{
									mCarSpeed = 3.2f + 1.8f;
								}
							}
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT){//���Ԓn�_1
					if (mChecks == 0){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector aiu;//�l�Ԃ��p�x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 1;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT2){//���Ԓn�_2
					if (mChecks == 1){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector aiu;//�l�Ԃ��p�x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 2;
							//mChecks = 0;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::ECHECKPOINT3){//���Ԓn�_3
					if (mChecks == 2){
						//�e���Ԓn�_��ʉ߂��Ȃ���1�������Ƃ݂Ȃ���Ȃ�
						CVector aiu;//�l�Ԃ��p�x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							mChecks = 3;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::EGOALPOINT){//�S�[���n�_
					if (mChecks == 3){
						//�e���Ԓn�_��ʉ߂��Ă��Ȃ����1�����肪�Ȃ���Ȃ�
						CVector aiu;//�l�Ԃ��p�x�N�g��
						if (CCollider::CollisionTriangleSphere(yc, mc, &aiu)){
							isTouchGoal = true;
						}
					}
				}
				if (yc->mpParent->mTag == CCharacter::EWATER){
					//�ʉ߉\�A�X�e�[�W1�̐�
				}
				else if (yc->mpParent->mTag == CCharacter::ECLEARWATER){
					//�ʉ߉\�A�X�e�[�W2�̐�
				}
				else if (yc->mpParent->mTag == CCharacter::ECHECKPOINT
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT2
					|| yc->mpParent->mTag == CCharacter::ECHECKPOINT3
					|| yc->mpParent->mTag == CCharacter::EGOALPOINT
					|| yc->mpParent->mTag == CCharacter::EDASHBOARD){
					//�����̃^�O���t�����I�u�W�F�N�g�͒ʉ߉\
				}
				else{
					CVector adjust;//�����p�x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
						//�ʒu�̍X�V
						mPosition = mPosition - adjust * -1;
						//�s��̍X�V
						CCharacter::Update();
						if (yc->mpParent->mTag == CCharacter::EWALL){
							//�������ɏՓ˂Ō����A�x�����̏Փ˂͓��ɕω��Ȃ�
							if (mCarSpeed > 6.5f){
								mCarSpeed = 2.0f;
								//���ˎ��A�G�t�F�N�g����
								new CEffect(mPosition + CVector(0.0f, 35.0f, 0.0f), 100.0f, 100.0f, TextureExp, 4, 4, 1, 0);
							}
							else if (mCarSpeed > 4.0f){
								mCarSpeed = 2.0f;
								//�y���Ԃ��������G�t�F�N�g����
								new CEffect(mPosition + CVector(0.0f, 15.5f, 0.0f), 60.0f, 60.0f, TextureHit, 3, 8, 1, 1);
							}
							else{
								//�ǂɂ�����Ȃ���̈ړ����A���x���x���Ȃ�
								if (mCarSpeed > 2.0f){
									mCarSpeed = 2.0f;
								}
							}
						}
						else if (yc->mpParent->mTag == CCharacter::EJUMPER){//�W�����v��ɐڐG������
							mVelocityJump = JUMPER01_POWER;
							mCanJump = true;
						}
						else{
							mVelocityJump = 0;
							mCanJump = true;
							mRotation = CCollider::CalculateEulerAngle(mc, yc, mMatrixRotate, PI);
						}
					}
					

				}
			}
			//�����̃R���C�_�����G�R���C�_�̎�
			if (mc->mTag == CCollider::ESEARCH){
				//�������ɏ�������̏���
				if (yc->mpParent->mTag == CCharacter::EDASHBOARD){
					CVector aiueo;//�Ƃ�܃x�N�g��
					if (CCollider::CollisionTriangleSphere(yc, mc, &aiueo)){
						if (isBoost == false){
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
				//
				if (mc->mTag == CCollider::EBODY){
					if (yc->mpParent->mTag == CCharacter::EPLAYER
						&& yc->mTag == CCollider::EBODY){
						CVector adjust;//�����p�x�N�g��
						////		//�����m�̏Փ˔���
						if (CCollider::Collision(mc, yc, &adjust)){
							//�ʒu�̍X�V
							mPosition = mPosition - adjust * -1;
							//�s��̍X�V
							CCharacter::Update();
						}
					}
					
					if (yc->mpParent->mTag == CCharacter::EENEMY
						&& yc->mTag == CCollider::EBODY){
						if (mc == yc){
							return;
						}
						CVector adjust;//�����p�x�N�g��
						////		//�����m�̏Փ˔���
						if (CCollider::Collision(mc, yc, &adjust)){
							//�ʒu�̍X�V
							mPosition = mPosition - adjust * -1;
							//�s��̍X�V
							CCharacter::Update();
						}
					}					
				}
				//
				if (mc->mTag == CCollider::ESEARCH){
					//�|�C���^����|�C���^�Ɍ����Ĉړ�����
					if (yc->mpParent->mTag == CCharacter::EPOINT){
						CVector adjust;//�����p�x�N�g��
						//		//�����m�̏Փ˔���
						if (CCollider::Collision(mc, yc, &adjust)){
							//�Փ˂����|�C���^�Ɩڎw���Ă���|�C���^��������
							if (yc->mpParent == mpPoint){
								//�|�C���g�o�ߎ��Ԃ̃��Z�b�g
								mPointTime = 0;
								//�Ƃ肠�������int�t���Ő������Ă���
								int r = (mc->mRadius + yc->mRadius) * 0.8f;
								int gap = (rand() % (r * 2) - r);
								//�GAI��Lv�ɂ�蕪�U�l���ω�
								if (CSceneTitle::mCPU_Level == 1){
									r = (mc->mRadius + yc->mRadius) * 0.5f;
									gap = (rand() % (r * 2) - r);
								}
								else if (CSceneTitle::mCPU_Level == 2){
									r = (mc->mRadius + yc->mRadius) * 0.4f;
									gap = (rand() % (r * 2) - r);
								}
								else if (CSceneTitle::mCPU_Level == 3){
									r = (mc->mRadius + yc->mRadius) * 0.2f;
									gap = (rand() % (r * 2) - r);
								}
								//���̃|�C���g��ݒ肷��
								SetNextPoint(mpPoint, gap, false);								
							}
						}
					}
				}
			}
		}
		break;
	}
}

void CEnemy::TaskCollision()
{
	mColBody.ChangePriority();
	mColTire.ChangePriority();
	mSearch.ChangePriority();
	CollisionManager.Collision(&mColBody);
	CollisionManager.Collision(&mColTire);
	CollisionManager.Collision(&mSearch);
}

//SetNextPoint(���݂̃|�C���g,�덷�̗L��,�덷�̗�,���݂̃|�C���g���ŏI�|�C���g��)
void CEnemy::SetNextPoint(CPoint *current_point, int gap_amount, bool iscurrentpointlast){
	bool gap = true;
	if (CSceneTitle::mCource_Number == 5){
		if (mChecks < 1){//���ԃ|�C���g1�܂Ō덷����
			gap = false;
		}
	}

	//�u��O�̖ڕW�n�_�v�̍X�V
	mVPoint_prev = mpPoint->mPosition;
	//���|�C���g�͍ŏI�|�C���g��
	if (iscurrentpointlast){
		//�덷�̗L��
		if (gap){
			mVPoint = mPoint->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap_amount;
		}
		else{
			mVPoint = mPoint->mPosition;
		}
		//�ŏ��̖ڕW�n�_�ɖ߂�
		mpPoint = mPoint;
	}
	else{
		CPoint *nexp = GetNextPoint(current_point, iscurrentpointlast);
		//�덷�̗L��
		if (gap){//�Ԃ�L��
			mVPoint = nexp->mPosition + CVector(1.0f, 0.0f, 1.0f)*gap_amount;
		}
		else{//�Ԃꖳ��
			mVPoint = nexp->mPosition;
		}
		//���̖ڕW�n�_�Ɉڂ�
		mpPoint = nexp;
	}	
}

//���̃|�C���g�̎擾
CPoint* CEnemy::GetNextPoint(CPoint *current_point, bool iscurrentpointlast){
	return current_point->GetNextPoint();
}

//�U���|�C���g
CPoint *CEnemy::mPoint;