#ifndef COBJFLOOR_H
#define COBJFLOOR_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CObjFloor :public CCharacter{
public:
	//�R���C�_�̒ǉ�
	CCollider *mpCollider;
	//�R���X�g���N�^�ŏ����ݒ�
	CObjFloor(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
	~CObjFloor();

	void Update();//virtual�c�Ē�`�\�ɂȂ�?

};
#endif
