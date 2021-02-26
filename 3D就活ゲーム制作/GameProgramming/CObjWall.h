//#ifndef COBJWALL_H
//#define COBJWALL_H
//
//#include "CObj.h"
////#include"CModel.h"
//
//class CObjWall :public CObj{
//public:
//	//CObjWall(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
//
//	////�X�V�����̃I�[�o�[���C�h
//	//void Update();
//
//	//�R���C�_�̒ǉ�
//	CCollider *mpCollider;
//	//�R���X�g���N�^�ŏ����ݒ�
//	CObjWall(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
//	~CObjWall();
//
//	int mObjNumber;
//
//	////�Փˏ���
//	//void Collision(CCollider *m, CCollider *y);
//
//	void Update();
//
//	/*CObjWall();*/
//};
//#endif


#ifndef COBJWALL_H
#define COBJWALL_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CObjWall :public CCharacter{
public:
	//�R���C�_�̒ǉ�
	CCollider *mpCollider;
	//�R���X�g���N�^�ŏ����ݒ�
	CObjWall(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
	~CObjWall ();

	int mObjNumber;

	void Update();//virtual�c�Ē�`�\�ɂȂ�?

};
#endif


