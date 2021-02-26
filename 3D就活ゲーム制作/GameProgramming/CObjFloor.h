#ifndef COBJFLOOR_H
#define COBJFLOOR_H
#include "CCharacter.h"
#include "CCollider.h"
#include "CCollisionManager.h"

class CObjFloor :public CCharacter{
public:
	//コライダの追加
	CCollider *mpCollider;
	//コンストラクタで初期設定
	CObjFloor(CModel*model, const CVector &position, const CVector &rotation, const CVector &scale);
	~CObjFloor();

	void Update();//virtual…再定義可能になる?

};
#endif
