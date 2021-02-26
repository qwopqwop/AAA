//#include "CObjWall.h"
//
//////�R���X�g���N�^
//////model:���f���̃|�C���^ position:�ʒu rotation:��] scale:�g�k
////CObjWall::CObjWall(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
////:mpCollider(0)
////{
////	mpModel = model;
////	mPosition = position;
////	mRotation = rotation;
////	mScale = scale;
////		
////	mTag = EWALL;
////
////	CCharacter::Update();
////	//���f���̎O�p�`�̐����A�R���C�_�̔z����쐬���܂�
////	mpCollider = new CCollider[model->mTriangles.size()];
////	for (int i = 0; i < model->mTriangles.size(); i++){
////		//�R���C�_���O�p�`�R���C�_�Őݒ肵�Ă����܂�
////		mpCollider[i].SetTriangle(this,
////			model->mTriangles[i].mV[0],
////			model->mTriangles[i].mV[1],
////			model->mTriangles[i].mV[2]);
////		mpCollider[i].ChangePriority();
////	}
////}
//
////�f�X�g���N�^
//CObjWall::~CObjWall(){
//	//�R���C�_������΍폜
//	if (mpCollider){
//		//delete[] �z����폜
//		delete[] mpCollider;
//	}
//}
////void CObjWall::Collision(CCollider *m, CCollider *y){
////	//
////}
//void CObjWall::Update(){
//	CCharacter::Update();
//}
////CObjWall::CObjWall(){
////
////}



#include "CObjWall.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CTaskManager.h"

#define DOOR_RANGEOFMOTION 90 //�h�A�̉���
#define DOOR_OPENSPEED 2 //�h�A�̊J�����x
#define ROLLINGSPEED 1.0f //��]���鑫��̉�]���x

#define G (9.8f / 120.0f)//�d�͉����x//60.0f

//�R���X�g���N�^
//model:���f���̃|�C���^ position:�ʒu rotation:��] scale:�g�k
CObjWall::CObjWall(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
:mpCollider(0)
{
	mpModel = model;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	mTag = EWALL;
	
	CCharacter::Update();
	//���f���̎O�p�`�̐����A�R���C�_�̔z����쐬���܂�
	mpCollider = new CCollider[model->mTriangles.size()];
	for (int i = 0; i < model->mTriangles.size(); i++){
		//�R���C�_���O�p�`�R���C�_�Őݒ肵�Ă����܂�
		mpCollider[i].SetTriangle(this,
			model->mTriangles[i].mV[0],
			model->mTriangles[i].mV[1],
			model->mTriangles[i].mV[2]);
		mpCollider[i].ChangePriority();
	}
}
//�f�X�g���N�^
CObjWall::~CObjWall(){
	//�R���C�_������΍폜
	if (mpCollider){
		//delete[] �z����폜
		delete[] mpCollider;
	}
}

void CObjWall::Update(){
	CCharacter::Update();
}