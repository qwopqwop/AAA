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
	/*printf("pmodel->mpVertex[0] = %f\n", pmodel->mpVertex[0]);
	for (int i = 0; i < pmodel->mMaterials.size(); i++){
		printf("mVertexNum : %f\n", pmodel->mMaterials[i].mVertexNum);
	}*/

	////�i�R�j�O�p�`�|���S���̊e���_��mMatrix���|���ă��[���h���W�����߁A�O�p�`�̏d�S�̍��W�����߂�
	////�e�R���C�_�̒��_�����[���h���W�֕ϊ�//�Q�l
	//v[0] = y->mV[0] * y->mMatrix * y->mpParent->mMatrix;
	//v[1] = y->mV[1] * y->mMatrix * y->mpParent->mMatrix;
	//v[2] = y->mV[2] * y->mMatrix * y->mpParent->mMatrix;
	for (int i = 0; i < triangle_size; i++){
		CVector v[3], sv;
		v[0] = pmodel->mTriangles[i].mV[0] * mMatrix;
		v[1] = pmodel->mTriangles[i].mV[1] * mMatrix;
		v[2] = pmodel->mTriangles[i].mV[2] * mMatrix;
		sv = CVector((v[0].mX + v[1].mX + v[2].mX) / 3.0f, (v[0].mY + v[1].mY + v[2].mY) / 3.0f, (v[0].mZ + v[1].mZ + v[2].mZ) / 3.0f);

		//�i�S�j�z��̃x�N�g���̒l�ɁA�O�p�`�|���S���̏d�S���W�������Ă���
		polygonarray[i] = sv;
		printf("%4d  �@", i + 1);
		printf("%13f, %13f, %13f\n", polygonarray[i].mX, polygonarray[i].mY, polygonarray[i].mZ);
	}


	//�����Q�@�擪�f�[�^�̒T��
	//�d�S���W�̔z�񂩂�A�X�^�[�g�ʒu�ɍł��߂��d�S�����߂�B
	//���߂��d�S�Ɣz��̐擪�����ւ���B
	CVector spos = startPos;
	CVector vdist;
	CVector nearestvec = CVector(0.0f, 0.0f, 0.0f);
	int nearest_arraynum;
	for (int i = 0; i < triangle_size; i++){
		
		vdist = spos - polygonarray[i];
		//��ԍŏ��̏d�S���W�͖������ɍł��߂����Ƃɂ���
		if (i == 0){
			nearestvec = vdist;
			printf("%4d�Ԗ�  �����F%8.2f\n", i, vdist.Length());
			nearest_arraynum = i;
		}
		else{
			if (vdist.Length() < nearestvec.Length()){
				printf("%4d�Ԗ�  �����F%8.2f\n", i, vdist.Length());
				nearestvec = vdist;
				nearest_arraynum = i;
			}
		}
	}
	printf("�ł��߂����W�̔z��c%4d�Ԗڂ̔z��\n", nearest_arraynum);
	printf("�����F%8.2f\n", nearestvec.Length());
	printf("�����F%8.2f\n", (spos - polygonarray[nearest_arraynum]).Length());
	//���߂��d�S�Ɣz��̐擪�����ւ���B
	CVector temp = polygonarray[0];
	polygonarray[0] = polygonarray[nearest_arraynum];
	polygonarray[nearest_arraynum] = temp;	
	printf("---�ϊ���---\n");
	printf("%4d�F�����F%8.2f\n", 0,(spos - polygonarray[0]).Length());
	printf("%4d�F�����F%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	////�e�R���C�_�̒��S���W�����߂�
	////���_�~�R���C�_�̕ϊ��s��~�e�̕ϊ��s��
	//CVector mpos = CVector() * m->mMatrix * m->mpParent->mMatrix;
	//CVector ypos = CVector() * y->mMatrix * y->mpParent->mMatrix;
	////���S���璆�S�ւ̃x�N�g�������߂�
	//mpos = mpos - ypos;
	////���S�̋��������a�̍��v��菬�����ƏՓ�
	//if (m->mRadius + y->mRadius > mpos.Length()) {
	//	if (mpos.Length() == 0){
	//		//���S�ɓ������W�̎��͔��a�̘a�̕��A��Ɉړ�������
	//		*a = CVector(0.0f, 1.0f, 0.0f) * (m->mRadius + y->mRadius);//CVector(0.0f, 0.1f, 0.0f);
	//	}
	//	else{
	//		*a = mpos.Normalize() * (m->mRadius + y->mRadius - mpos.Length());
	//	}
	//}

	//�����R�@2�Ԗڃf�[�^�̒T��
	//�d�S���W��2�Ԗڈȍ~�̔z��ɂ��āA�擪�̃f�[�^����i�s�����ɂ���d�S�̒��ŁA
	//�ł��擪�ɋ߂��d�S��T���A�z���2�ԖڂƓ���ւ���B
	////�|�C���g�ւ̃x�N�g�������߂�
	//CVector dir = mVPoint - mPosition;
	////�������ւ̃x�N�g�������߂�
	//CVector left = CVector(1.0f, 0.0f, 0.0f) * CMatrix().RotateY(mRotation.mY);
	//left.Dot(dir)

	CVector poly_forward = foward;
	float sdot = 0;
	int sArraynum = 0;
	for (int i = 0; i < triangle_size; i++){
		CVector dir = polygonarray[i] - polygonarray[0];
		//printf("���ρF%8.2f\n", polygonarray[i].Dot(polygonarray[i] - polygonarray[0]));
		printf("[%4d]�̓��ρF%8.2f", i, poly_forward.Dot(polygonarray[i] - polygonarray[0]));

		if (poly_forward.Dot(polygonarray[i] - polygonarray[0]) > 0){
			printf(" �@���\n");
			if (sdot == 0 || sdot > poly_forward.Dot(polygonarray[i] - polygonarray[0])){
				sdot = poly_forward.Dot(polygonarray[i] - polygonarray[0]);
				sArraynum = i;
			}
			
		}
		else{
			printf("\n");
		}
	}
	//[490]��[492]�ӂ肪2�ԖڂɂȂ�͂��c�H
	printf("[%d] %f\n", sArraynum, sdot);
	temp = polygonarray[1];
	polygonarray[1] = polygonarray[sArraynum];
	polygonarray[sArraynum] = temp;
	printf("---����ɕϊ���---\n");
	/*for (int i = 0; i < triangle_size; i++){
		printf("%4d�F�����F%8.2f\n", i, (spos - polygonarray[i]).Length());
	}*/
	printf("%4d�F�����F%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	printf("%4d�F�����F%8.2f\n", sArraynum, (spos - polygonarray[sArraynum]).Length());
	

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

	delete[] polygonarray;
}
