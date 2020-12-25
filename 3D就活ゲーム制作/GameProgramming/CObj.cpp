#include "CObj.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CTaskManager.h"

#define DOOR_RANGEOFMOTION 90 //�h�A�̉���
#define DOOR_OPENSPEED 2 //�h�A�̊J�����x
#define ROLLINGSPEED 1.0f //��]���鑫��̉�]���x

//�R���X�g���N�^
//model:���f���̃|�C���^ position:�ʒu rotation:��] scale:�g�k
CObj::CObj(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale, int objnum)
:mpCollider(0)
{
	mpModel = model;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;
	mObjNumber = objnum;

	if (mObjNumber == 9){
		mTag = EWATER;
	}
	if (mObjNumber >= 18 && mObjNumber <= 20){
		mTag = ECLEARWATER;
	}
	/*if (mObjNumber == 20){
		mTag = ECLEARWATER;
	}*/

	if (mObjNumber == 101){
		//�`�F�b�N�|�C���g
		mTag = ECHECKPOINT;
		//�D��x�ύX
		mPriority = 0;
		CTaskManager::Get()->Remove(this);
		CTaskManager::Get()->Add(this);
	}
	if (mObjNumber == 102){
		//�`�F�b�N�|�C���g
		mTag = ECHECKPOINT2;
		//�D��x�ύX
		mPriority = 0;
		CTaskManager::Get()->Remove(this);
		CTaskManager::Get()->Add(this);
	}
	if (mObjNumber == 103){
		//�`�F�b�N�|�C���g
		mTag = ECHECKPOINT3;
		//�D��x�ύX
		mPriority = 0;
		CTaskManager::Get()->Remove(this);
		CTaskManager::Get()->Add(this);
	}
	if (mObjNumber == 111){
		//������
		mTag = EDASHBOARD;
		//mpCollider->mPosition.mY += 5.0f;
		//mPosition.mY += 5.0f;
	}
	if (mObjNumber == 112){
		//�Ő�(����)
		mTag = EGRASS;
	}
	if (mObjNumber == 200){
		//��
		mTag = EWALL;
	}


	if (mObjNumber == 99 || mObjNumber == 575){
		//�����蔻��𐶐����Ȃ�
		//����ɂ�蓖���蔻��̐��𑝂₷�قǏ������d���Ȃ邱�Ƃ����؂��ꂽ
		//�t�ɓ����蔻��𑝂₳�Ȃ���΂����̂ŁA�K�v�Ȃ������蔻��͂ǂ�ǂ�J�b�g���Ă�����B
	}
	else{
		CCharacter::Update();
		//���f���̎O�p�`�̐����A�R���C�_�̔z����쐬���܂�
		mpCollider = new CCollider[model->mTriangles.size()];
		for (int i = 0; i < model->mTriangles.size(); i++){
			//�R���C�_���O�p�`�R���C�_�Őݒ肵�Ă����܂�
			mpCollider[i].SetTriangle(this,
				model->mTriangles[i].mV[0],
				model->mTriangles[i].mV[1],
				model->mTriangles[i].mV[2]);
			//if (mObjNumber == 111){
			//	//�������̓����蔻�肾����Ɉړ�������
			//	model->mTriangles[i].mV[0] = model->mTriangles[i].mV[0] + CVector(0.0f, 5.0f, 0.0f);
			//	model->mTriangles[i].mV[1] = model->mTriangles[i].mV[1] + CVector(0.0f, 5.0f, 0.0f);
			//	model->mTriangles[i].mV[2] = model->mTriangles[i].mV[2] + CVector(0.0f, 5.0f, 0.0f);
			//}
			mpCollider[i].ChangePriority();
		}
	}


	//mEnabled = false;
}
//�f�X�g���N�^
CObj::~CObj(){
	//�R���C�_������΍폜
	if (mpCollider){
		//delete[] �z����폜
		delete[] mpCollider;
	}
}

void CObj::Collision(CCollider *m, CCollider *y){
	/*if (mObjNumber != 9){
		if (CCollider::Collision(m, y)){
			if (y->mpParent->mTag == CCharacter::EPLAYER){
				mEnabled = false;
			}
		}
	}*/

	/*if (y->mpParent->mTag == CCharacter::EBULLET){
		if (CCollider::Collision(m, y)){
			if (y->mpParent->mTag == CCharacter::EBULLET){
				mEnabled = false;
				
			}
		}
	}*/
	/*if (CCollider::Collision(m, y)){
		if (y->mpParent->mTag == CCharacter::EBULLET){
			mEnabled = false;

		}
	}*/

	//if (mObjNumber == 13){//��
	//	if (CCollider::Collision(m, y)){
	//		if (y->mpParent->mTag == CCharacter::EPLAYER){
	//			mEnabled = false;
	//		}
	//	}
	//}
	
}

void CObj::Update(){
	if (mObjNumber == 18){
		//�{�[�g���擾���Ȃ��ƕ\������Ȃ���I
		if (CPlayer::mpPlayer->mHaveBoat == false){
			mScale.mY = 0.0f;
		}
		//�擾����Ε\��������I
		else{
			mScale.mY = 35.7f;
			mRotation.mY += 3;
		}
	}
	//if (mObjNumber == 19){
	//	//�{�[�g���擾���Ȃ��ƕ\������Ȃ���I
	//	if (CPlayer::mpPlayer->mHaveBoat == false){
	//		mScale.mY = 0.0f;
	//	}
	//	//�擾��������Ε\��������I
	//	else{
	//		mScale.mY = 35.7f;
	//		mRotation.mY -= 3;
	//	}
	//}

	if (mObjNumber == 21){//���̔ԍ�
		//��𗬂��u���b�N�H
		mPosition.mX += 0.4f;
		if (mPosition.mX > 200.0f){
			mPosition.mX = -200.0f;
		}
	}
	if (mObjNumber == 25){//�񃌉�
		mRotation.mX += ROLLINGSPEED;
	}

	if (mObjNumber == 31){//���̔ԍ�
		//��𗬂��u���b�N�H
		mPosition.mX += 2.5f;
		if (mPosition.mX > 3200.0f){
			mPosition.mX = 0.0f;
		}
	}

	if (mObjNumber == 334){
		mScale.mX += 0.1f;
		mScale.mY += 0.1f;
		mScale.mZ += 0.1f;
	}
	if (mObjNumber == 575){
		mPosition.mX = CPlayer::mpPlayer->mPosition.mX;
		mPosition.mZ = CPlayer::mpPlayer->mPosition.mZ;
		mRotation.mY = CPlayer::mpPlayer->mRotation.mY;
		//�����蔻��͕K�v�Ȃ�
	//	mPosition.mY = CPlayer::mpPlayer->mPosition.mY;
	}

	CCharacter::Update();

	//���܂������Ȃ�
	/*if (mObjNumber == 13){
		if (mDeleteFlag){
			mEnabled = false;
		}
	}*/

	if (mObjNumber == 10){
		if (CItem::mTargetAmount <= 0){
			mEnabled = false;
		}
	}
	if (mObjNumber == 22){
		if (CPlayer::mpPlayer->mIsGetKey){
			//�h�A���J��
			//mEnabled = false;
			if (mRotation.mY < DOOR_RANGEOFMOTION){
				mRotation.mY += DOOR_OPENSPEED;
				mPosition.mX -= DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
				mPosition.mZ -= DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
			}
		}
		else{
			//�h�A���܂�
			if (mRotation.mY > 0.0f){
				mRotation.mY -= DOOR_OPENSPEED;
				mPosition.mX += DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
				mPosition.mZ += DOOR_OPENSPEED * mScale.mX / DOOR_RANGEOFMOTION;
			}
		}
	}
}

void CObjection::TaskCollision()
{
	/*mColBody.ChangePriority();
	mColTire.ChangePriority();
	CollisionManager.Collision(&mColBody);
	CollisionManager.Collision(&mColTire);*/
	//mpCollider.ChangePriority();
	//CollisionManager.Collision(&mpCollider);
}