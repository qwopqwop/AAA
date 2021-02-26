#include "CObjFloor.h"
#include "CItem.h"
#include "CPlayer.h"
#include "CTaskManager.h"

//�R���X�g���N�^
//model:���f���̃|�C���^ position:�ʒu rotation:��] scale:�g�k
CObjFloor::CObjFloor(CModel *model, const CVector &position, const CVector &rotation, const CVector &scale)
:mpCollider(0)
{
	mpModel = model;
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	//mTag = ENONE;//�����l�ƈꏏ

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
CObjFloor::~CObjFloor(){
	//�R���C�_������΍폜
	if (mpCollider){
		//delete[] �z����폜
		delete[] mpCollider;
	}
}
//�X�V����
void CObjFloor::Update(){
	CCharacter::Update();
}