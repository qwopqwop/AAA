#include "CCameraPos.h"
//#include "CKey.h"

#include "CItem.h"
#include "CSceneTitle.h"

#include "CTaskManager.h"

#include <stdio.h>
#include <math.h>

#define PI 3.141592

CCameraPos *CCameraPos::mpCamera = 0;

#define G (9.8f / 90.0f)//�d�͉����x//60.0f
#define JUMPV0 (4.0f*4.0f)//�W�����v����//4.0f

#define MAXSPEED 20.0f //4.5f+3.0f *5//�Ԃ̍ō����x
#define MAXSPEED_BACK 1.0f*2 *2//�Ԃ̌�ނ���ő呬�x
#define CAR_POWER 0.05f*2 //*2//1�t���[���ӂ�̎Ԃ̉������Ă�����
#define CAR_BREAK_POWER 0.025f*2 *2//�O�i���̃u���[�L�̋���
//#define MAXSPEED_LIMITUP 10.0f //�u�[�X�g���̍ō����x�̏���˔j��

#define DECELERATE 0.05f*2 //�Ԃ̌��������
#define FIX_ANGLE_VALUE 0.5f*2 //�p�x��0�x�Ɍ����Ē���������(���X�EZ�p)

#define JUMPER01_POWER 3.0f;//�W�����v��1�ɂ��W�����v�̋���

//#define MAXSPEED 7.0f //�Ԃ̍ō����x
//#define MAXSPEED_BACK 2.0f //�Ԃ̌�ނ���ő呬�x
//#define CAR_POWER 0.1f //1�t���[���ӂ�̎Ԃ̉������Ă�����
//#define CAR_BREAK_POWER 0.05f //�O�i���̃u���[�L�̋���
//
//#define DECELERATE 0.1f //�Ԃ̌��������
//#define FIX_ANGLE_VALUE 0.5f //�p�x��0�x�Ɍ����Ē���������(���X�EZ�p)

CCameraPos::CCameraPos()
//�ԑ̂�Y���W��0.0f�ɂ������񂾂��ǁE�E�E
//0.0f�ɂ�����ԑ̂������Ă�悤�Ɍ����Ă��܂�
:mColCam(this, CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 0.0f), CVector(1.0f, 1.0f, 1.0f), 10.0f * 3)
{
	mpCamera = this;	

	CanMove = false;	
	mTag = EPLAYER;

	//if (CSceneTitle::mMode == 3){
	//	//�X�^�[�g�n�_�̍��W��ݒ�;
	//	mStartPoint[0] = 0.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = 80.0f;
	//	mStartRotation = 90.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else if (CSceneTitle::mMode == 2){
	//	//�X�^�[�g�n�_�̍��W��ݒ�;
	//	mStartPoint[0] = 2222.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -2510.0f;
	//	mStartRotation = 0.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else if (CSceneTitle::mMode == 4){
	//	//�X�^�[�g�n�_�̍��W��ݒ�;
	//	mStartPoint[0] = 0.0f;  mStartPoint[1] = 0.0f;  mStartPoint[2] = 0.0f;
	//	//mStartPoint[0] = 450.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -50.0f;
	//	mStartRotation = 180.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else if (CSceneTitle::mMode == 5){
	//	//�X�^�[�g�n�_�̍��W��ݒ�;
	//	mStartPoint[0] = -3755.5f;  mStartPoint[1] = 13.5f;  mStartPoint[2] = 16060.5f;
	//	mStartRotation = -145.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//else{
	//	//�X�^�[�g�n�_�̍��W��ݒ�;
	//	mStartPoint[0] = 350.0f;  mStartPoint[1] = -13.538f;  mStartPoint[2] = -100.0f;
	//	mStartRotation = 0.0f;
	//	mRotation.mY = mStartRotation;
	//}
	//mPosition = CVector(mStartPoint[0], mStartPoint[1], mStartPoint[2]);
	CCharacter::Update();

	mColCam.mTag = CCollider::ECAMERA;

	mPosition = CVector(0.0f, 170.0f, 400.0f) * CPlayer::mpPlayer->mMatrixRotate * CPlayer::mpPlayer->mMatrixTranslate;
}

void CCameraPos::Update(){	
	
	//mPosition = CVector(0.0f, 0.0f, 0.0f) * mMatrixRotate * mMatrixTranslate;
	//CCharacter::Update();
	//mPosition = CVector(0.0f, 0.0f, 0.0f) * //mMatrixScale * 
	//	CMatrix().RotateZ(0) *
	//	CMatrix().RotateX(0) *
	//	CMatrix().RotateY(0)
	//	*mMatrixTranslate;//�ł��Ă�H
	//mPosition = CVector(0.0f,170.0f,400.0f) * CPlayer::mpPlayer->mMatrixRotate * CPlayer::mpPlayer->mMatrixTranslate;
	////mPosition.mY += 120.0f;
	////mPosition.mZ += 100.0f;	
	//mPosition = CVector(mADMoveX, 0.0f, mWSMoveZ + mCarSpeed) * mMatrixRotate * mMatrixTranslate;
	

	//printf(".");//�������̓v���C���[���狗�������Ȃ藣��ĂĂ��������������Ă����B

	CCharacter::Update();	
}

void CCameraPos::Collision(CCollider *mc, CCollider *yc){
	//���g�̃R���C�_�^�C�v�̔���
	switch (mc->mType){
	case CCollider::ESPHERE://���R���C�_
		//����̃R���C�_���O�p�R���C�_�̎�
		if (yc->mType == CCollider::ETRIANGLE){
			//�����̃R���C�_���{�̂̎�
			if (mc->mTag == CCollider::ECAMERA){				
				//else{ �ȊO�͒ʉ߉\
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
					//�����͍s���邪�A�����̃p�l���͒ʉ߉\
				}
				else{
					CVector adjust;//�����p�x�N�g��
					//		//�O�p�`�Ƌ��̏Փ˔���
					if (CCollider::CollisionTriangleSphere(yc, mc, &adjust)){
						//�ʒu�̍X�V
						mPosition = mPosition - adjust * -1;
						//�s��̍X�V
						CCharacter::Update();
						//printf("�J��������\n");
					}
				}
			}
		}

		if (yc->mType == CCollider::ESPHERE){
			//�J�����͎ԂƏՓ˂��Ȃ�
			if (mc->mTag == CCollider::ECAMERA){
				//printf("1");
				if (yc->mpParent->mTag == CCharacter::EPLAYER){

					//printf("2");
					if (yc->mTag == CCollider::ECAMERA_RANGE){

						//printf("3");
						CVector adjust;//�����p�x�N�g��
						////		//�����m�̏Փ˔���
						if (CCollider::Collision(mc, yc, &adjust)==false){
							//printf("�͈͊O\n");
							//printf("%f  %f  %f\n", yc->mpParent->mPosition.mX, yc->mpParent->mPosition.mY, yc->mpParent->mPosition.mZ);
							
							/*mPosition = yc->mpParent->mPosition;
							mPosition.mY += 100.0f;*/
							////�ʒu�̍X�V
							//mPosition = mPosition - adjust * -1;


							mPosition = yc->mpParent->mPosition;
							mPosition = mPosition - adjust*-1;

							//�s��̍X�V
							CCharacter::Update();

						/*	mPosition.mX += 1.0f;
							mPosition.mY += 1.0f;
							mPosition.mZ += 1.0f;
							CCharacter::Update();*/

							CVector aio = CVector(0.0f, 0.0f, -1.0f)*CMatrix().RotateY(yc->mpParent->mRotation.mY);
							mPosition = mPosition + aio;
							printf("x:%f y:%f z:%f\n", aio.mX, aio.mY, aio.mZ);
							CCharacter::Update();

							CVector adj2;
							if (CCollider::Collision(mc, yc, &adj2)){
								//printf("?");
								mPosition = mPosition - adj2 *-1;
								//�s��̍X�V
								CCharacter::Update();
							}

							//mPosition = mPosition - adjust * -1;

							
						}
					}
				}
			}
		}
		//if (yc->mType == CCollider::ESPHERE){
		//	if (CCollider::Collision(mc, yc)){				
		//		//�J�����͎ԂƏՓ˂��Ȃ�
		//		if (mc->mTag == CCollider::ECAMERA){
		//			if (yc->mpParent->mTag == CCharacter::EPLAYER){
		//				if (yc->mTag == CCollider::ECAMERA_RANGE){
		//					CVector adjust;//�����p�x�N�g��
		//					////		//�����m�̏Փ˔���
		//					if (CCollider::Collision(mc, yc, &adjust)){
		//						////�ʒu�̍X�V
		//						//mPosition = mPosition - adjust * -1;
		//						////�s��̍X�V
		//						//CCharacter::Update();

		//						printf("�͈͊O\n");
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
		break;
	}
}


void CCameraPos::TaskCollision()
{
	mColCam.ChangePriority();
	CollisionManager.Collision(&mColCam);
}