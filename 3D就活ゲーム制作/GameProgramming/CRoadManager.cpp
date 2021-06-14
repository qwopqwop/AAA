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
		//��ԍŏ��̏d�S���W�͖������ɍł��߂����̂Ƃ���
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
	

	//�����R�@2�Ԗڃf�[�^�̒T��
	//�d�S���W��2�Ԗڈȍ~�̔z��ɂ��āA�擪�̃f�[�^����i�s�����ɂ���d�S�̒��ŁA
	//�ł��擪�ɋ߂��d�S��T���A�z���2�ԖڂƓ���ւ���B
	CVector poly_forward = foward;
	float sdot = 0;
	int sArraynum = 0;
	bool isfirst = true;
	for (int i = 0 + 1; i < triangle_size; i++){
		CVector dir = polygonarray[i] - polygonarray[0];
		//printf("���ρF%8.2f\n", polygonarray[i].Dot(polygonarray[i] - polygonarray[0]));
		printf("[%4d]�̓��ρF%8.2f", i, poly_forward.Dot(polygonarray[i] - polygonarray[0]));

		//���ς��v���X�ɂȂ���̂����
		if (poly_forward.Dot(polygonarray[i] - polygonarray[0]) > 0){
			//���̒��ōł�polygonarray[0]�Ƌ߂����̂�[1]�ƂȂ�
			printf(" �@���\n");

			printf(" �����F%8.2f\n", (polygonarray[i] - polygonarray[0]).Length());
			vdist = polygonarray[i] - polygonarray[0];
			//��ԍŏ��̃v���X���ς͂����ݒ�
			if (isfirst){
				nearestvec = vdist;
				printf("%4d�Ԗ�  �����F%8.2f\n", i, vdist.Length());
				sArraynum = i;
				isfirst = false;
			}
			else{
				if (vdist.Length() < nearestvec.Length()){
					printf("%4d�Ԗ�  �����F%8.2f\n", i, vdist.Length());
					nearestvec = vdist;
					sArraynum = i;
				}
			}

		}
		else{
			printf("\n");
		}
	}
	printf("[%d] %f\n", sArraynum,  nearestvec.Length());
	temp = polygonarray[1];
	polygonarray[1] = polygonarray[sArraynum];
	polygonarray[sArraynum] = temp;
	printf("---����ɕϊ���---\n");
	/*for (int i = 0; i < triangle_size; i++){
		printf("%4d�F�����F%8.2f\n", i, (spos - polygonarray[i]).Length());
	}*/
	printf("%4d�F�����F%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	printf("%4d�F�����F%8.2f\n", sArraynum, (spos - polygonarray[sArraynum]).Length());
//	printf("[%d]    %.2f  %.2f  %.2f", sArraynum, polygonarray[sArraynum].mX, polygonarray[sArraynum].mY, polygonarray[sArraynum].mZ);
	////[490]��[492]�ӂ肪2�ԖڂɂȂ�͂��c�H
	//printf("[%d] %f\n", sArraynum, sdot);
	//temp = polygonarray[1];
	//polygonarray[1] = polygonarray[sArraynum];
	//polygonarray[sArraynum] = temp;
	//printf("---����ɕϊ���---\n");
	for (int i = 0; i < triangle_size; i++){
		printf("%4d�F�����F%8.2f\n", i, (spos - polygonarray[i]).Length());
	}
	//printf("%4d�F�����F%8.2f\n", nearest_arraynum, (spos - polygonarray[nearest_arraynum]).Length());
	//printf("%4d�F�����F%8.2f\n", sArraynum, (spos - polygonarray[sArraynum]).Length());


	printf("�������������������(�S)��������\n");
	//�����S�@3�Ԗڈȍ~�̃f�[�^�̕��ѕς�
	//�z���3�Ԗڈȍ~�ɂ��Ĉȉ��̎菇�ŕ��ѕς���
	//�i�P�j���݈ʒu��3�Ԗڂɂ���
	//�i�Q�j���݈ʒu�̈�O�̒l�ɍł��߂��d�S��T���A���݈ʒu�Ɠ���ւ���
	//�i�R�j���݈ʒu������ɂ��āA�i�Q�j���J��Ԃ�	
	sArraynum = 0;
	for (int i = 2; i < triangle_size; i++){
		//i�Ԗڂ��O�̃f�[�^�͕��ѕς��̑ΏۂɂȂ�Ȃ�
		isfirst = true;
		for (int j = i; j < triangle_size; j++){
			vdist = polygonarray[i - 1] - polygonarray[j];
			if (isfirst){
				nearestvec = vdist;
				printf("%4d�Ԗ�  �����F%8.2f\n", j, vdist.Length());
				sArraynum = j;
				isfirst = false;

			}
			else{

				if (vdist.Length() < nearestvec.Length()){
					printf("%4d�Ԗ�  �����F%8.2f\n", j, vdist.Length());
					nearestvec = vdist;
					sArraynum = j;
				}
			}
		}
		temp = polygonarray[i];
		polygonarray[i] = polygonarray[sArraynum];
		polygonarray[sArraynum] = temp;
		printf("----Change----\n");
	}

	//�z��S�Ă̍��W���o�͂���
	for (int i = 0; i < triangle_size; i++){
		printf("[%3d]   X:%10.2f  Y:%10.2f  Z:%10.2f�@�@ �X�^�[�g�n�_����̋����F%8.2f\n", i, polygonarray[i].mX, polygonarray[i].mY, polygonarray[i].mZ, (spos - polygonarray[i]).Length());
	}


	//�����T�@�d�S�̔z�񂩂�CPoint�𐶐�����
	//�i�P�j�z��̍Ō�ƍŌォ��1�O�̏d�S���A���ԍ��W�����߁ACPoint�𐶐�����B
	//�i�Q�j�z��̌�3�ڂ���O�Ɍ����āA2�����ԍ��W������CPoint�𐶐�����B
	//�i�R�j�������I���ƁA�ŏ��ɍ쐬����CPoint�̎��|�C���^�ɍŌ��CPoint�̃|�C���^��������
	////�ŏ��̖ڕW��ݒ�
	//CEnemy::mPoint = �Ō��CPoint�̃|�C���^�̎��|�C���^;
	printf("�����T�@-���S���W��-\n");
	CPoint *next, *first;
	CVector center;
	isfirst = true;
	for (int i = triangle_size - 1; i > 0; i -= 2){
		//���S���W
		center = (polygonarray[i] + polygonarray[i - 1]) * (1.0f / 2.0f);
		printf("X:%10.2f  Y:%10.2f  Z:%10.2f\n", center.mX, center.mY, center.mZ);
		if (isfirst){
			first = next = new CPoint(center, 100.0f, nullptr);
			isfirst = false;
		}
		else{
			next = new CPoint(center, 100.0f, next);
		}		
	}
	//�ŏ��ɐ��������|�C���^�̎��|�C���^�̐ݒ�
	first->Set(center, 100.0f, next);
	//CEnemy::mPoint = �Ō��CPoint�̃|�C���^�̎��|�C���^;
	
	//CPoint *next, *first;
	////��납��O�Ɍ������Đ������Ă���
	//first = next = CEnemy::mPoint54 = new CPoint(CVector(-3543.0f, 64.0f, 16338.0f), 100.0f, nullptr);
	////CPoint�������Ɏ��|�C���^��ݒ肵�Ă���
	//next = CEnemy::mPoint53 = new CPoint(CVector(-2086.0f, 64.0f, 17555.0f), 100.0f, next);
	//next = CEnemy::mPoint52 = new CPoint(CVector(-244.0f, 64.0f, 18457.0f), 100.0f, next);
	//next = CEnemy::mPoint51 = new CPoint(CVector(2191.0f, 64.0f, 18783.0f), 100.0f, next);
	//next = CEnemy::mPoint50 = new CPoint(CVector(4095.0f, 64.0f, 18200.0f), 100.0f, next);
	//next = CEnemy::mPoint49 = new CPoint(CVector(5310.0f, 64.0f, 17000.0f), 100.0f, next);
	//next = CEnemy::mPoint48 = new CPoint(CVector(6410.0f, 64.0f, 15188.0f), 100.0f, next);
	//next = CEnemy::mPoint47 = new CPoint(CVector(6945.0f, 64.0f, 13182.0f), 100.0f, next);
	//next = CEnemy::mPoint46 = new CPoint(CVector(6830.0f, 64.0f, 10230.0f), 100.0f, next);
	//next = CEnemy::mPoint45 = new CPoint(CVector(7260.0f, 64.0f, 8450.0f), 100.0f, next);
	//next = CEnemy::mPoint44 = new CPoint(CVector(8371.0f, 64.0f, 7235.0f), 140.0f, next);
	//next = CEnemy::mPoint43 = new CPoint(CVector(9530.0f, 64.0f, 7065.0f), 120.0f, next);
	//next = CEnemy::mPoint42 = new CPoint(CVector(9990.0f, 64.0f, 7720.0f), 120.0f, next);
	//next = CEnemy::mPoint41 = new CPoint(CVector(10595.0f, 64.0f, 8612.0f), 120.0f, next);
	//next = CEnemy::mPoint40 = new CPoint(CVector(12230.0f, 64.0f, 8710.0f), 100.0f, next);
	//next = CEnemy::mPoint39 = new CPoint(CVector(13473.0f, 64.0f, 8125.0f), 100.0f, next);
	//next = CEnemy::mPoint38 = new CPoint(CVector(14424.0f, 64.0f, 6273.0f), 100.0f, next);
	//next = CEnemy::mPoint37 = new CPoint(CVector(15461.0f, 64.0f, 165.0f), 100.0f, next);
	//next = CEnemy::mPoint36 = new CPoint(CVector(14829.0f, 64.0f, -2543.0f), 100.0f, next);
	//next = CEnemy::mPoint35 = new CPoint(CVector(12680.0f, 64.0f, -6967.0f), 100.0f, next);
	//next = CEnemy::mPoint34 = new CPoint(CVector(10123.0f, 64.0f, -7222.0f), 100.0f, next);
	//next = CEnemy::mPoint33 = new CPoint(CVector(8055.0f, 64.0f, -6644.0f), 100.0f, next);
	//next = CEnemy::mPoint32 = new CPoint(CVector(7484.0f, 64.0f, -6189.0f), 100.0f, next);
	//next = CEnemy::mPoint31 = new CPoint(CVector(2925.5f, 375.0f, 955.0f), 100.0f, next);
	//next = CEnemy::mPoint30 = new CPoint(CVector(2494.5f, 500.0f, 1206.0f), 100.0f, next);
	//next = CEnemy::mPoint29 = new CPoint(CVector(2091.5f, 584.0f, 897.0f), 100.0f, next);
	//next = CEnemy::mPoint28 = new CPoint(CVector(1002.5f, 955.0f, -645.0f), 100.0f, next);
	//next = CEnemy::mPoint27 = new CPoint(CVector(-1279.0f, 1575.0f, -2653.0f), 100.0f, next);
	//next = CEnemy::mPoint26 = new CPoint(CVector(-4360.0f, 2420.0f, -4840.0f), 100.0f, next);
	//next = CEnemy::mPoint25 = new CPoint(CVector(-6842.0f, 2983.0f, -5783.0f), 100.0f, next);
	//next = CEnemy::mPoint24 = new CPoint(CVector(-9216.0f, 3625.0f, -6044.0f), 100.0f, next);
	//next = CEnemy::mPoint23 = new CPoint(CVector(-13104.0f, 4671.0f, -5555.0f), 100.0f, next);
	//next = CEnemy::mPoint22 = new CPoint(CVector(-14540.0f, 4960.0f, -5115.0f), 100.0f, next);
	//next = CEnemy::mPoint21 = new CPoint(CVector(-16072.0f, 4970.0f, -3791.0f), 100.0f, next);
	//next = CEnemy::mPoint20 = new CPoint(CVector(-15975.0f, 4870.0f, -1342.0f), 100.0f, next);
	//next = CEnemy::mPoint19 = new CPoint(CVector(-14827.0f, 3966.0f, 1878.0f), 100.0f, next);
	//next = CEnemy::mPoint18 = new CPoint(CVector(-13393.0f, 3333.0f, 3628.0f), 100.0f, next);
	//next = CEnemy::mPoint17 = new CPoint(CVector(-9549.0f, 2700.0f, 4348.0f), 100.0f, next);
	//next = CEnemy::mPoint16 = new CPoint(CVector(-7810.0f, 2425.0f, 4349.0f), 100.0f, next);
	//next = CEnemy::mPoint15 = new CPoint(CVector(-7272.0f, 2312.0f, 4513.0f), 100.0f, next);
	//next = CEnemy::mPoint14 = new CPoint(CVector(-6883.0f, 2161.0f, 5038.0f), 100.0f, next);
	//next = CEnemy::mPoint13 = new CPoint(CVector(-7355.0f, 2075.0f, 6084.0f), 100.0f, next);
	//next = CEnemy::mPoint12 = new CPoint(CVector(-9377.0f, 2233.0f, 6699.0f), 100.0f, next);
	//next = CEnemy::mPoint11 = new CPoint(CVector(-9640.0f, 2148.0f, 7427.0f), 100.0f, next);
	//next = CEnemy::mPoint10 = new CPoint(CVector(-9434.0f, 1850.0f, 8411.0f), 100.0f, next);
	//next = CEnemy::mPoint9 = new CPoint(CVector(-8595.0f, 1570.0f, 9218.0f), 100.0f, next);
	//next = CEnemy::mPoint8 = new CPoint(CVector(-7943.0f, 1421.0f, 9162.0f), 100.0f, next);
	//next = CEnemy::mPoint7 = new CPoint(CVector(-7207.0f, 1320.0f, 8818.0f), 100.0f, next);
	//next = CEnemy::mPoint6 = new CPoint(CVector(-5666.0f, 1050.0f, 8679.0f), 100.0f, next);
	//next = CEnemy::mPoint5 = new CPoint(CVector(-4640.0f, 900.0f, 9125.0f), 100.0f, next);
	//next = CEnemy::mPoint4 = new CPoint(CVector(-4156.0f, 675.0f, 10020.0f), 100.0f, next);
	//next = CEnemy::mPoint3 = new CPoint(CVector(-4254.0f, 555.0f, 10627.0f), 100.0f, next);
	//next = CEnemy::mPoint2 = new CPoint(CVector(-4879.0f, 500.0f, 11355.0f), 100.0f, next);
	//next = CEnemy::mPoint = new CPoint(CVector(-5492.0f, 130.0f, 13030.0f), 100.0f, next);
	////�ŏ��ɐ��������|�C���^�̎��|�C���^�̐ݒ�
	//first->Set(CVector(-3543.0f, 64.0f, 16338.0f), 100.0f, next);


	delete[] polygonarray;
}
