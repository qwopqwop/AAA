#include "CRoadManager.h"
#include "CPoint.h"
#include "CEnemy.h"

//CRoadManager(���f���f�[�^�̃|�C���^,�ʒu,��],�g��k��,�X�^�[�g�ʒu,�i�s����)
CRoadManager::CRoadManager(CModel *model, const CVector& position, const CVector& rotation, const CVector& scale, const CVector& startPos, const CVector& foward)
: CObjFloor(model, position, rotation, scale)
{
	Init(model, position, rotation, scale, startPos, foward);
}

void CRoadManager::Init(CModel* pmodel, const CVector& pos, const CVector& rot, const CVector& scale, const CVector& startPos, const CVector& foward)
{
	//mMatrix�̍X�V
	mPosition = pos;
	mRotation = rot;
	mScale = scale;
	CCharacter::Update();

	/*printf("//�i�P�jpmodel����O�p�`�|���S���̐����擾����\n");
	printf("//�i�Q�j�|���S���̐����x�N�g���̔z����쐬����B\n");
	printf("//�i�R�j�O�p�`�|���S���̊e���_��mMatrix���|���ă��[���h���W�����߁A�O�p�`�̏d�S�̍��W�����߂�\n");
	printf("//�i�S�j�z��̃x�N�g���̒l�ɁA�O�p�`�|���S���̏d�S���W�������Ă���\n");*/
	//�����P�@�O�p�`�|���S���̏d�S���W�����߂Ĕz��ɂ���
	//�i�P�jpmodel����O�p�`�|���S���̐����擾����
	int triangle_size = pmodel->mTriangles.size();
	printf("�O�p�`�|���S���̐��F%d\n", triangle_size);
	
	//�i�Q�j�|���S���̐����x�N�g���̔z����쐬����B
	CVector *polygonarray;
	polygonarray = new CVector[triangle_size];
	printf("pmodel->mpVertex[0] = %f\n", pmodel->mpVertex[0]);
	for (int i = 0; i < pmodel->mMaterials.size(); i++){
		printf("mVertexNum : %f\n", pmodel->mMaterials[i].mVertexNum);
	}


	////�i�R�j�O�p�`�|���S���̊e���_��mMatrix���|���ă��[���h���W�����߁A�O�p�`�̏d�S�̍��W�����߂�
	////�e�R���C�_�̒��_�����[���h���W�֕ϊ�//�Q�l
	//v[0] = y->mV[0] * y->mMatrix * y->mpParent->mMatrix;
	//v[1] = y->mV[1] * y->mMatrix * y->mpParent->mMatrix;
	//v[2] = y->mV[2] * y->mMatrix * y->mpParent->mMatrix;
	int men = 0;
	for (int i = 0; i < triangle_size; i++){
		polygonarray[i] * mMatrix;
		men++;
		printf("%4d  ",men);
		printf("%f, %f, %f\n", polygonarray[i].mX, polygonarray[i].mY, polygonarray[i].mZ);
	}

	//�i�S�j�z��̃x�N�g���̒l�ɁA�O�p�`�|���S���̏d�S���W�������Ă���


	//�����Q�@�擪�f�[�^�̒T��
	//�d�S���W�̔z�񂩂�A�X�^�[�g�ʒu�ɍł��߂��d�S�����߂�B
	//���߂��d�S�Ɣz��̐擪�����ւ���B

	//�����R�@2�Ԗڃf�[�^�̒T��
	//�d�S���W��2�Ԗڈȍ~�̔z��ɂ��āA�擪�̃f�[�^����i�s�����ɂ���d�S�̒��ŁA
	//�ł��擪�ɋ߂��d�S��T���A�z���2�ԖڂƓ���ւ���B

	//�����S�@3�Ԗڈȍ~�̃f�[�^�̕��ѕς�
	//�z���3�Ԗڈȍ~�ɂ��Ĉȉ��̎菇�ŕ��ѕς���
	//�i�P�j���݈ʒu��3�Ԗڂɂ���
	//�i�Q�j���݈ʒu�̈�O�̒l�ɍł��߂��d�S��T���A���݈ʒu�Ɠ���ւ���
	//�i�R�j���݈ʒu������ɂ��āA�i�Q�j���J��Ԃ�

	//�����T�@�d�S�̔z�񂩂�CPoint�𐶐�����
	//�i�P�j�z��̍Ō�ƍŌォ��1�O�̏d�S���A���ԍ��W�����߁ACPoint�𐶐�����B
	//�i�Q�j�z��̌�3�ڂ���O�Ɍ����āA2�����ԍ��W������CPoint�𐶐�����B
	//�i�R�j�������I���ƁA�ŏ��ɍ쐬����CPoint�̎��|�C���^�ɍŌ��CPoint�̃|�C���^��������

	////�ŏ��̖ڕW��ݒ�
	//CEnemy::mPoint = �Ō��CPoint�̃|�C���^�̎��|�C���^;
}
