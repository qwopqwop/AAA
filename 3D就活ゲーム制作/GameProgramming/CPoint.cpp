#include "CPoint.h"

CPoint::CPoint()
: mCollider(this, CVector(), CVector(), CVector(1.0f, 1.0f, 1.0f), 1.0f)
{
	/*mpPoint = this;*/

	mTag = EPOINT;//�^�O���|�C���g�ɂ���
	mScale = CVector(1.0f, 1.0f, 1.0f);
}

void CPoint::Set(const CVector &pos, float r){
	mPosition = pos;
	mCollider.mRadius = r;
	CCharacter::Update();
}

CPoint::CPoint(const CVector &pos, float r, int pointnumber)
: mCollider(this, CVector(), CVector(), CVector(1.0f, 1.0f, 1.0f), 1.0f)
{
	mPosition = pos;
	mCollider.mRadius = r;
	mTag = EPOINT;//�^�O���|�C���g�ɂ���
	CCharacter::Update();
}
